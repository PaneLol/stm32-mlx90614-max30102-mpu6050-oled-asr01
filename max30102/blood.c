#include "blood.h"
#include "usart.h"

uint16_t g_fft_index = 0;         	 	//fft输入输出下标

struct compx s1[FFT_N+16];           	//FFT输入和输出：从S[1]开始存放，根据大小自己定义
struct compx s2[FFT_N+16];           	//FFT输入和输出：从S[1]开始存放，根据大小自己定义

struct
{
	float 	Hp	;			//血红蛋白	
	float 	HpO2;			//氧合血红蛋白
	
}g_BloodWave;//血液波形数据

BloodData g_blooddata = {0};					//血液数据存储

#define CORRECTED_VALUE			47   			//标定血液氧气含量

/*funcation start ------------------------------------------------------------*/
//血液检测信息更新
void blood_data_update(void)
{
	//标志位被使能时 读取FIFO
	g_fft_index=0;
	while(g_fft_index < FFT_N)
	{
		while(MAX30102_INTPin_Read()==0)
		{
			//读取FIFO
			max30102_read_fifo();  //read from MAX30102 FIFO2
			//将数据写入fft输入并清除输出
			if(g_fft_index < FFT_N)
			{
				//将数据写入fft输入并清除输出
				s1[g_fft_index].real = fifo_red;
				s1[g_fft_index].imag= 0;
				s2[g_fft_index].real = fifo_ir;
				s2[g_fft_index].imag= 0;
				g_fft_index++;
			}
		}
	}
}

//血液信息转换
void blood_data_translate(void)
{	
	float n_denom;
	uint16_t i;
	//直流滤波
	float dc_red =0; 
	float dc_ir =0;
	float ac_red =0; 
	float ac_ir =0;
	
	for (i=0 ; i<FFT_N ; i++ ) 
	{
		dc_red += s1[i].real ;
		dc_ir +=  s2[i].real ;
	}
		dc_red =dc_red/FFT_N ;
		dc_ir =dc_ir/FFT_N ;
	for (i=0 ; i<FFT_N ; i++ )  
	{
		s1[i].real =  s1[i].real - dc_red ; 
		s2[i].real =  s2[i].real - dc_ir ; 
	}
	
	//移动平均滤波
	UsartPrintf(USART_DEBUG,"***********8 pt Moving Average red******************************************************\r\n");
//	
	for(i = 1;i < FFT_N-1;i++) 
	{
			n_denom= ( s1[i-1].real + 2*s1[i].real + s1[i+1].real);
			s1[i].real=  n_denom/4.00; 
			
			n_denom= ( s2[i-1].real + 2*s2[i].real + s2[i+1].real);
			s2[i].real=  n_denom/4.00; 			
	}
	//八点平均滤波
	for(i = 0;i < FFT_N-8;i++) 
	{
			n_denom= ( s1[i].real+s1[i+1].real+ s1[i+2].real+ s1[i+3].real+ s1[i+4].real+ s1[i+5].real+ s1[i+6].real+ s1[i+7].real);
			s1[i].real=  n_denom/8.00; 
			
			n_denom= ( s2[i].real+s2[i+1].real+ s2[i+2].real+ s2[i+3].real+ s2[i+4].real+ s2[i+5].real+ s2[i+6].real+ s2[i+7].real);
			s2[i].real=  n_denom/8.00; 
		
			UsartPrintf(USART_DEBUG,"%f\r\n",s1[i].real);		
	}
	UsartPrintf(USART_DEBUG,"************8 pt Moving Average ir*************************************************************\r\n");
	for(i = 0;i < FFT_N;i++) 
	{
		UsartPrintf(USART_DEBUG,"%f\r\n",s2[i].real);	
	}
	UsartPrintf(USART_DEBUG,"**************************************************************************************************\r\n");
	//开始变换显示	
	g_fft_index = 0;	
	//快速傅里叶变换
	FFT(s1);
	FFT(s2);
	//解平方
	UsartPrintf(USART_DEBUG,"开始FFT算法****************************************************************************************************\r\n");
	for(i = 0;i < FFT_N;i++) 
	{
		s1[i].real=sqrtf(s1[i].real*s1[i].real+s1[i].imag*s1[i].imag);
		s1[i].real=sqrtf(s2[i].real*s2[i].real+s2[i].imag*s2[i].imag);
	}
	//计算交流分量
	for (i=1 ; i<FFT_N ; i++ ) 
	{
		ac_red += s1[i].real ;
		ac_ir +=  s2[i].real ;
	}
	
	for(i = 0;i < FFT_N/2;i++) 
	{
		UsartPrintf(USART_DEBUG,"%f\r\n",s1[i].real);
	}
	UsartPrintf(USART_DEBUG,"****************************************************************************************\r\n");
	for(i = 0;i < FFT_N/2;i++) 
	{
		UsartPrintf(USART_DEBUG,"%f\r\n",s2[i].real);
	}
	
	UsartPrintf(USART_DEBUG,"结束FFT算法***************************************************************************************************************\r\n");
//	for(i = 0;i < 50;i++) 
//	{
//		if(s1[i].real<=10)
//			break;
//	}
	
	//UsartPrintf(USART_DEBUG,"%d\r\n",(int)i);
	//读取峰值点的横坐标  结果的物理意义为 
	int s1_max_index = find_max_num_index(s1, 30);
	int s2_max_index = find_max_num_index(s2, 30);
	UsartPrintf(USART_DEBUG,"%d\r\n",s1_max_index);
	UsartPrintf(USART_DEBUG,"%d\r\n",s2_max_index);
	float Heart_Rate = 60.00 * ((100.0 * s1_max_index )/ 512.00)+20;
	
	g_blooddata.heart = Heart_Rate;
	
		float R = (ac_ir*dc_red)/(ac_red*dc_ir);
		float sp02_num =-45.060*R*R+ 30.354 *R + 94.845;
		g_blooddata.SpO2 = sp02_num;
			
}


void blood_Loop(void)
{
	//血液信息获取
	blood_data_update();
	//血液信息转换
	blood_data_translate();
	//显示血液状态信息
	g_blooddata.SpO2 = (g_blooddata.SpO2 > 99.99) ? 99.99:g_blooddata.SpO2;
//	UsartPrintf(USART_DEBUG,"指令心率%3d",g_blooddata.heart);
	OLED_ShowFloatNum(0,0,g_blooddata.heart,2,2,OLED_6X8);
//	UsartPrintf(USART_DEBUG,"指令血氧%0.2f",g_blooddata.SpO2);
	OLED_ShowFloatNum(50,0, g_blooddata.SpO2 , 2,2,OLED_6X8);
	
	
//	OLED_ShowNum(3,4,g_blooddata.SpO2,3,OLED_8X16);
	//tft显示刷新
	//LED 蜂鸣器信息更新
}

