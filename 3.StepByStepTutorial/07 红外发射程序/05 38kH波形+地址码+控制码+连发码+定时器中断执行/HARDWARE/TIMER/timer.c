#include "timer.h"
#include "led.h"
#include "usart.h"
#include "Infrared_Send.h"

/*******************************************�ڲ�����**********************************************************************/
static u32 g_Timer2_Delay=0;
u8 g_KeyIsOn=0;   

/*********************�궨��************************/
//���⿪��״̬
#define INFRARED_SEND_STATUS_ON             0       //�����״̬
#define INFRARED_SEND_STATUS_OFF            1       //����ر�״̬

//******************************************************************************        
//name:             TIM2_Delay50us       
//introduce:        ��ʱ����ʱ50us   
//parameter:        n_Time     
//return:           none                           
//****************************************************************************** 
void TIM2_Delay50us(u32 n_Time)
{
	g_Timer2_Delay = n_Time;
	while(g_Timer2_Delay != 0);
}

//=========================================================================================================================== 	  
//ͨ�ö�ʱ��2�жϳ�ʼ��
//����ʱ��ѡ��ΪAPB1��2������APB1Ϊ36M
//arr���Զ���װֵ��
//psc��ʱ��Ԥ��Ƶ��
//����ʹ�õ��Ƕ�ʱ��3!
void TIM2_Int_Init(u16 arr,u16 psc)
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE); //ʱ��ʹ��

	TIM_TimeBaseStructure.TIM_Period = arr; //��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ	 ������50Ϊ50us
	TIM_TimeBaseStructure.TIM_Prescaler =psc; //����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ  1Mhz�ļ���Ƶ��  
	TIM_TimeBaseStructure.TIM_ClockDivision = 0; //����ʱ�ӷָ�:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM���ϼ���ģʽ
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure); //����TIM_TimeBaseInitStruct��ָ���Ĳ�����ʼ��TIMx��ʱ�������λ
 
	TIM_ITConfig(TIM2,TIM_IT_Update,ENABLE ); //ʹ��ָ����TIM2�ж�,��������ж�

	NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;  //TIM2�ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  //��ռ���ȼ�0��
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;  //�����ȼ�3��
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQͨ����ʹ��
	NVIC_Init(&NVIC_InitStructure);  //����NVIC_InitStruct��ָ���Ĳ�����ʼ������NVIC�Ĵ���

	TIM_Cmd(TIM2, ENABLE);  //ʹ��TIMx����							 
}

