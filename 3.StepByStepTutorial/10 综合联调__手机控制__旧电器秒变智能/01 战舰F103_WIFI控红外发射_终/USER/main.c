#include "sys.h"
#include "delay.h"
#include "usart.h" 
#include "led.h" 		 	 
#include "lcd.h"  
#include "key.h"     
#include "malloc.h"		
#include "usart3.h"
#include "common.h" 
#include "timer.h"
 
/************************************************
基于战舰V3的ESP8266的AP服务器程序。实现通过网络控制电路板亮灭。
1）精减，无SD卡，无25Q64存储，无汉字显示的8266作为服务器端口程序。
   精减后方便移植到小板运行。
2)  8266生成一个无线信号，提供账号名及密码。
3）开机就建立连接，不需要再按什么按钮。
4）收到1，LED1亮，收到2，LED2灭。
************************************************/

 int main(void)
 {	 	
	u8 timex;	
    u8 *p;	 

	delay_init();	    	 //延时函数初始化	  
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//设置中断优先级分组为组2：2位抢占优先级，2位响应优先级
	uart_init(115200);	 	    //串口初始化为115200		
 	LED_Init();		  			//初始化与LED连接的硬件接口
	KEY_Init();					//初始化按键
	LCD_Init();			   		//初始化LCD   
	usart3_init(115200);		//初始化串口3 
 	my_mem_init(SRAMIN);		//初始化内部内存池

	POINT_COLOR=RED;
	LCD_ShowString(20,30,200,16,16,"ATK-ESP8266 Is Setting...");
	while(atk_8266_send_cmd("AT","OK",20))//检查WIFI模块是否在线
	{
		atk_8266_quit_trans();//退出透传
		atk_8266_send_cmd("AT+CIPMODE=0","OK",200);  //关闭透传模式	
		LCD_ShowString(40,55,200,16,16,"ATK-ESP8266 Error!");
		delay_ms(800);
		LCD_Fill(40,55,200,55+16,WHITE);
		LCD_ShowString(40,55,200,16,16,"Find ATK-ESP8266 again..."); 
	} 
	while(atk_8266_send_cmd("ATE0","OK",20));//关闭回显
		
	atk_8266_send_cmd("AT+CWMODE=2","OK",20);//AT+CWMODE=2,设为AP模式
	atk_8266_send_cmd("AT+RST","OK",20);     //AT+RST,重启
	delay_ms(1000);//延时2s等待模块重启
	delay_ms(1000);
	delay_ms(1000);
	sprintf((char*)p,"AT+CWSAP=\"%s\",\"%s\",1,4",wifiap_ssid,wifiap_password);    //配置模块AP模式无线参数
	atk_8266_send_cmd(p,"OK",1000);
	
	TIM2_Int_Init(49,71);    //1Mhz的计数频率，计数到50为50us
	TIM3_PWM_Init(947,1);	 //分频系数1，分出72/(1+1)=36MHZ。PWM频率=36000000/(947+1)=37.97Khz
		
	while(1)
	{
		delay_ms(10); 
		atk_8266_at_response(1);//检查ATK-ESP8266模块发送过来的数据,及时上传给电脑
		atk_8266_wifiap_test();	//WIFI AP测试
	 		
		//以下显示程序正常运行
		if((timex%20)==0)LED1=!LED1;//200ms闪烁 
		timex++;	 
	} 			
}


















