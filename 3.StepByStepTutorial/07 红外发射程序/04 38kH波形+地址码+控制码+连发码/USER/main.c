#include "led.h"
#include "delay.h"
#include "key.h"
#include "sys.h"
#include "usart.h"
#include "timer.h"
#include "Infrared_Send.h"
	
//����ң��ʶ����(ID),ÿ��ң�����ĸ�ֵ��������һ��,��Ҳ��һ����.
//����ѡ�õ�ң����ʶ����Ϊ0
#define REMOTE_ID 0
 
 int main(void)
 {	
	u8 t=0;	
 	vu8 key=0;
	vu8 ControlValue=0;
	 
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
					ControlValue = 104;
					break;
				case KEY2_PRES:	//����2��������Ϊ152
				    ControlValue = 152;
					break;
				case KEY1_PRES:	//���Ͳ��Բ��֣�������Ϊ255
					ControlValue = 255;
					break;
				case KEY0_PRES:	//����1��������Ϊ1
					ControlValue = 1;
					break;
			}
			
			Infrared_Send(REMOTE_ID,ControlValue);
		}
		
		//==========================================================		
		t++;  //LED1��˸ʾ�������������
		if(t==20)
		{
			t=0;
			LED1=!LED1;
		}	   
	}	 
 }

