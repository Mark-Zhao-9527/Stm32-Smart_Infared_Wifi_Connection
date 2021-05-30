#include "common.h"
#include "Infrared_Send.h"


//ATK-ESP8266 WIFI AP����
//���ڲ���TCP/UDP����
//����ֵ:0,����
//    ����,�������
u8 atk_8266_wifiap_test(void)
{
	u8 netpro=0;	//����ģʽ
	u8 key;
	u8 timex=0; 
	u8 ipbuf[16]; 	//IP����
	u8 *p;
	u16 t=999;		//���ٵ�һ�λ�ȡ����״̬
	u8 res=0;
	u16 rlen=0;
	u8 constate=0;	//����״̬
	p=mymalloc(SRAMIN,32);							//����32�ֽ��ڴ�
	
PRESTA:
//	netpro=atk_8266_netpro_sel(50,30,(u8*)ATK_ESP8266_CWMODE_TBL[1]);	//ѡ������ģʽ
//  20191027 ������������Ϊֱ�Ӷ���ֵ��ʹ����AP SERVERģʽ�� netpro=0��Ϊ��ģʽ��		
	if(netpro&0X02)   //UDP
	{
		LCD_Clear(WHITE);
		POINT_COLOR=RED;
		//Show_Str_Mid(0,30,"ATK-ESP WIFI-AP ����",16,240); 
		LCD_ShowString(0,30,200,16,16,"ATK-ESP8266 WIFI test ");
		//Show_Str(30,50,200,16,"��������ATK-ESPģ��,���Ե�...",12,0);
		LCD_ShowString(30,50,200,16,16,"ATK-ESP8266 WIFI is setting,please wait! ");
		if(atk_8266_ip_set("WIFI-AP remote IP SET",(u8*)ATK_ESP8266_WORKMODE_TBL[netpro],(u8*)portnum,ipbuf))goto PRESTA;	//IP����
		sprintf((char*)p,"AT+CIPSTART=\"UDP\",\"%s\",%s",ipbuf,(u8*)portnum);    //����Ŀ��UDP������
		atk_8266_send_cmd("AT+CIPMUX=0","OK",100);  //������ģʽ
		LCD_Clear(WHITE);
		while(atk_8266_send_cmd(p,"OK",500));
			
	}
	else     //TCP
	{
		if(netpro&0X01)     //TCP Client    ͸��ģʽ����
		{
			LCD_Clear(WHITE);
			POINT_COLOR=RED;
		    //Show_Str_Mid(0,30,"ATK-ESP WIFI-AP ����",16,240); 
			LCD_ShowString(0,30,200,16,16,"ATK-ESP8266 WIFI test ");
			//Show_Str(30,50,200,16,"��������ATK-ESPģ��,���Ե�...",12,0);
			LCD_ShowString(30,50,200,16,16,"ATK-ESP8266 WIFI is setting,please wait! ");
			if(atk_8266_ip_set("WIFI-AP remote IP SET",(u8*)ATK_ESP8266_WORKMODE_TBL[netpro],(u8*)portnum,ipbuf))goto PRESTA;	//IP����
			atk_8266_send_cmd("AT+CIPMUX=0","OK",20);   //0�������ӣ�1��������
			sprintf((char*)p,"AT+CIPSTART=\"TCP\",\"%s\",%s",ipbuf,(u8*)portnum);    //����Ŀ��TCP������
			while(atk_8266_send_cmd(p,"OK",200))
			{
				LCD_Clear(WHITE);
				POINT_COLOR=RED;
				//Show_Str_Mid(0,40,"WK_UP:������ѡ",16,240);
				LCD_ShowString(0,40,200,16,16,"WK_UP:back and set again");
				//Show_Str_Mid(0,80,"ATK-ESP ����TCP Serverʧ��",12,240); //����ʧ��
				LCD_ShowString(0,80,200,16,16,"ATK-ESP link failed");				
				//key=KEY_Scan(0);
				//if(key==WKUP_PRES)goto PRESTA;
			}	
			atk_8266_send_cmd("AT+CIPMODE=1","OK",200);      //����ģʽΪ��͸��		
		}
		else					//TCP Server
		{
			LCD_Clear(WHITE);
			POINT_COLOR=RED;
			//Show_Str_Mid(0,30,"ATK-ESP WIFI-AP ����",16,240); 
			LCD_ShowString(30,30,200,16,16,"ATK-ESP8266 WIFI test ");
			//Show_Str(30,50,200,16,"��������ATK-ESPģ��,���Ե�...",12,0);
			LCD_ShowString(30,50,200,16,16,"ATK-ESP8266 WIFI is setting,please wait! ");
			atk_8266_send_cmd("AT+CIPMUX=1","OK",20);   //0�������ӣ�1��������
			sprintf((char*)p,"AT+CIPSERVER=1,%s",(u8*)portnum);
			atk_8266_send_cmd(p,"OK",20);     //����Serverģʽ���˿ں�Ϊ8086
		}
	}
	
	//Show_Str_Mid(0,30,"ATK-ESP WIFI-AP ����",16,240); 
	LCD_ShowString(30,30,200,16,16,"ATK-ESP8266 WIFI test ");
	//Show_Str(30,50,200,16,"��������ATK-ESPģ��,���Ե�...",12,0);
	LCD_ShowString(30,50,200,16,16,"ATK-ESP8266 WIFI is setting");		
	LCD_Fill(30,50,239,50+12,WHITE);			//���֮ǰ����ʾ
	//Show_Str(30,50,200,16,"����ATK-ESPģ��ɹ�!",12,0);
	LCD_ShowString(30,50,200,16,16,"ATK-ESP8266 WIFI ok! ");
	delay_ms(200);
	//Show_Str(30,50,200,16,"WK_UP:�˳�����  KEY0:��������",12,0);
	LCD_Clear(WHITE);
	POINT_COLOR=RED;
	LCD_ShowString(30,30,200,16,16,"ATK-ESP8266 WIFI Test ");
	LCD_ShowString(30,50,200,16,16,"MODE: AP_Server");
	LCD_ShowString(30,70,200,16,16,"KEY0: Send Data");
	LCD_ShowString(30,90,200,16,16,"Status:");	
	atk_8266_get_wanip(ipbuf);//������ģʽ,��ȡWAN IP
	sprintf((char*)p,"IP:%s Port:%s",ipbuf,(u8*)portnum);
	//Show_Str(30,65,200,12,p,12,0);				//��ʾIP��ַ�Ͷ˿�	
	LCD_ShowString(30,110,200,16,16,p);
	//Show_Str(30,80,200,12,"״̬:",12,0); 		    //����״̬
	//Show_Str(120,80,200,12,"ģʽ:",12,0); 		//����״̬
	//Show_Str(30,100,200,12,"��������:",12,0); 	//��������
	//Show_Str(30,115,200,12,"��������:",12,0);	//��������
	//atk_8266_wificonf_show(30,180,"�����豸����WIFI�ȵ�:",(u8*)wifiap_ssid,(u8*)wifiap_encryption,(u8*)wifiap_password);
	POINT_COLOR=BLUE;
	//Show_Str(120+30,80,200,12,(u8*)ATK_ESP8266_WORKMODE_TBL[netpro],12,0); 		//����״̬
	USART3_RX_STA=0;
	while(1)
	{
		key=KEY_Scan(0);
		if(key==WKUP_PRES)		//WK_UP �˳�����		 
		{  
			res=0;
			atk_8266_quit_trans();	//�˳�͸��
			atk_8266_send_cmd("AT+CIPMODE=0","OK",20);   //�ر�͸��ģʽ
			break;												 
		}
		else if(key==KEY0_PRES)	//KEY0 �������� 
		{
		
			if((netpro==3)||(netpro==2))   //UDP
			{
				sprintf((char*)p,"ATK-8266 %s test%02d\r\n",ATK_ESP8266_WORKMODE_TBL[netpro],t/10);//��������
				//Show_Str(30+54,100,200,12,p,12,0);
				LCD_ShowString(30+54,130,200,12,12,p);
				atk_8266_send_cmd("AT+CIPSEND=25","OK",200);  //����ָ�����ȵ�����
				delay_ms(200);
				atk_8266_send_data(p,"OK",100);  //����ָ�����ȵ�����
				timex=100;
			}
			else if((netpro==1))   //TCP Client
			{
				atk_8266_quit_trans();
				atk_8266_send_cmd("AT+CIPSEND","OK",20);       //��ʼ͸��
				sprintf((char*)p,"ATK-8266 %s test%02d\r\n",ATK_ESP8266_WORKMODE_TBL[netpro],t/10);//��������
				//Show_Str(30+54,100,200,12,p,12,0);
				LCD_ShowString(30+54,130,200,12,12,p);
				u3_printf("%s",p);
				timex=100;
			}
			else    //TCP Server
			{
				sprintf((char*)p,"Send: %s mode sendvalue %02d\r\n",ATK_ESP8266_WORKMODE_TBL[netpro],t/10);//��������
				//Show_Str(30+54,100,200,12,p,12,0);
				LCD_ShowString(30,130,200,12,12,p);
				atk_8266_send_cmd("AT+CIPSEND=0,35","OK",200);  //����ָ�����ȵ�����
				delay_ms(200);
				atk_8266_send_data(p,"OK",100);  //����ָ�����ȵ�����
				timex=255;
			}
		}
	
		if(timex)timex--;  //������������ʾһ��ʱ����Զ�����
		if(timex==1)LCD_Fill(30,130,239,142,WHITE);
		t++;
		delay_ms(5);
		
		if(USART3_RX_STA&0X8000)		//���յ�һ��������
		{ 
			rlen=USART3_RX_STA&0X7FFF;	//�õ����ν��յ������ݳ���
			USART3_RX_BUF[rlen]=0;		//��ӽ����� 
			printf("%s",USART3_RX_BUF);	//���͵�����   
			sprintf((char*)p,"Revice:%d BITS DATA:",rlen);//���յ����ֽ��� 
			LCD_Fill(30,150,239,170,WHITE);
			POINT_COLOR=BRED;
			//Show_Str(30+54,115,156,12,p,12,0); 			//��ʾ���յ������ݳ���
			LCD_ShowString(30,150,156,12,12,p);
			POINT_COLOR=BLUE;
			LCD_Fill(30,170,239,319,WHITE);
			//Show_Str(30,130,180,190,USART3_RX_BUF,12,0);//��ʾ���յ������� 
			LCD_ShowString(30,170,239,16,12,USART3_RX_BUF);	
			LCD_ShowxNum(30,190,USART3_RX_BUF[11],2,12,0);			
			//////////////////////////////////////////////////////////////////////////////////////////
			//��Ҫע����ǣ�ͨ��������8266����1��1������������ʾ�ǽ��յ�12λ���ݣ������1��ʵ��11λ��
			//2019.10.27 �Թ�����Ӳ��֣��ж��յ���1��2������1������LED1,����2���ر�LED
			switch(USART3_RX_BUF[11])  //��16������1��2��ʹ�ô����жϣ�����ʹ��ASCIIת�� 
			{
				case 1: LED1 = 0;Infrared_Send(0,104);	break;   //Infrared_Send(REMOTE_ID,ControlValue);
				case 2: LED1 = 1;Infrared_Send(0,152);	break;
				default:/*ȱʡ��*/  break;
			}	
			///////////////////////////////////////////////////////////////////////////////////////////
			USART3_RX_STA=0;
			if(constate!='+')	
				t=1000;    //״̬Ϊ��δ����,������������״̬
			else 
				t=0;       //״̬Ϊ�Ѿ�������,10����ټ��
		}  
		if(t==1000)//����10����û���յ��κ�����,��������ǲ��ǻ�����.
		{
			constate=atk_8266_consta_check();//�õ�����״̬
			if(constate=='+') 
				//Show_Str(30+30,80,200,16,"���ӳɹ�",12,0);  //����״̬
				LCD_ShowString(30+60,90,200,12,16,"8266 link success");
			else 
				//Show_Str(30+30,80,200,12,"����ʧ��",12,0); 
				LCD_ShowString(30+60,90,200,12,16,"8266 link error  ");				
			t=0;
		}
		
		//if((t%250)==0)	LED1=!LED1;  //��ʾ������������,2020.01.15 ��������ʱ�رա��ֻ�ͨ��WIFI��������ʱ������LED1ͬʱ����
		atk_8266_at_response(1);
	}
	myfree(SRAMIN,p);		//�ͷ��ڴ� 
	return res;		
} 







