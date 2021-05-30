#ifndef __TIMER_H
#define __TIMER_H
#include "sys.h"

/*******************************************全局变量*************************************************************/
extern u8 g_KeyIsOn;   //当有按键按下去时，此值为1，抬起后此值为0

void TIM2_Delay50us(u32 n_Time);
void TIM2_Int_Init(u16 arr,u16 psc);
void TIM3_PWM_Init(u16 arr,u16 psc);
#endif
