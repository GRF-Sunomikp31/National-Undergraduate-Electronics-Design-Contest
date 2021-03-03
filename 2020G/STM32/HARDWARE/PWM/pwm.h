#ifndef __PWM_H
#define __PWM_H

#include "sys.h"

#define PWMA TIM3 -> CCR2  // 5  xiaduoji 
#define PWMB TIM3 -> CCR3  //  0  shangduoji 

#define PWMA_Origin     18515
#define PWMB_Origin     18700

void TIM3_PWM_Init (u16 arr,u16 psc);

#endif

