#include "led.h"
#include "delay.h"
#include "key.h"
#include "sys.h"
#include "usart.h"
#include "timer.h"
#include "Infrared_Send.h"
	
 int main(void)
 {	
	u8 t=0;	
 	vu8 key=0;
	vu8 ControlValueP=0,ControlValueN=0;
    vu32 ControlValueSend;	 
	delay_init();	    	 //��ʱ������ʼ��	  
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); 	 //����NVIC�жϷ���2:2λ��ռ���ȼ���2λ��Ӧ���ȼ�
	uart_init(115200);	     //���ڳ�ʼ��Ϊ115200
	KEY_Init();
 	LED_Init();			     //LED�˿ڳ�ʼ��
	
	TIM2_Int_Init(49,71);    //1Mhz�ļ���Ƶ�ʣ�������50Ϊ50us
	TIM3_PWM_Init(947,1);	 //��Ƶϵ��1���ֳ�72/(1+1)=36MHZ��PWMƵ��=36000000/(947+1)=37.97Khz
	 
   	while(1)
	{
		key=KEY_Scan(0);	//�õ���ֵ
	   	if(key)
		{						   
			switch(key)
			{				 
				case WKUP_PRES:	//����1��������Ϊ104
					ControlValueP = 104;
				    ControlValueN =  (u8) ~ControlValueP;
				    ControlValueSend = (u32) ((ControlValueP<<8)+ ControlValueN);
				    LED1=!LED1;
					break;
				case KEY2_PRES:	//����2��������Ϊ152
				    ControlValueSend = 39015;
				    LED1=!LED1;
					break;
				case KEY1_PRES:	//���Ͳ��Բ��֣�������Ϊ255
					ControlValueSend=65280;
					LED1=!LED1;
					break;
				case KEY0_PRES:	//����0��������Ϊ0
					ControlValueSend=255;
					LED1=!LED1;
					break;
			}
			
			ControlValueSend = ControlValueSend + 0xFF0000;
			Infrared_Send(ControlValueSend);
		}
		
		//==========================================================		
		t++;
		if(t==20)
		{
			t=0;
			LED1=!LED1;
		}	   
	}	 
 }

