#include "sys.h"
#include "delay.h"
#include "usart.h" 
#include "led.h" 		 	 
#include "lcd.h"  
#include "key.h"     
#include "malloc.h"		
#include "usart3.h"
#include "common.h" 
 
/************************************************
����ս��V3��ESP8266��AP����������ʵ��ͨ��������Ƶ�·������
1����������SD������25Q64�洢���޺�����ʾ��8266��Ϊ�������˿ڳ���
   �����󷽱���ֲ��С�����С�
2)  8266����һ�������źţ��ṩ�˺��������롣
3�������ͽ������ӣ�����Ҫ�ٰ�ʲô��ť��
4���յ�1��LED1�����յ�2��LED2��
************************************************/

 int main(void)
 {	 	
	u8 timex;	
    u8 *p;	 

	delay_init();	    	 //��ʱ������ʼ��	  
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//�����ж����ȼ�����Ϊ��2��2λ��ռ���ȼ���2λ��Ӧ���ȼ�
	uart_init(115200);	 	    //���ڳ�ʼ��Ϊ115200		
 	LED_Init();		  			//��ʼ����LED���ӵ�Ӳ���ӿ�
	KEY_Init();					//��ʼ������
	LCD_Init();			   		//��ʼ��LCD   
	usart3_init(115200);		//��ʼ������3 
 	my_mem_init(SRAMIN);		//��ʼ���ڲ��ڴ��

	POINT_COLOR=RED;
	LCD_ShowString(20,30,200,16,16,"ATK-ESP8266 Is Setting...");
	while(atk_8266_send_cmd("AT","OK",20))//���WIFIģ���Ƿ�����
	{
		atk_8266_quit_trans();//�˳�͸��
		atk_8266_send_cmd("AT+CIPMODE=0","OK",200);  //�ر�͸��ģʽ	
		LCD_ShowString(40,55,200,16,16,"ATK-ESP8266 Error!");
		delay_ms(800);
		LCD_Fill(40,55,200,55+16,WHITE);
		LCD_ShowString(40,55,200,16,16,"Find ATK-ESP8266 again..."); 
	} 
	while(atk_8266_send_cmd("ATE0","OK",20));//�رջ���
		
	atk_8266_send_cmd("AT+CWMODE=2","OK",20);//AT+CWMODE=2,��ΪAPģʽ
	atk_8266_send_cmd("AT+RST","OK",20);     //AT+RST,����
	delay_ms(1000);//��ʱ2s�ȴ�ģ������
	delay_ms(1000);
	delay_ms(1000);
	sprintf((char*)p,"AT+CWSAP=\"%s\",\"%s\",1,4",wifiap_ssid,wifiap_password);    //����ģ��APģʽ���߲���
	atk_8266_send_cmd(p,"OK",1000);
		
	while(1)
	{
		delay_ms(10); 
		atk_8266_at_response(1);//���ATK-ESP8266ģ�鷢�͹���������,��ʱ�ϴ�������
		atk_8266_wifiap_test();	//WIFI AP����
	 		
		//������ʾ������������
		if((timex%20)==0)LED0=!LED0;//200ms��˸ 
		timex++;	 
	} 			
}


















