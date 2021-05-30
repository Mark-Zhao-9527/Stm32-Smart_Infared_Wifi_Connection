#include "led.h"
#include "delay.h"
#include "key.h"
#include "sys.h"
#include "usart.h"
#include "timer.h"
	
 int main(void)
 {		
	delay_init();	    	 //��ʱ������ʼ��	  
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); 	 //����NVIC�жϷ���2:2λ��ռ���ȼ���2λ��Ӧ���ȼ�
	uart_init(115200);	     //���ڳ�ʼ��Ϊ115200
 	LED_Init();			     //LED�˿ڳ�ʼ��
	 
 	TIM3_PWM_Init(947,1);	 //��Ƶϵ��1���ֳ�72/(1+1)=36MHZ��PWMƵ��=36000000/(947+1)=37.97Khz
	TIM_SetCompare2(TIM3,316);//����ռ�ձ�316/947=1/3��
	 
   	while(1)
	{
		LED0=!LED0;
		delay_ms(1000);	//LED0ÿ1s��תһ�Σ�ָʾ������������	   
	}	 
 }

