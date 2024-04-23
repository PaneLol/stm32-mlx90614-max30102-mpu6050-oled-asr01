#ifndef __DELAY_H
#define __DELAY_H 			   
#include "stm32f10x.h"
//////////////////////////////////////////////////////////////////////////////////	 
//使用SysTick的普通计数模式对延迟进行管理
//包括delay_us,delay_ms
//V1.2修改说明
//修正了中断中调用出现死循环的错误
//防止延时不准确,采用do while结构!
//////////////////////////////////////////////////////////////////////////////////	 
void Delay_init(u8 SYSCLK);
void Delay_ms(u16 nms);
void Delay_us(u32 nus);

#endif

