#include "led.h"
#include "delay.h"
#include "key.h"
#include "sys.h"
#include "usart.h"
#include "timer.h"
	
 int main(void)
 {		
	delay_init();	    	 //延时函数初始化	  
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); 	 //设置NVIC中断分组2:2位抢占优先级，2位响应优先级
	uart_init(115200);	     //串口初始化为115200
 	LED_Init();			     //LED端口初始化
	 
 	TIM3_PWM_Init(947,1);	 //分频系数1，分出72/(1+1)=36MHZ。PWM频率=36000000/(947+1)=37.97Khz
	TIM_SetCompare2(TIM3,316);//设置占空比316/947=1/3。
	 
   	while(1)
	{
		LED0=!LED0;
		delay_ms(1000);	//LED0每1s翻转一次，指示程序正常运行	   
	}	 
 }

