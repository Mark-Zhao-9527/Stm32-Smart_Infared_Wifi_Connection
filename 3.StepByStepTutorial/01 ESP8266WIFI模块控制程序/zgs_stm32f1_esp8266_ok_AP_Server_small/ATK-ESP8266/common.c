#include "common.h"
/////////////////////////////////////////////////////////////////////////////////////////////////////////// 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEK STM32������
//ATK-ESP8266 WIFIģ�� ������������	   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//�޸�����:2014/4/3
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2009-2019
//All rights reserved									  
/////////////////////////////////////////////////////////////////////////////////////////////////////////// 

/////////////////////////////////////////////////////////////////////////////////////////////////////////// 
//�û�������

//���Ӷ˿ں�:8086,�������޸�Ϊ�����˿�.
const u8* portnum="8086";		 

//WIFI STAģʽ,����Ҫȥ���ӵ�·�������߲���,��������Լ���·��������,�����޸�.
const u8* wifista_ssid="ALIENTEK";			//·����SSID��
const u8* wifista_encryption="wpawpa2_aes";	//wpa/wpa2 aes���ܷ�ʽ
const u8* wifista_password="13436698145"; 	//��������

//WIFI APģʽ,ģ���������߲���,�������޸�.
const u8* wifiap_ssid="ZHAO-ESP8266";			//����SSID��
const u8* wifiap_encryption="wpawpa2_aes";	    //wpa/wpa2 aes���ܷ�ʽ
const u8* wifiap_password="13436698145"; 		//�������� 

/////////////////////////////////////////////////////////////////////////////////////////////////////////// 
//4������ģʽ
const u8 *ATK_ESP8266_CWMODE_TBL[3]={"STA Mode","AP Mode","AP&STA Mode"};	//ATK-ESP8266,3������ģʽ,Ĭ��Ϊ·����(ROUTER)ģʽ 
//4�ֹ���ģʽ
const u8 *ATK_ESP8266_WORKMODE_TBL[3]={"TCP Server","TCP Client"," UDP Mode"};	//ATK-ESP8266,4�ֹ���ģʽ
//5�ּ��ܷ�ʽ
const u8 *ATK_ESP8266_ECN_TBL[5]={"OPEN","WEP","WPA_PSK","WPA2_PSK","WPA_WAP2_PSK"};
/////////////////////////////////////////////////////////////////////////////////////////////////////////// 

