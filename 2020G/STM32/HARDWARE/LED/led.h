#ifndef __LED_H
#define __LED_H	 
#include "sys.h"

#define LED0 PCout(13)// PC13  程序指示灯
#define LED1 PAout(4)// PA4  声光指示灯
#define BEEP PAout(5)//PA5  蜂鸣器

void LED_Init(void);//初始化
		 				    
#endif
