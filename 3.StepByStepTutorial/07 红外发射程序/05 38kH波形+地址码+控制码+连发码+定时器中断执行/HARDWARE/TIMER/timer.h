#ifndef __TIMER_H
#define __TIMER_H
#include "sys.h"

/*******************************************ȫ�ֱ���*************************************************************/
extern u8 g_KeyIsOn;   //���а�������ȥʱ����ֵΪ1��̧����ֵΪ0

void TIM2_Delay50us(u32 n_Time);
void TIM2_Int_Init(u16 arr,u16 psc);
void TIM3_PWM_Init(u16 arr,u16 psc);
#endif