//usmart֧�ֲ���
//���յ���ATָ��Ӧ�����ݷ��ظ����Դ���
//mode:0,������USART3_RX_STA;
//     1,����USART3_RX_STA;
void atk_8266_at_response(u8 mode)
{
	if(USART3_RX_STA&0X8000)		//���յ�һ��������
	{ 
		USART3_RX_BUF[USART3_RX_STA&0X7FFF]=0;//��ӽ�����
		printf("%s",USART3_RX_BUF);	//���͵�����
		if(mode)USART3_RX_STA=0;
	} 
}
//ATK-ESP8266���������,�����յ���Ӧ��
//str:�ڴ���Ӧ����
//����ֵ:0,û�еõ��ڴ���Ӧ����
//    ����,�ڴ�Ӧ������λ��(str��λ��)
u8* atk_8266_check_cmd(u8 *str)
{
	char *strx=0;
	if(USART3_RX_STA&0X8000)		//���յ�һ��������
	{ 
		USART3_RX_BUF[USART3_RX_STA&0X7FFF]=0;//��ӽ�����
		strx=strstr((const char*)USART3_RX_BUF,(const char*)str);
	} 
	return (u8*)strx;
}
//��ATK-ESP8266��������
//cmd:���͵������ַ���
//ack:�ڴ���Ӧ����,���Ϊ��,���ʾ����Ҫ�ȴ�Ӧ��
//waittime:�ȴ�ʱ��(��λ:10ms)
//����ֵ:0,���ͳɹ�(�õ����ڴ���Ӧ����)
//       1,����ʧ��
u8 atk_8266_send_cmd(u8 *cmd,u8 *ack,u16 waittime)
{
	u8 res=0; 
	USART3_RX_STA=0;
	u3_printf("%s\r\n",cmd);	//��������
	if(ack&&waittime)		//��Ҫ�ȴ�Ӧ��
	{
		while(--waittime)	//�ȴ�����ʱ
		{
			delay_ms(10);
			if(USART3_RX_STA&0X8000)//���յ��ڴ���Ӧ����
			{
				if(atk_8266_check_cmd(ack))
				{
					printf("ack:%s\r\n",(u8*)ack);
					break;//�õ���Ч���� 
				}
					USART3_RX_STA=0;
			} 
		}
		if(waittime==0)res=1; 
	}
	return res;
} 
//��ATK-ESP8266����ָ������
//data:���͵�����(����Ҫ��ӻس���)
//ack:�ڴ���Ӧ����,���Ϊ��,���ʾ����Ҫ�ȴ�Ӧ��
//waittime:�ȴ�ʱ��(��λ:10ms)
//����ֵ:0,���ͳɹ�(�õ����ڴ���Ӧ����)luojian
u8 atk_8266_send_data(u8 *data,u8 *ack,u16 waittime)
{
	u8 res=0; 
	USART3_RX_STA=0;
	u3_printf("%s",data);	//��������
	if(ack&&waittime)		//��Ҫ�ȴ�Ӧ��
	{
		while(--waittime)	//�ȴ�����ʱ
		{
			delay_ms(10);
			if(USART3_RX_STA&0X8000)//���յ��ڴ���Ӧ����
			{
				if(atk_8266_check_cmd(ack))break;//�õ���Ч���� 
				USART3_RX_STA=0;
			} 
		}
		if(waittime==0)res=1; 
	}
	return res;
}
//ATK-ESP8266�˳�͸��ģʽ
//����ֵ:0,�˳��ɹ�;
//       1,�˳�ʧ��
u8 atk_8266_quit_trans(void)
{
	while((USART3->SR&0X40)==0);	//�ȴ����Ϳ�
	USART3->DR='+';      
	delay_ms(15);					//���ڴ�����֡ʱ��(10ms)
	while((USART3->SR&0X40)==0);	//�ȴ����Ϳ�
	USART3->DR='+';      
	delay_ms(15);					//���ڴ�����֡ʱ��(10ms)
	while((USART3->SR&0X40)==0);	//�ȴ����Ϳ�
	USART3->DR='+';      
	delay_ms(500);					//�ȴ�500ms
	return atk_8266_send_cmd("AT","OK",20);//�˳�͸���ж�.
}
//��ȡATK-ESP8266ģ���AP+STA����״̬
//����ֵ:0��δ����;1,���ӳɹ�
u8 atk_8266_apsta_check(void)
{
	if(atk_8266_quit_trans())return 0;			//�˳�͸�� 
	atk_8266_send_cmd("AT+CIPSTATUS",":",50);	//����AT+CIPSTATUSָ��,��ѯ����״̬
	if(atk_8266_check_cmd("+CIPSTATUS:0")&&
		 atk_8266_check_cmd("+CIPSTATUS:1")&&
		 atk_8266_check_cmd("+CIPSTATUS:2")&&
		 atk_8266_check_cmd("+CIPSTATUS:4"))
		return 0;
	else return 1;
}
//��ȡATK-ESP8266ģ�������״̬
//����ֵ:0,δ����;1,���ӳɹ�.
u8 atk_8266_consta_check(void)
{
	u8 *p;
	u8 res;
	if(atk_8266_quit_trans())return 0;			//�˳�͸�� 
	atk_8266_send_cmd("AT+CIPSTATUS",":",50);	//����AT+CIPSTATUSָ��,��ѯ����״̬
	p=atk_8266_check_cmd("+CIPSTATUS:"); 
	res=*p;									//�õ�����״̬	
	return res;
}

