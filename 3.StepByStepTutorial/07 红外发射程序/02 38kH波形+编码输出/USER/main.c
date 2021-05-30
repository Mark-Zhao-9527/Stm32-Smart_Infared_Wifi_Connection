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
	delay_init();	    	 //延时函数初始化	  
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); 	 //设置NVIC中断分组2:2位抢占优先级，2位响应优先级
	uart_init(115200);	     //串口初始化为115200
	KEY_Init();
 	LED_Init();			     //LED端口初始化
	
	TIM2_Int_Init(49,71);    //1Mhz的计数频率，计数到50为50us
	TIM3_PWM_Init(947,1);	 //分频系数1，分出72/(1+1)=36MHZ。PWM频率=36000000/(947+1)=37.97Khz
	 
   	while(1)
	{
		key=KEY_Scan(0);	//得到键值
	   	if(key)
		{						   
			switch(key)
			{				 
				case WKUP_PRES:	//发送1，控制码为104
					ControlValueP = 104;
				    ControlValueN =  (u8) ~ControlValueP;
				    ControlValueSend = (u32) ((ControlValueP<<8)+ ControlValueN);
				    LED1=!LED1;
					break;
				case KEY2_PRES:	//发送2，控制码为152
				    ControlValueSend = 39015;
				    LED1=!LED1;
					break;
				case KEY1_PRES:	//发送测试部分，控制码为255
					ControlValueSend=65280;
					LED1=!LED1;
					break;
				case KEY0_PRES:	//发送0，控制码为0
					ControlValueSend=255;
					LED1=!LED1;
					break;
			}
			
			ControlValueSend = ControlValueSend+0xFF0000;
			Infrared_Send(ControlValueSend);
		}
		
		//==========================================================		
		t++;  //LED1闪烁示意程序正常运行
		if(t==20)
		{
			t=0;
			LED1=!LED1;
		}		   
	}	 
 }

