#ifndef __USART2_H
#define __USART2_H
#include "stdio.h"	
#include "sys.h" 
	  	
#define USART2_REC_LEN 200

void uart2_init(u32 bound);
void Usart2_Write_Bytes(USART_TypeDef* USARTx, unsigned char *pdata, int length);

#endif