//��ȡClient ip��ַ
//ipbuf:ip��ַ���������
void atk_8266_get_wanip(u8* ipbuf)
{
	u8 *p,*p1;
		if(atk_8266_send_cmd("AT+CIFSR","OK",50))//��ȡWAN IP��ַʧ��
		{
			ipbuf[0]=0;
			return;
		}		
		p=atk_8266_check_cmd("\"");
		p1=(u8*)strstr((const char*)(p+1),"\"");
		*p1=0;
		sprintf((char*)ipbuf,"%s",p+1);	
}

//��ȡAP+STA ip��ַ����ָ��λ����ʾ
//ipbuf:ip��ַ���������
void atk_8266_get_ip(u8 x,u8 y)
{
		u8 *p;
		u8 *p1;
		u8 *p2;
		u8 *ipbuf;
		u8 *buf;
		p=mymalloc(SRAMIN,32);							//����32�ֽ��ڴ�
		p1=mymalloc(SRAMIN,32);							//����32�ֽ��ڴ�
		p2=mymalloc(SRAMIN,32);							//����32�ֽ��ڴ�
	  ipbuf=mymalloc(SRAMIN,32);							//����32�ֽ��ڴ�
		buf=mymalloc(SRAMIN,32);							//����32�ֽ��ڴ�
		if(atk_8266_send_cmd("AT+CIFSR","OK",50))//��ȡWAN IP��ַʧ��
		{ 
			*ipbuf=0;
		}
		else
		{		
			p=atk_8266_check_cmd("APIP,\"");
			p1=(u8*)strstr((const char*)(p+6),"\"");
			p2=p1;
			*p1=0;
			ipbuf=p+6;
			sprintf((char*)buf,"AP IP:%s �˿�:%s",ipbuf,(u8*)portnum);
			//Show_Str(x,y,200,12,buf,12,0);				//��ʾAPģʽ��IP��ַ�Ͷ˿�
			p=(u8*)strstr((const char*)(p2+1),"STAIP,\"");
			p1=(u8*)strstr((const char*)(p+7),"\"");
			*p1=0;
			ipbuf=p+7;
			sprintf((char*)buf,"STA IP:%s �˿�:%s",ipbuf,(u8*)portnum);
			//Show_Str(x,y+15,200,12,buf,12,0);				//��ʾSTAģʽ��IP��ַ�Ͷ˿�
			myfree(SRAMIN,p);		//�ͷ��ڴ�
			myfree(SRAMIN,p1);		//�ͷ��ڴ�
			myfree(SRAMIN,p2);		//�ͷ��ڴ�
			myfree(SRAMIN,ipbuf);		//�ͷ��ڴ�
			myfree(SRAMIN,buf);		//�ͷ��ڴ�
		}
}

