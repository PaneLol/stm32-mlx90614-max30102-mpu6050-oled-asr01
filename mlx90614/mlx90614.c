#include "mlx90614.h"

u8 value_H,value_L,PEC;


/***************管脚的初始化*************************
*MLX90614_SDA;PA2    开漏输出8
*MLX90614_SCL;PB9     推挽输出
***************************************************/
void MLX90614_IO_Init()
{
	//打开GPIOB时钟
	RCC->APB2ENR |= (0x1 << 3);
	RCC->APB2ENR |= (0x1 << 2);
	

	GPIOB->CRH &= 0xffffff0f;
	GPIOB->CRH |= 0x00000030;	
	
	/*PA2 开漏输出*/	
	GPIOA->CRL &= ~(0xf<<8);
	GPIOA->CRL |= 0x7<<8;
	
	MLX90614_SDA_out=1;
	MLX90614_SCL=1;
}


/*******发起始位*********/
void MLX90614_SDA_Start()
{	
	MLX90614_SDA_out = 1;
	Delay_us(1);
	MLX90614_SCL = 1;
	Delay_us(5);
	MLX90614_SDA_out = 0;
	Delay_us(10);
	MLX90614_SCL = 0;	
	Delay_us(4);
}

/*********发结束位********/
void MLX90614_SDA_Stop()
{
	MLX90614_SCL = 0;
	Delay_us(5);
	MLX90614_SDA_out = 0;
	Delay_us(5);
	MLX90614_SCL = 1;
	Delay_us(10);
	MLX90614_SDA_out = 1;
	
}


/*发送一个位*/
void MLX90614_SDA_send_bit(u8 bit_out)
{	
	if(bit_out == 1)
		MLX90614_SDA_out = 1;
	else
		MLX90614_SDA_out = 0;
	Delay_us(5);
	MLX90614_SCL = 1;
	Delay_us(10);
	MLX90614_SCL = 0;	
	Delay_us(4);
}



/***  功 能 ：发送一个字节   ***
 ***  参 数 ：发送的一个字节 ***
 ***  返回值：应答位         ***/
u8 MLX90614_SDA_send(u8 dat)
{
	u8 i = 0,bit_out = 0;
	
	for(i=0;i<8;i++)
	{	  
		if(dat & 0x80)   //是从最高位开始传
			bit_out=1;    
		else
			bit_out=0;
		MLX90614_SDA_send_bit(bit_out); 
		dat <<= 1;		
	}
	if(MLX90614_SDA_receive_bit())
		return 1;
	else
		return 0;
}



/**接收一个位**/ 
u8 MLX90614_SDA_receive_bit()
{	
	u8 bit_in = 0;

	MLX90614_SDA_out = 1;
	MLX90614_SCL = 1;
	Delay_us(10);	
	
	if(MLX90614_SDA_in)
		bit_in = 1;
	else
		bit_in = 0;
	
	MLX90614_SCL = 0;
	Delay_us(4);
	
	return bit_in;
}


/**接收一个字节**/
u8 MLX90614_SDA_receive()
{
	u8 i=0,MLX90614_dat=0;
	
	for(i=0;i<8;i++)
	{
		MLX90614_dat <<= 1;
		if(MLX90614_SDA_receive_bit())
			MLX90614_dat |= 0x01;
	}
	MLX90614_SDA_send_bit(0);
	return MLX90614_dat;
}


	
/*读取温度值*/
/*
1、起始
2、发送要Slave address Tobj1 007h （物体温度）
3、读 读应答
4、发送要执行的命令（选择读RAM）
5、发送起始位
6、读寄存器
7、读PEC数据
*/

float MLX90614_read_value()
{
	u16 MLX90614_value = 0;
  float MLX90614_t = 0;
	
	MLX90614_SDA_Start();	
  //Slave address 单个MLX90614时地址为0x00  地址+wr	
	if(MLX90614_SDA_send(0x00) == 1)    //有应答  
		return 0;
	//发送命令
	if(MLX90614_SDA_send(0x07) == 1)
		return 0;
	//-----------
		
	MLX90614_SDA_Start();    //Sr
		
	if(MLX90614_SDA_send(0x01) == 1)
		return 0;
		
	//读Tobj1低八位并发送应答
	value_L = MLX90614_SDA_receive();
			
	//读Tobj1高八位并发送应答  
	value_H = MLX90614_SDA_receive();

		
	//读PEC出错数据包并发送应答 
	PEC = MLX90614_SDA_receive();
		
	MLX90614_SDA_Stop();	
	
	/*数据的处理*/	
	MLX90614_value = value_H;
  MLX90614_value	<<= 8;
	MLX90614_value |= value_L;
	MLX90614_t=MLX90614_value*0.02-272.3;                                                                         5;
	
	return MLX90614_t;	
}