//��ʱ��2�жϷ������
void TIM2_IRQHandler(void)   //TIM3�ж�
{
	u8 i = 0;
	u32 nData;
	static u8 ProessControl = 0;
	
	if(TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET) //���ָ����TIM�жϷ������:TIM �ж�Դ 
	{
		TIM_ClearITPendingBit(TIM2, TIM_IT_Update  );  //���TIMx���жϴ�����λ:TIM �ж�Դ 
//		if(g_Timer2_Delay != 0X00)
//		{
//			g_Timer2_Delay--;  //ÿ�ε���50usʱ�䣬��ʱ�����Զ���1��ֱ�����굽���ʱʱ��
//		}
//���������ʹ���������
		if(g_KeyIsOn==1)
		{ 
		  switch(ProessControl)
		  {
			  case 0: nData = (u32) ( 0xFF0000 + (0x66<<8) + ((u8) ~(0x66)));
					  //��ʼ�����⣬Ĭ��Ϊ��
					  Infrared_Send_Status(INFRARED_SEND_STATUS_OFF);
					  //9ms��ʼ�źŵ͵�ƽ
		              Infrared_Send_Status(INFRARED_SEND_STATUS_ON);
			          TIM2->ARR = 9000;
			          ProessControl = 1;
					  break;
			  
			  case 1: //TIM2_Delay50us(180);  		  
					  //4.5ms��ʼ�źŸߵ�ƽ��
					  Infrared_Send_Status(INFRARED_SEND_STATUS_OFF);
			          TIM2->ARR = 4500;
			          ProessControl = 2;
					  break;

			  case 2://TIM2_Delay50us(90);    //2019.01.13 22:33 ��������ʱ��ԭ��û�аѳ���д�꣬�´δ�ʱ����д
		  
					  //ѭ������32bit������
					  for(i = 0; i < 32; i++)
					  {
						//ͨ�õ�0.56ms�͵�ƽ
						Infrared_Send_Status(INFRARED_SEND_STATUS_ON);
						TIM2_Delay50us(11);    
					  
						//��ֵ0
						if((nData & 0x80000000) == 0)
						{
						  //0.565ms�ߵ�ƽ
						  Infrared_Send_Status(INFRARED_SEND_STATUS_OFF);
						  TIM2_Delay50us(11);       
						}
						//��ֵ1
						else
						{
						  //1.69ms�ߵ�ƽ��
						  Infrared_Send_Status(INFRARED_SEND_STATUS_OFF);
						  TIM2_Delay50us(34); 
						}

						//������λ
						nData <<= 1;
					  }
					  
					  //----------------------------------------------------------------------------------
					  //���������û��̧�������ͷ�������
					  // NEC ��涨��������(�� 9ms �͵�ƽ+2.5ms �ߵ�ƽ+0.56ms �͵�ƽ+97.94ms �ߵ�ƽ���)
					  while(g_KeyIsOn==1)
					  {
						 //9ms�͵�ƽ
						 Infrared_Send_Status(INFRARED_SEND_STATUS_ON);
						 TIM2_Delay50us(180);  
					  
						 //2.5ms�ߵ�ƽ��
						Infrared_Send_Status(INFRARED_SEND_STATUS_OFF);
						TIM2_Delay50us(50); 
					  
						 //0.56ms�͵�ƽ
						 Infrared_Send_Status(INFRARED_SEND_STATUS_ON);
						 TIM2_Delay50us(11);  
					  
						 //97.94ms�ߵ�ƽ��
						Infrared_Send_Status(INFRARED_SEND_STATUS_OFF);
						TIM2_Delay50us(1899); 
					  }
					  break;
		  //----------------------------------------------------------------------------------
			  default: break;
		}
	  }
	}
}
//===========================================================================================================================
//TIM3 PWM���ֳ�ʼ�� 
//PWM�����ʼ��
//arr���Զ���װֵ
//psc��ʱ��Ԥ��Ƶ��
void TIM3_PWM_Init(u16 arr,u16 psc)
{  
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);	//ʹ�ܶ�ʱ��3ʱ��
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB  | RCC_APB2Periph_AFIO, ENABLE);  //ʹ��GPIO�����AFIO���ù���ģ��ʱ��
	
	GPIO_PinRemapConfig(GPIO_PartialRemap_TIM3, ENABLE); //Timer3������ӳ��  TIM3_CH2->PB5    
 
   //���ø�����Ϊ�����������,���TIM3 CH2��PWM���岨��	GPIOB.5
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5; //TIM_CH2
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  //�����������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);//��ʼ��GPIO
 
   //��ʼ��TIM3
	TIM_TimeBaseStructure.TIM_Period = arr; //��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ
	TIM_TimeBaseStructure.TIM_Prescaler =psc; //����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ 
	TIM_TimeBaseStructure.TIM_ClockDivision = 0; //����ʱ�ӷָ�:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM���ϼ���ģʽ
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure); //����TIM_TimeBaseInitStruct��ָ���Ĳ�����ʼ��TIMx��ʱ�������λ
	
	//��ʼ��TIM3 ChanneL_2 PWMģʽ	   //PWM1ģʽʱ���Ƚ��������Ϊ���趨�£����������趨ֵʱ�����Ϊ�ߡ���ռ�ձ�Ϊ1/3
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; //ѡ��ʱ��ģʽ:TIM�����ȵ���ģʽ1
 	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //�Ƚ����ʹ��
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; //�������:TIM����Ƚϼ��Ը�
	TIM_OC2Init(TIM3, &TIM_OCInitStructure);  //����Tָ���Ĳ�����ʼ������TIM3 OC2

	TIM_OC2PreloadConfig(TIM3, TIM_OCPreload_Enable);  //ʹ��TIM3��CCR2�ϵ�Ԥװ�ؼĴ���
 
	//TIM_Cmd(TIM3, ENABLE);  
	TIM_Cmd(TIM3, DISABLE);  //����TIM3
	
	TIM_SetCompare2(TIM3,672);//����ռ�ձ�316/947=1/3��
}
