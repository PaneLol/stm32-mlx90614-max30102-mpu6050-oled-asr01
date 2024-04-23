//#include "Usart.h"
// 
//void usat2_init(void)
//{
//	GPIO_InitTypeDef uart;
//	USART_InitTypeDef uart1;
//	NVIC_InitTypeDef uart2;
// 
//	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);//使能相应时钟
//	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);	//串口时钟
// 
//	
//	//tx
//	uart.GPIO_Mode=GPIO_Mode_AF_PP;//复用推挽输出
//	uart.GPIO_Pin=GPIO_Pin_9;//输出引脚
//	uart.GPIO_Speed=GPIO_Speed_10MHz;
//	GPIO_Init(GPIOA,&uart);
// 
//	
//	//rx
//	uart.GPIO_Mode=GPIO_Mode_IN_FLOATING;//浮空输入
//	uart.GPIO_Pin=GPIO_Pin_10;
//	uart.GPIO_Speed=GPIO_Speed_10MHz;
//	GPIO_Init(GPIOA,&uart);//模式设置
//	
//	uart1.USART_BaudRate=9600;//比特率
//	uart1.USART_HardwareFlowControl=USART_HardwareFlowControl_None;//不使用yingjianliu
//	uart1.USART_Mode=USART_Mode_Rx|USART_Mode_Tx;//使能发送与接收
//	uart1.USART_Parity=USART_Parity_No;//奇偶校验
//	uart1.USART_StopBits=USART_StopBits_1;//停止位
//	uart1.USART_WordLength=USART_WordLength_8b;//字长		
//	USART_Init(USART1,&uart1);//初始化串口，并进行配置
//	
//	
//	USART_Cmd(USART1,ENABLE);//使能串口
//	//以上为串口
//	
//	//之后为中断
//	USART_ITConfig(USART1,USART_IT_RXNE,ENABLE);//（中断使能函数）开启接收中断；串口端选择，方式，使能{EA}
//	
//	uart2.NVIC_IRQChannel=USART1_IRQn;
//	uart2.NVIC_IRQChannelCmd=ENABLE;
//	uart2.NVIC_IRQChannelPreemptionPriority=1;//抢占优先级
//	uart2.NVIC_IRQChannelSubPriority=1;//响应优先级
//	NVIC_Init(&uart2);//NVIC中断优先级
//	
//}
// 
//void USART1_IRQ(void)//中断服务函数
//{
//	u8 res;
//	if(USART_GetITStatus(USART1,USART_IT_RXNE))
//	{
//		res=USART_ReceiveData(USART1);//接收
//		USART_SendData(USART1,res);//发送
//	}
//}