//#include "Usart.h"
// 
//void usat2_init(void)
//{
//	GPIO_InitTypeDef uart;
//	USART_InitTypeDef uart1;
//	NVIC_InitTypeDef uart2;
// 
//	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);//ʹ����Ӧʱ��
//	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);	//����ʱ��
// 
//	
//	//tx
//	uart.GPIO_Mode=GPIO_Mode_AF_PP;//�����������
//	uart.GPIO_Pin=GPIO_Pin_9;//�������
//	uart.GPIO_Speed=GPIO_Speed_10MHz;
//	GPIO_Init(GPIOA,&uart);
// 
//	
//	//rx
//	uart.GPIO_Mode=GPIO_Mode_IN_FLOATING;//��������
//	uart.GPIO_Pin=GPIO_Pin_10;
//	uart.GPIO_Speed=GPIO_Speed_10MHz;
//	GPIO_Init(GPIOA,&uart);//ģʽ����
//	
//	uart1.USART_BaudRate=9600;//������
//	uart1.USART_HardwareFlowControl=USART_HardwareFlowControl_None;//��ʹ��yingjianliu
//	uart1.USART_Mode=USART_Mode_Rx|USART_Mode_Tx;//ʹ�ܷ��������
//	uart1.USART_Parity=USART_Parity_No;//��żУ��
//	uart1.USART_StopBits=USART_StopBits_1;//ֹͣλ
//	uart1.USART_WordLength=USART_WordLength_8b;//�ֳ�		
//	USART_Init(USART1,&uart1);//��ʼ�����ڣ�����������
//	
//	
//	USART_Cmd(USART1,ENABLE);//ʹ�ܴ���
//	//����Ϊ����
//	
//	//֮��Ϊ�ж�
//	USART_ITConfig(USART1,USART_IT_RXNE,ENABLE);//���ж�ʹ�ܺ��������������жϣ����ڶ�ѡ�񣬷�ʽ��ʹ��{EA}
//	
//	uart2.NVIC_IRQChannel=USART1_IRQn;
//	uart2.NVIC_IRQChannelCmd=ENABLE;
//	uart2.NVIC_IRQChannelPreemptionPriority=1;//��ռ���ȼ�
//	uart2.NVIC_IRQChannelSubPriority=1;//��Ӧ���ȼ�
//	NVIC_Init(&uart2);//NVIC�ж����ȼ�
//	
//}
// 
//void USART1_IRQ(void)//�жϷ�����
//{
//	u8 res;
//	if(USART_GetITStatus(USART1,USART_IT_RXNE))
//	{
//		res=USART_ReceiveData(USART1);//����
//		USART_SendData(USART1,res);//����
//	}
//}