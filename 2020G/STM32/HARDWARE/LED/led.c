#include "led.h"

void LED_Init(void)
{
 
 GPIO_InitTypeDef  GPIO_InitStructure;
 	
 RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC|RCC_APB2Periph_GPIOA, ENABLE);	 //ʹ��PB,PA�˿�ʱ��
	
 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //�������
 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO���ٶ�Ϊ50MHz
 
 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;				 //LED0-->PC.13 �˿�����	
 GPIO_Init(GPIOC, &GPIO_InitStructure);					 //�����趨������ʼ��GPIOC.13
 GPIO_SetBits(GPIOC,GPIO_Pin_13);						    //PC.13 �����

 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;	    		 //LED1-->PA.4 �˿�����, �������
 GPIO_Init(GPIOA, &GPIO_InitStructure);	  				 //������� ��IO���ٶ�Ϊ50MHz
 GPIO_SetBits(GPIOA,GPIO_Pin_4); 						  //PA.4 �����
	
 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;	    		 //BEEP-->PA.5 �˿�����, �������
 GPIO_Init(GPIOA, &GPIO_InitStructure);	  				 //������� ��IO���ٶ�Ϊ50MHz
 GPIO_ResetBits(GPIOA,GPIO_Pin_5); 						 //PA.5 ����� 
}
 