//ATK-ESP8266ģ����Ϣ��ʾ
//x,y:��ʾ��Ϣ����ʼ����.
//wanip:0,ȫ��������ʾ;1,������wanip.
void atk_8266_msg_show(u16 x,u16 y,u8 wanip)
{
	u8 *p,*p1,*p2;
	p=mymalloc(SRAMIN,32);							//����32�ֽ��ڴ�
	p1=mymalloc(SRAMIN,32);							//����32�ֽ��ڴ�
	p2=mymalloc(SRAMIN,32);							//����32�ֽ��ڴ�
	POINT_COLOR=BLUE;
	atk_8266_send_cmd("AT+CWMODE=2","OK",20);
	atk_8266_send_cmd("AT+RST","OK",20);
	delay_ms(1000);//��ʱ2s�ȴ�ģ������
	delay_ms(1000);//
	delay_ms(1000);
	delay_ms(1000);
	sprintf((char*)p,"AT+CWSAP=\"%s\",\"%s\",1,4",wifiap_ssid,wifiap_password);    //����ģ��APģʽ���߲���
	atk_8266_send_cmd(p,"OK",1000);
	if(wanip==0)//ȫ����
	{
		atk_8266_send_cmd("AT+GMR","OK",20);		//��ȡ�̼��汾��
		p=atk_8266_check_cmd("SDK version:");
		//Show_Str(x,y,240,16,"�̼��汾:",16,0);Show_Str(x+72,y,240,16,p,16,0);
		atk_8266_send_cmd("AT+CWMODE?","+CWMODE:",20);	//��ȡ����ģʽ
		p=atk_8266_check_cmd(":");
		//Show_Str(x,y+16,240,16,"����ģʽ:",16,0);Show_Str(x+72,y+16,240,16,(u8*)ATK_ESP8266_CWMODE_TBL[*(p+1)-'1'],16,0);
  	    atk_8266_send_cmd("AT+CWSAP?","+CWSAP:",20);	//��ȡwifi����
		p=atk_8266_check_cmd("\"");
		p1=(u8*)strstr((const char*)(p+1),"\"");
		p2=p1;
		*p1=0;
		//Show_Str(x,y+32,240,16,"SSID��:",16,0);Show_Str(x+56,y+32,240,16,p+1,16,0);
		p=(u8*)strstr((const char*)(p2+1),"\"");
		p1=(u8*)strstr((const char*)(p+1),"\"");
		p2=p1;
		*p1=0;		
		//Show_Str(x,y+48,240,16,"����:",16,0);Show_Str(x+40,y+48,240,16,p+1,16,0);
		p=(u8*)strstr((const char*)(p2+1),",");
		p1=(u8*)strstr((const char*)(p+1),",");
		*p1=0;
		//Show_Str(x,y+64,240,16,"ͨ����:",16,0);Show_Str(x+56,y+64,240,16,p+1,16,0);
		//Show_Str(x,y+80,240,16,"���ܷ�ʽ:",16,0);Show_Str(x+72,y+80,240,16,(u8*)ATK_ESP8266_ECN_TBL[*(p1+1)-'0'],16,0);
	}
	myfree(SRAMIN,p);		//�ͷ��ڴ� 
	myfree(SRAMIN,p1);		//�ͷ��ڴ� 
	myfree(SRAMIN,p2);		//�ͷ��ڴ� 
}
//ATK-ESP8266ģ��WIFI���ò�����ʾ(��WIFI STA/WIFI APģʽ����ʱʹ��)
//x,y:��ʾ��Ϣ����ʼ����.
//rmd:��ʾ��Ϣ
//ssid,encryption,password:���������SSID,���ܷ�ʽ,����
void atk_8266_wificonf_show(u16 x,u16 y,u8* rmd,u8* ssid,u8* encryption,u8* password)
{ 
	POINT_COLOR=RED;
	//Show_Str(x,y,240,12,rmd,12,0);//��ʾ��ʾ��Ϣ 
	//Show_Str(x,y+20,240,12,"SSID:",12,0);
	//Show_Str(x,y+36,240,12,"���ܷ�ʽ:",12,0);
	//Show_Str(x,y+52,240,12,"����:",12,0); 
	POINT_COLOR=BLUE;
	//Show_Str(x+30,y+20,240,12,ssid,12,0);
	//Show_Str(x+54,y+36,240,12,encryption,12,0);
	//Show_Str(x+30,y+52,240,12,password,12,0); 	  
}
//����ģʽѡ��
//����ֵ:
//0,TCP������
//1,TCP�ͻ���
//2,UDPģʽ
u8 atk_8266_netpro_sel(u16 x,u16 y,u8* name) 
{
	u8 key=0,t=0,*p;
	u8 netpro=0;
	LCD_Clear(WHITE);
	POINT_COLOR=RED;
	p=mymalloc(SRAMIN,50);//����50���ֽڵ��ڴ�
	sprintf((char*)p,"%s ����ģʽѡ��",name);
	//Show_Str_Mid(0,y,p,16,240); 				    	 
	//Show_Str(x,y+30,200,16,"KEY0:��һ��",16,0); 				    	 
	//Show_Str(x,y+50,200,16,"KEY1:��һ��",16,0);				    	 
	//Show_Str(x,y+70,200,16,"WK_UP:ȷ��",16,0); 
	//Show_Str(x,y+100,200,16,"��ѡ��:",16,0); 
	POINT_COLOR=BLUE;
 	//Show_Str(x+16,y+120,200,16,"TCP������",16,0); 				    	 
	//Show_Str(x+16,y+140,200,16,"TCP�ͻ���",16,0);				    	 
	//Show_Str(x+16,y+160,200,16,"UDPģʽ",16,0); 
	POINT_COLOR=RED;
	//Show_Str(x,y+120,200,16,"��",16,0); 
	while(1)
	{
		key=KEY_Scan(0); 
		if(key)
		{
			if(key==WKUP_PRES)break;       //WK_UP����
			//Show_Str(x,y+120+netpro*20,200,16,"  ",16,0);//���֮ǰ����ʾ
			if(key==KEY0_PRES)//KEY0����
			{
				if(netpro<2)netpro++;
				else netpro=0;
			}else if(key==KEY1_PRES)//KEY1����
			{
				if(netpro>0)netpro--;
				else netpro=2; 
			}
			//Show_Str(x,y+120+netpro*20,200,16,"��",16,0);//ָ������Ŀ
				
		} 
		delay_ms(10);
		atk_8266_at_response(1);
		if((t++)==20){t=0;LED0=!LED0;}//LED��˸
	} 
	myfree(SRAMIN,p);
	return netpro;//��������ģʽѡ��ֵ 
} 



