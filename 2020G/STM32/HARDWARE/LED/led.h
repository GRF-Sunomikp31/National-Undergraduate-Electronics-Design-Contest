#ifndef __LED_H
#define __LED_H	 
#include "sys.h"

#define LED0 PCout(13)// PC13  ����ָʾ��
#define LED1 PAout(4)// PA4  ����ָʾ��
#define BEEP PAout(5)//PA5  ������

void LED_Init(void);//��ʼ��
		 				    
#endif
