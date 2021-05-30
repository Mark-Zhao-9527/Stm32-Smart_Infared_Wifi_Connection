#include "led.h"
#include "delay.h"
#include "key.h"
#include "sys.h"
#include "usart.h"
#include "timer.h"
#include "Infrared_Send.h"

/**************************************���������趨**************************************************************/	
//����ң��ʶ����(ID),ÿ��ң�����ĸ�ֵ��������һ��,��Ҳ��һ����.
//����ѡ�õ�ң����ʶ����Ϊ0
#define REMOTE_ID 0


 
 int main(void)
 {	
	u16 t=0;	
 	//vu8 key=0;
	vu8 ControlValue=0;
	 
	extern u8 g_KeyIsOn;   //���а�������ȥʱ����ֵΪ1��̧����ֵΪ0 
	 
	delay_init();	    	 //��ʱ������ʼ��	  
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); 	 //����NVIC�жϷ���2:2λ��ռ���ȼ���2λ��Ӧ���ȼ�
	uart_init(115200);	     //���ڳ�ʼ��Ϊ115200
	KEY_Init();
 	LED_Init();			     //LED�˿ڳ�ʼ��
	
	TIM2_Int_Init(49,71);    //1Mhz�ļ���Ƶ�ʣ�������50Ϊ50us
	TIM3_PWM_Init(947,1);	 //��Ƶϵ��1���ֳ�72/(1+1)=36MHZ��PWMƵ��=36000000/(947+1)=37.97Khz
	 
   	while(1)
	{
		if(KEY0==0)
		{
			if(g_KeyIsOn==0)  //������⻹û�з����ȥ
			{
				delay_ms(10);//ȥ���� 
				if(KEY0==0)
				{
					g_KeyIsOn=1; //ʾ�ⰴ��������
					//ControlValue = 123;
					//Infrared_Send(REMOTE_ID,ControlValue);  //ͨ�����ⷢ��ܷ�������ź�
					//2020.01.13 �����������䣬ʹ�ö�ʱ�����жϺ��ⷢ��
				}
			}
		}
		else
			g_KeyIsOn=0; //ʾ�ⰴ����̧��
		
		//=============================================================================		
		t++;  //LED1��˸ʾ�������������
		if(t==65535)
		{
			t=0;
			LED1=!LED1;
		}	   
	}	 
 }

