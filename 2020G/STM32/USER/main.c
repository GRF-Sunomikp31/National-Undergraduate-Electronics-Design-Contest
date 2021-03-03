#include "sys.h"

unsigned char MATA[6] = {0x69, 0x4C, 0x44, 0x3A, 0x30, 0x31 };
u8 ISM[] = {"iSM"}, ILD[] = {"iLD:1"};
u8 DATA[20], DATA1[20]; 

extern u8  USART2_RX_BUF[200]; //接收缓冲,最大USART_REC_LEN个字节.末字节为换行符 
extern u16 USART2_RX_STA;         		//接收状态标记	
extern float DIS;
extern u8 shape;
	
int main(void)
{		
	int key_val;//, PWM_val = 18600;
	int len,t;
	int i;
	int DISS_sum;

//	u8 Last_shape;
	delay_init();	    	 //延时函数初始化	  
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); //设置NVIC中断分组2:2位抢占优先级，2位响应优先级
	uart2_init(38400);	 //串口初始化为115200
	uart_init(9600);
 	LED_Init();			     //LED端口初始化
	KEY_Init();          //初始化与按键连接的硬件接口
	TIM3_PWM_Init(19999,72);
	IIC_Init();
	OLED_Init();
	OLED_Clear();
	
	delay_ms(1000);
 	while(1) 
	{
		key_val = KEY_Scan(0);
		if (key_val == 1)
		{		
			
			for (i = 0; i < 10; i ++)
			{		
				if(USART_RX_STA&0x8000)
				{					   
					len=USART_RX_STA&0x3fff;//得到此次接收到的数据长度
					for(t=0;t<len;t++)
					{
						DATA[t] = USART_RX_BUF[t];	
					}
					USART_RX_STA=0;
				}		
				
				Usart2_Write_Bytes (USART2, ISM, 3);				
				delay_ms(500);			
				
				MODO_1();	
				if (DIS < 45 && DIS > 25)
				DISS_sum += DIS;
				else
					i --;
			}
			DIS = DISS_sum / 10;
			OLED_ShowNum (30, 6, DIS, 4, 12);
			DISS_sum = 0; 
			
			for(i = 0; i < 3; i ++)
			{
				BEEP = 1;
				LED1 = 0;
				delay_ms (300);
				BEEP = 0;
				LED1 = 1;
				delay_ms (300);
			}	
			Usart2_Write_Bytes (USART2, ILD, 5);				
			delay_ms(500);				
		}
		
		if (key_val == 2)
		{
			Scan();
			MODO_2();
			for (i = 0; i < 10; i ++)
			{		

				if(USART_RX_STA&0x8000)
				{					   
					len=USART_RX_STA&0x3fff;//得到此次接收到的数据长度
					for(t=0;t<len;t++)
					{
						DATA[t] = USART_RX_BUF[t];	
					}
					USART_RX_STA=0;
				}		
					
				Usart2_Write_Bytes (USART2, ISM, 3);				
				delay_ms(500);			
					
				MODO_1();	
				if (DIS < 45 && DIS > 25)
				DISS_sum += DIS;
				else
					i --;
			}
			DIS = DISS_sum / 10;
			OLED_ShowNum (30, 6, DIS, 4, 12);
			DISS_sum = 0; 
			
			for(i = 0; i < 3; i ++)
			{
				BEEP = 1;
				LED1 = 0;
				delay_ms (300);
				BEEP = 0;
				LED1 = 1;
				delay_ms (300);
			}	
			Usart2_Write_Bytes (USART2, ILD, 5);				
		}
	}	
}


