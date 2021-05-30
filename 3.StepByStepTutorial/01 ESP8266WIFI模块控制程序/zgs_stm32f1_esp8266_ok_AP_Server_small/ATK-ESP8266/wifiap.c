#include "common.h"
/////////////////////////////////////////////////////////////////////////////////////////////////////////// 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//ALIENTEK STM32开发板
//ATK-ESP8266 WIFI模块 WIFI AP驱动代码	   
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//修改日期:2014/4/3
//版本：V1.0
//版权所有，盗版必究。
//Copyright(C) 广州市星翼电子科技有限公司 2009-2019
//All rights reserved									  
/////////////////////////////////////////////////////////////////////////////////////////////////////////// 


//ATK-ESP8266 WIFI AP测试
//用于测试TCP/UDP连接
//返回值:0,正常
//    其他,错误代码
u8 atk_8266_wifiap_test(void)
{
	u8 netpro=0;	//网络模式
	u8 key;
	u8 timex=0; 
	u8 ipbuf[16]; 	//IP缓存
	u8 *p;
	u16 t=999;		//加速第一次获取链接状态
	u8 res=0;
	u16 rlen=0;
	u8 constate=0;	//连接状态
	p=mymalloc(SRAMIN,32);							//申请32字节内存
	
PRESTA:
//	netpro=atk_8266_netpro_sel(50,30,(u8*)ATK_ESP8266_CWMODE_TBL[1]);	//选择网络模式
//  20191027 屏蔽上条，改为直接定义值，使进入AP SERVER模式。 netpro=0即为此模式。		
	if(netpro&0X02)   //UDP
	{
		LCD_Clear(WHITE);
		POINT_COLOR=RED;
		//Show_Str_Mid(0,30,"ATK-ESP WIFI-AP 测试",16,240); 
		LCD_ShowString(0,30,200,16,16,"ATK-ESP8266 WIFI test ");
		//Show_Str(30,50,200,16,"正在配置ATK-ESP模块,请稍等...",12,0);
		LCD_ShowString(30,50,200,16,16,"ATK-ESP8266 WIFI is setting,please wait! ");
		if(atk_8266_ip_set("WIFI-AP remote IP SET",(u8*)ATK_ESP8266_WORKMODE_TBL[netpro],(u8*)portnum,ipbuf))goto PRESTA;	//IP输入
		sprintf((char*)p,"AT+CIPSTART=\"UDP\",\"%s\",%s",ipbuf,(u8*)portnum);    //配置目标UDP服务器
		atk_8266_send_cmd("AT+CIPMUX=0","OK",100);  //单链接模式
		LCD_Clear(WHITE);
		while(atk_8266_send_cmd(p,"OK",500));
			
	}
	else     //TCP
	{
		if(netpro&0X01)     //TCP Client    透传模式测试
		{
			LCD_Clear(WHITE);
			POINT_COLOR=RED;
		    //Show_Str_Mid(0,30,"ATK-ESP WIFI-AP 测试",16,240); 
			LCD_ShowString(0,30,200,16,16,"ATK-ESP8266 WIFI test ");
			//Show_Str(30,50,200,16,"正在配置ATK-ESP模块,请稍等...",12,0);
			LCD_ShowString(30,50,200,16,16,"ATK-ESP8266 WIFI is setting,please wait! ");
			if(atk_8266_ip_set("WIFI-AP remote IP SET",(u8*)ATK_ESP8266_WORKMODE_TBL[netpro],(u8*)portnum,ipbuf))goto PRESTA;	//IP输入
			atk_8266_send_cmd("AT+CIPMUX=0","OK",20);   //0：单连接，1：多连接
			sprintf((char*)p,"AT+CIPSTART=\"TCP\",\"%s\",%s",ipbuf,(u8*)portnum);    //配置目标TCP服务器
			while(atk_8266_send_cmd(p,"OK",200))
			{
				LCD_Clear(WHITE);
				POINT_COLOR=RED;
				//Show_Str_Mid(0,40,"WK_UP:返回重选",16,240);
				LCD_ShowString(0,40,200,16,16,"WK_UP:back and set again");
				//Show_Str_Mid(0,80,"ATK-ESP 连接TCP Server失败",12,240); //连接失败
				LCD_ShowString(0,80,200,16,16,"ATK-ESP link failed");				
				//key=KEY_Scan(0);
				//if(key==WKUP_PRES)goto PRESTA;
			}	
			atk_8266_send_cmd("AT+CIPMODE=1","OK",200);      //传输模式为：透传		
		}
		else					//TCP Server
		{
			LCD_Clear(WHITE);
			POINT_COLOR=RED;
			//Show_Str_Mid(0,30,"ATK-ESP WIFI-AP 测试",16,240); 
			LCD_ShowString(30,30,200,16,16,"ATK-ESP8266 WIFI test ");
			//Show_Str(30,50,200,16,"正在配置ATK-ESP模块,请稍等...",12,0);
			LCD_ShowString(30,50,200,16,16,"ATK-ESP8266 WIFI is setting,please wait! ");
			atk_8266_send_cmd("AT+CIPMUX=1","OK",20);   //0：单连接，1：多连接
			sprintf((char*)p,"AT+CIPSERVER=1,%s",(u8*)portnum);
			atk_8266_send_cmd(p,"OK",20);     //开启Server模式，端口号为8086
		}
	}
	
	//Show_Str_Mid(0,30,"ATK-ESP WIFI-AP 测试",16,240); 
	LCD_ShowString(30,30,200,16,16,"ATK-ESP8266 WIFI test ");
	//Show_Str(30,50,200,16,"正在配置ATK-ESP模块,请稍等...",12,0);
	LCD_ShowString(30,50,200,16,16,"ATK-ESP8266 WIFI is setting");		
	LCD_Fill(30,50,239,50+12,WHITE);			//清除之前的显示
	//Show_Str(30,50,200,16,"配置ATK-ESP模块成功!",12,0);
	LCD_ShowString(30,50,200,16,16,"ATK-ESP8266 WIFI ok! ");
	delay_ms(200);
	//Show_Str(30,50,200,16,"WK_UP:退出测试  KEY0:发送数据",12,0);
	LCD_Clear(WHITE);
	POINT_COLOR=RED;
	LCD_ShowString(30,30,200,16,16,"ATK-ESP8266 WIFI Test ");
	LCD_ShowString(30,50,200,16,16,"MODE: AP_Server");
	LCD_ShowString(30,70,200,16,16,"KEY0: Send Data");
	LCD_ShowString(30,90,200,16,16,"Status:");	
	atk_8266_get_wanip(ipbuf);//服务器模式,获取WAN IP
	sprintf((char*)p,"IP:%s Port:%s",ipbuf,(u8*)portnum);
	//Show_Str(30,65,200,12,p,12,0);				//显示IP地址和端口	
	LCD_ShowString(30,110,200,16,16,p);
	//Show_Str(30,80,200,12,"状态:",12,0); 		    //连接状态
	//Show_Str(120,80,200,12,"模式:",12,0); 		//连接状态
	//Show_Str(30,100,200,12,"发送数据:",12,0); 	//发送数据
	//Show_Str(30,115,200,12,"接收数据:",12,0);	//接收数据
	//atk_8266_wificonf_show(30,180,"请用设备连接WIFI热点:",(u8*)wifiap_ssid,(u8*)wifiap_encryption,(u8*)wifiap_password);
	POINT_COLOR=BLUE;
	//Show_Str(120+30,80,200,12,(u8*)ATK_ESP8266_WORKMODE_TBL[netpro],12,0); 		//连接状态
	USART3_RX_STA=0;
	while(1)
	{
		key=KEY_Scan(0);
		if(key==WKUP_PRES)		//WK_UP 退出测试		 
		{  
			res=0;
			atk_8266_quit_trans();	//退出透传
			atk_8266_send_cmd("AT+CIPMODE=0","OK",20);   //关闭透传模式
			break;												 
		}
		else if(key==KEY0_PRES)	//KEY0 发送数据 
		{
		
			if((netpro==3)||(netpro==2))   //UDP
			{
				sprintf((char*)p,"ATK-8266 %s test%02d\r\n",ATK_ESP8266_WORKMODE_TBL[netpro],t/10);//测试数据
				//Show_Str(30+54,100,200,12,p,12,0);
				LCD_ShowString(30+54,130,200,12,12,p);
				atk_8266_send_cmd("AT+CIPSEND=25","OK",200);  //发送指定长度的数据
				delay_ms(200);
				atk_8266_send_data(p,"OK",100);  //发送指定长度的数据
				timex=100;
			}
			else if((netpro==1))   //TCP Client
			{
				atk_8266_quit_trans();
				atk_8266_send_cmd("AT+CIPSEND","OK",20);       //开始透传
				sprintf((char*)p,"ATK-8266 %s test%02d\r\n",ATK_ESP8266_WORKMODE_TBL[netpro],t/10);//测试数据
				//Show_Str(30+54,100,200,12,p,12,0);
				LCD_ShowString(30+54,130,200,12,12,p);
				u3_printf("%s",p);
				timex=100;
			}
			else    //TCP Server
			{
				sprintf((char*)p,"Send: %s mode sendvalue %02d\r\n",ATK_ESP8266_WORKMODE_TBL[netpro],t/10);//测试数据
				//Show_Str(30+54,100,200,12,p,12,0);
				LCD_ShowString(30,130,200,12,12,p);
				atk_8266_send_cmd("AT+CIPSEND=0,35","OK",200);  //发送指定长度的数据
				delay_ms(200);
				atk_8266_send_data(p,"OK",100);  //发送指定长度的数据
				timex=255;
			}
		}
	
		if(timex)timex--;  //发送数据区显示一定时间后自动清理
		if(timex==1)LCD_Fill(30,130,239,142,WHITE);
		t++;
		delay_ms(5);
		
		if(USART3_RX_STA&0X8000)		//接收到一次数据了
		{ 
			rlen=USART3_RX_STA&0X7FFF;	//得到本次接收到的数据长度
			USART3_RX_BUF[rlen]=0;		//添加结束符 
			printf("%s",USART3_RX_BUF);	//发送到串口   
			sprintf((char*)p,"Revice:%d BITS DATA:",rlen);//接收到的字节数 
			LCD_Fill(30,150,239,170,WHITE);
			POINT_COLOR=BRED;
			//Show_Str(30+54,115,156,12,p,12,0); 			//显示接收到的数据长度
			LCD_ShowString(30,150,156,12,12,p);
			POINT_COLOR=BLUE;
			LCD_Fill(30,170,239,319,WHITE);
			//Show_Str(30,130,180,190,USART3_RX_BUF,12,0);//显示接收到的数据 
			LCD_ShowString(30,170,239,16,12,USART3_RX_BUF);	
			LCD_ShowxNum(30,190,USART3_RX_BUF[11],2,12,0);			
			//////////////////////////////////////////////////////////////////////////////////////////
			//需要注意的是，通过网络往8266发送1个1，接收数据显示是接收到12位数据，而这个1其实在11位。
			//2019.10.27 赵贵生添加部分，判断收到是1还2，若是1，点亮LED1,若是2，关闭LED
			switch(USART3_RX_BUF[11])  //以16进制数1，2，使用此条判断，否则使用ASCII转化 
			{
				case 1: LED1 = 0;	break;  
				case 2: LED1 = 1;	break;
				default:/*缺省项*/  break;
			}	
			///////////////////////////////////////////////////////////////////////////////////////////
			USART3_RX_STA=0;
			if(constate!='+')	
				t=1000;    //状态为还未连接,立即更新连接状态
			else 
				t=0;       //状态为已经连接了,10秒后再检查
		}  
		if(t==1000)//连续10秒钟没有收到任何数据,检查连接是不是还存在.
		{
			constate=atk_8266_consta_check();//得到连接状态
			if(constate=='+') 
				//Show_Str(30+30,80,200,16,"连接成功",12,0);  //连接状态
				LCD_ShowString(30+60,90,200,12,16,"8266 link success");
			else 
				//Show_Str(30+30,80,200,12,"连接失败",12,0); 
				LCD_ShowString(30+60,90,200,12,16,"8266 link error  ");				
			t=0;
		}
		
		if((t%100)==0)LED0=!LED0;  //显示程序正常运行
		atk_8266_at_response(1);
	}
	myfree(SRAMIN,p);		//释放内存 
	return res;		
} 







