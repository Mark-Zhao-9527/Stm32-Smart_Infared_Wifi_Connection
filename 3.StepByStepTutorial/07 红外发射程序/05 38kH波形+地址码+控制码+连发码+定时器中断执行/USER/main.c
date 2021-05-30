#include "led.h"
#include "delay.h"
#include "key.h"
#include "sys.h"
#include "usart.h"
#include "timer.h"
#include "Infrared_Send.h"

/**************************************调试数据设定**************************************************************/	
//红外遥控识别码(ID),每款遥控器的该值基本都不一样,但也有一样的.
//我们选用的遥控器识别码为0
#define REMOTE_ID 0


 
 int main(void)
 {	
	u16 t=0;	
 	//vu8 key=0;
	vu8 ControlValue=0;
	 
	extern u8 g_KeyIsOn;   //当有按键按下去时，此值为1，抬起后此值为0 
	 
	delay_init();	    	 //延时函数初始化	  
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); 	 //设置NVIC中断分组2:2位抢占优先级，2位响应优先级
	uart_init(115200);	     //串口初始化为115200
	KEY_Init();
 	LED_Init();			     //LED端口初始化
	
	TIM2_Int_Init(49,71);    //1Mhz的计数频率，计数到50为50us
	TIM3_PWM_Init(947,1);	 //分频系数1，分出72/(1+1)=36MHZ。PWM频率=36000000/(947+1)=37.97Khz
	 
   	while(1)
	{
		if(KEY0==0)
		{
			if(g_KeyIsOn==0)  //假如红外还没有发射出去
			{
				delay_ms(10);//去抖动 
				if(KEY0==0)
				{
					g_KeyIsOn=1; //示意按键被按下
					//ControlValue = 123;
					//Infrared_Send(REMOTE_ID,ControlValue);  //通过红外发射管发射红外信号
					//2020.01.13 屏蔽上面两句，使用定时器来判断红外发射
				}
			}
		}
		else
			g_KeyIsOn=0; //示意按键被抬下
		
		//=============================================================================		
		t++;  //LED1闪烁示意程序正常运行
		if(t==65535)
		{
			t=0;
			LED1=!LED1;
		}	   
	}	 
 }

