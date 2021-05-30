#include "led.h"
#include "delay.h"
#include "key.h"
#include "sys.h"
#include "usart.h"
#include "timer.h"
#include "Infrared_Send.h"
	
//红外遥控识别码(ID),每款遥控器的该值基本都不一样,但也有一样的.
//我们选用的遥控器识别码为0
#define REMOTE_ID 0
 
 int main(void)
 {	
	u8 t=0;	
 	vu8 key=0;
	vu8 ControlValue=0;
	 
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
					ControlValue = 104;
					break;
				case KEY2_PRES:	//发送2，控制码为152
				    ControlValue = 152;
					break;
				case KEY1_PRES:	//发送测试部分，控制码为255
					ControlValue = 255;
					break;
				case KEY0_PRES:	//发送1，控制码为1
					ControlValue = 1;
					break;
			}
			
			Infrared_Send(REMOTE_ID,ControlValue);
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