//IP����
//title:ip���ñ���
//mode:����ģʽ
//port:�˿ں�
//*ip:ip������(����IP���ϲ㺯��)
//����ֵ:0,ȷ������;1,ȡ������.
u8 atk_8266_ip_set(u8* title,u8* mode,u8* port,u8* ip) 
{
	u8 res=0;
// 	u8 key;
//	u8 timex=0;  
//	u8 iplen=0;			//IP���� 
	LCD_Clear(WHITE);  
	POINT_COLOR=RED; 
	//Show_Str_Mid(0,30,title,16,240);		//��ʾ����	
    LCD_ShowString(0,30,200,16,16,title);	
	//Show_Str(30,90,200,16,"����ģʽ:",16,0);	//����ģʽ��ʾ
	LCD_ShowString(30,90,200,16,16,"WorkMode");
	//Show_Str(30,110,200,16,"IP��ַ:",16,0);	//IP��ַ���Լ�������
	LCD_ShowString(30,110,200,16,16,"IP");
	//Show_Str(30,130,200,16,"�˿�:",16,0);	//�˿ں�
	LCD_ShowString(30,130,200,16,16,"Port");
	//kbd_fn_tbl[0]="����";
	//kbd_fn_tbl[1]="����"; 
	//atk_8266_load_keyboard(0,180);			//��ʾ���� 
	POINT_COLOR=BLUE;
 	//Show_Str(30+72,90,200,16,mode,16,0);	//��ʾ����ģʽ	
	LCD_ShowString(30+72,90,200,16,16,mode);
 	//Show_Str(30+40,130,200,16,port,16,0);	//��ʾ�˿� 	
	LCD_ShowString(30+40,130,200,16,16,port);
	ip[0]=0; 		
//	while(1)
//	{ 
//		key=atk_8266_get_keynum(0,180);
//		if(key)
//		{   
//			if(key<12)
//			{
//				if(iplen<15)
//				{ 
//					ip[iplen++]=kbd_tbl[key-1][0];
//				}
//			}else
//			{
//				if(key==13)if(iplen)iplen--;	//ɾ��  
//				if(key==14&&iplen)break; 		//ȷ������ 
//				if(key==15){res=1;break;}		//ȡ������
//			} 
//			ip[iplen]=0; 
//			LCD_Fill(30+56,110,239,110+16,WHITE);
//			Show_Str(30+56,110,200,16,ip,16,0);			//��ʾIP��ַ 	
//		} 
//		timex++;
//		if(timex==20)
//		{
//			timex=0;
//			LED0=!LED0;
//		}
//		delay_ms(10);
//		atk_8266_at_response(1);//WIFIģ�鷢����������,��ʱ�ϴ�������
//	} 
	return res;
}



















































