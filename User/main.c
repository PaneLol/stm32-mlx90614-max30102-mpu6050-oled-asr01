#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "OLED.h"
#include "mlx90614.h"
#include <stdio.h>
#include "max30102.h"
#include "blood.h"
#include "algorithm.h"
#include "xiic.h"
#include "MPU6050.h"

uint8_t ID;								//定义用于存放ID号的变量
int16_t AX, AY, AZ, GX, GY, GZ;			//定义用于存放各个数据的变量

int main(void)
{
		int i=0;

		Delay_init(72);			//设置外部晶振和单片机时钟频率为72m
    MLX90614_IO_Init();
    OLED_Init();
		MPU6050_Init();		//MPU6050初始化	
	

//		OLED_ShowString(0, 0, "Temperature:",OLED_6X8);
//		OLED_Update();
//		Delay_ms(3000);
	
//		OLED_ShowString(1, 1, "ID:",OLED_6X8);		//显示静态字符串
//		ID = MPU6050_GetID();				//获取MPU6050的ID号
//		OLED_ShowHexNum(1, 30, ID, 2,OLED_6X8);		//OLED显示ID号
	
	
	
	
//		OLED_ShowString(0,20 , "heart:",OLED_6X8);
//		OLED_ShowString(0,40 , "blood:",OLED_6X8);
//		OLED_Update();

		IIC_GPIO_INIT();
		MAX30102_GPIO();
		Max30102_reset();
		MAX30102_Config();
	
//		for(i =0;i<128;i++)
//		{
//				while(MAX30102_INTPin_Read()==0)
//				{
//					max30102_read_fifo();
//				}
//		}
		
    while (1) 
		{

		
//				// 读取温度值
//        float temperature = MLX90614_read_value();


//        // 在屏幕上显示温度信息
//        OLED_ShowFloatNum(70, 0, temperature, 3, 2, OLED_6X8);
//				blood_Loop();

		MPU6050_GetData(&AX, &AY, &AZ, &GX, &GY, &GZ);		//获取MPU6050的数据
		OLED_ShowNum(50,50,AX,5,OLED_6X8);
			
				OLED_Update();
			

    }
}
