#include "modo.h"
#include "string.h"
#include "math.h"
#include "oled.h"

extern u8 DATA[], DATA1[];

u8 DISS[6];
static int len, flag = 1;

int x, y,distance, t, x_dis, y_dis;
float DIS, S;
u8 shape[10], color[10];
u8 SHAPE[6][10] = {"Round", "Square", "Triangle", "Basketball","Volleyball","Football"};
u8 COLOR[3][5] = {"RED","GREEN","BLUE"};

float SPRTODATA(u8 *p) 
{
    u16 i=0,j=0,t=0;
    double temp=0;
    for(i=2;i<8;i++)
    {   if(p[i]=='m') break;
        if(p[i]=='.') 
        {
            t=1;  
            j=i;  
        }
        else
        {
            if(t) 
            {
                temp=(double)(p[i]-'0')/pow(10,(i-j))+temp; 
            }
            else 
            {
                temp=(double)(p[i]-'0')+temp*10; 
            }
        }
    }
    return temp;
}


void MODO_1(void)
{
	
	OLED_Clear();
	
	//形状
	if(DATA[4] == 1)
		OLED_ShowString(20, 0, "Round", 12);
	if(DATA[4] == 2)
		OLED_ShowString(20, 0, "Triangle", 12);
	if(DATA[4] == 3)
		OLED_ShowString(20, 0, "Square", 12);
	
	//颜色
	OLED_ShowString(0, 2, "color:", 12);
	if(DATA[5
		] == 1)
		OLED_ShowString(60, 2, "Red", 12);
	if(DATA[5] == 2)
		OLED_ShowString(60, 2, "Green", 12);
	if(DATA[5] == 3)
		OLED_ShowString(60, 2, "Blue", 12);
	
	//距离
	OLED_ShowString(0, 4, "DIS:", 12);
	strlcpy (DISS, DATA1 + 2, 7);
	OLED_ShowString(35,4, DISS, 12);
	
	//尺寸
	OLED_ShowString(0, 6, "Size:", 12);
	distance = DATA[6];
	S = SPRTODATA(DATA1);
	DIS = distance * S / 2.533 ;
	OLED_ShowNum (30, 6, DIS, 4, 12);
	
}

void MODO_2(void)
{
	while (flag != 5)
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
		x = DATA[2];
		y = DATA[3];
		x_dis = x - 80;
		y_dis = 60 - y;

		PWMA = PWMA + (x - 80) * 0.1;
		delay_ms (100);
		PWMB = PWMB + (y - 60) * 0.1;	
		delay_ms (100);
		
		if (x_dis < 5 && y_dis < 5)
			flag = 5;
	}	
}

void Scan (void)
{
	flag = 1;
//	PWMA = PWMA_Origin + 300;
	while (flag != 0)
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
			
		if (flag == 1)
		{
			PWMA = PWMA - 10;
			delay_ms (100);
		}
		if(PWMA <= 18240)
		{
			flag = 2;
		}
		if (flag == 2)
		{
			PWMA = PWMA + 10;
			delay_ms (100);
		}
		if (PWMA > 18780)
		{
			flag = 1;
		}
		
		if (DATA[4] != 0x00)
			flag = 0;
	}
}


