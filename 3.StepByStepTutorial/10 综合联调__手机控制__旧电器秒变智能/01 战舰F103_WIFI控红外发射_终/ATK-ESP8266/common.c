#include "common.h"
/////////////////////////////////////////////////////////////////////////////////////////////////////////// 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//ALIENTEK STM32开发板
//ATK-ESP8266 WIFI模块 公用驱动代码	   
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//修改日期:2014/4/3
//版本：V1.0
//版权所有，盗版必究。
//Copyright(C) 广州市星翼电子科技有限公司 2009-2019
//All rights reserved									  
/////////////////////////////////////////////////////////////////////////////////////////////////////////// 

/////////////////////////////////////////////////////////////////////////////////////////////////////////// 
//用户配置区

//连接端口号:8086,可自行修改为其他端口.
const u8* portnum="8086";		 

//WIFI STA模式,设置要去连接的路由器无线参数,请根据你自己的路由器设置,自行修改.
const u8* wifista_ssid="ALIENTEK";			//路由器SSID号
const u8* wifista_encryption="wpawpa2_aes";	//wpa/wpa2 aes加密方式
const u8* wifista_password="13436698145"; 	//连接密码

//WIFI AP模式,模块对外的无线参数,可自行修改.
const u8* wifiap_ssid="ZHAO-ESP8266";			//对外SSID号
const u8* wifiap_encryption="wpawpa2_aes";	    //wpa/wpa2 aes加密方式
const u8* wifiap_password="13436698145"; 		//连接密码 

/////////////////////////////////////////////////////////////////////////////////////////////////////////// 
//4个网络模式
const u8 *ATK_ESP8266_CWMODE_TBL[3]={"STA Mode","AP Mode","AP&STA Mode"};	//ATK-ESP8266,3种网络模式,默认为路由器(ROUTER)模式 
//4种工作模式
const u8 *ATK_ESP8266_WORKMODE_TBL[3]={"TCP Server","TCP Client"," UDP Mode"};	//ATK-ESP8266,4种工作模式
//5种加密方式
const u8 *ATK_ESP8266_ECN_TBL[5]={"OPEN","WEP","WPA_PSK","WPA2_PSK","WPA_WAP2_PSK"};
/////////////////////////////////////////////////////////////////////////////////////////////////////////// 

//usmart支持部分
//将收到的AT指令应答数据返回给电脑串口
//mode:0,不清零USART3_RX_STA;
//     1,清零USART3_RX_STA;
void atk_8266_at_response(u8 mode)
{
	if(USART3_RX_STA&0X8000)		//接收到一次数据了
	{ 
		USART3_RX_BUF[USART3_RX_STA&0X7FFF]=0;//添加结束符
		printf("%s",USART3_RX_BUF);	//发送到串口
		if(mode)USART3_RX_STA=0;
	} 
}
//ATK-ESP8266发送命令后,检测接收到的应答
//str:期待的应答结果
//返回值:0,没有得到期待的应答结果
//    其他,期待应答结果的位置(str的位置)
u8* atk_8266_check_cmd(u8 *str)
{
	char *strx=0;
	if(USART3_RX_STA&0X8000)		//接收到一次数据了
	{ 
		USART3_RX_BUF[USART3_RX_STA&0X7FFF]=0;//添加结束符
		strx=strstr((const char*)USART3_RX_BUF,(const char*)str);
	} 
	return (u8*)strx;
}
//向ATK-ESP8266发送命令
//cmd:发送的命令字符串
//ack:期待的应答结果,如果为空,则表示不需要等待应答
//waittime:等待时间(单位:10ms)
//返回值:0,发送成功(得到了期待的应答结果)
//       1,发送失败
u8 atk_8266_send_cmd(u8 *cmd,u8 *ack,u16 waittime)
{
	u8 res=0; 
	USART3_RX_STA=0;
	u3_printf("%s\r\n",cmd);	//发送命令
	if(ack&&waittime)		//需要等待应答
	{
		while(--waittime)	//等待倒计时
		{
			delay_ms(10);
			if(USART3_RX_STA&0X8000)//接收到期待的应答结果
			{
				if(atk_8266_check_cmd(ack))
				{
					printf("ack:%s\r\n",(u8*)ack);
					break;//得到有效数据 
				}
					USART3_RX_STA=0;
			} 
		}
		if(waittime==0)res=1; 
	}
	return res;
} 
//向ATK-ESP8266发送指定数据
//data:发送的数据(不需要添加回车了)
//ack:期待的应答结果,如果为空,则表示不需要等待应答
//waittime:等待时间(单位:10ms)
//返回值:0,发送成功(得到了期待的应答结果)luojian
u8 atk_8266_send_data(u8 *data,u8 *ack,u16 waittime)
{
	u8 res=0; 
	USART3_RX_STA=0;
	u3_printf("%s",data);	//发送命令
	if(ack&&waittime)		//需要等待应答
	{
		while(--waittime)	//等待倒计时
		{
			delay_ms(10);
			if(USART3_RX_STA&0X8000)//接收到期待的应答结果
			{
				if(atk_8266_check_cmd(ack))break;//得到有效数据 
				USART3_RX_STA=0;
			} 
		}
		if(waittime==0)res=1; 
	}
	return res;
}
//ATK-ESP8266退出透传模式
//返回值:0,退出成功;
//       1,退出失败
u8 atk_8266_quit_trans(void)
{
	while((USART3->SR&0X40)==0);	//等待发送空
	USART3->DR='+';      
	delay_ms(15);					//大于串口组帧时间(10ms)
	while((USART3->SR&0X40)==0);	//等待发送空
	USART3->DR='+';      
	delay_ms(15);					//大于串口组帧时间(10ms)
	while((USART3->SR&0X40)==0);	//等待发送空
	USART3->DR='+';      
	delay_ms(500);					//等待500ms
	return atk_8266_send_cmd("AT","OK",20);//退出透传判断.
}
//获取ATK-ESP8266模块的AP+STA连接状态
//返回值:0，未连接;1,连接成功
u8 atk_8266_apsta_check(void)
{
	if(atk_8266_quit_trans())return 0;			//退出透传 
	atk_8266_send_cmd("AT+CIPSTATUS",":",50);	//发送AT+CIPSTATUS指令,查询连接状态
	if(atk_8266_check_cmd("+CIPSTATUS:0")&&
		 atk_8266_check_cmd("+CIPSTATUS:1")&&
		 atk_8266_check_cmd("+CIPSTATUS:2")&&
		 atk_8266_check_cmd("+CIPSTATUS:4"))
		return 0;
	else return 1;
}
//获取ATK-ESP8266模块的连接状态
//返回值:0,未连接;1,连接成功.
u8 atk_8266_consta_check(void)
{
	u8 *p;
	u8 res;
	if(atk_8266_quit_trans())return 0;			//退出透传 
	atk_8266_send_cmd("AT+CIPSTATUS",":",50);	//发送AT+CIPSTATUS指令,查询连接状态
	p=atk_8266_check_cmd("+CIPSTATUS:"); 
	res=*p;									//得到连接状态	
	return res;
}

//获取Client ip地址
//ipbuf:ip地址输出缓存区
void atk_8266_get_wanip(u8* ipbuf)
{
	u8 *p,*p1;
		if(atk_8266_send_cmd("AT+CIFSR","OK",50))//获取WAN IP地址失败
		{
			ipbuf[0]=0;
			return;
		}		
		p=atk_8266_check_cmd("\"");
		p1=(u8*)strstr((const char*)(p+1),"\"");
		*p1=0;
		sprintf((char*)ipbuf,"%s",p+1);	
}

//获取AP+STA ip地址并在指定位置显示
//ipbuf:ip地址输出缓存区
void atk_8266_get_ip(u8 x,u8 y)
{
		u8 *p;
		u8 *p1;
		u8 *p2;
		u8 *ipbuf;
		u8 *buf;
		p=mymalloc(SRAMIN,32);							//申请32字节内存
		p1=mymalloc(SRAMIN,32);							//申请32字节内存
		p2=mymalloc(SRAMIN,32);							//申请32字节内存
	  ipbuf=mymalloc(SRAMIN,32);							//申请32字节内存
		buf=mymalloc(SRAMIN,32);							//申请32字节内存
		if(atk_8266_send_cmd("AT+CIFSR","OK",50))//获取WAN IP地址失败
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
			sprintf((char*)buf,"AP IP:%s 端口:%s",ipbuf,(u8*)portnum);
			//Show_Str(x,y,200,12,buf,12,0);				//显示AP模式的IP地址和端口
			p=(u8*)strstr((const char*)(p2+1),"STAIP,\"");
			p1=(u8*)strstr((const char*)(p+7),"\"");
			*p1=0;
			ipbuf=p+7;
			sprintf((char*)buf,"STA IP:%s 端口:%s",ipbuf,(u8*)portnum);
			//Show_Str(x,y+15,200,12,buf,12,0);				//显示STA模式的IP地址和端口
			myfree(SRAMIN,p);		//释放内存
			myfree(SRAMIN,p1);		//释放内存
			myfree(SRAMIN,p2);		//释放内存
			myfree(SRAMIN,ipbuf);		//释放内存
			myfree(SRAMIN,buf);		//释放内存
		}
}

//ATK-ESP8266模块信息显示
//x,y:显示信息的起始坐标.
//wanip:0,全部更新显示;1,仅更新wanip.
void atk_8266_msg_show(u16 x,u16 y,u8 wanip)
{
	u8 *p,*p1,*p2;
	p=mymalloc(SRAMIN,32);							//申请32字节内存
	p1=mymalloc(SRAMIN,32);							//申请32字节内存
	p2=mymalloc(SRAMIN,32);							//申请32字节内存
	POINT_COLOR=BLUE;
	atk_8266_send_cmd("AT+CWMODE=2","OK",20);
	atk_8266_send_cmd("AT+RST","OK",20);
	delay_ms(1000);//延时2s等待模块重启
	delay_ms(1000);//
	delay_ms(1000);
	delay_ms(1000);
	sprintf((char*)p,"AT+CWSAP=\"%s\",\"%s\",1,4",wifiap_ssid,wifiap_password);    //配置模块AP模式无线参数
	atk_8266_send_cmd(p,"OK",1000);
	if(wanip==0)//全更新
	{
		atk_8266_send_cmd("AT+GMR","OK",20);		//获取固件版本号
		p=atk_8266_check_cmd("SDK version:");
		//Show_Str(x,y,240,16,"固件版本:",16,0);Show_Str(x+72,y,240,16,p,16,0);
		atk_8266_send_cmd("AT+CWMODE?","+CWMODE:",20);	//获取网络模式
		p=atk_8266_check_cmd(":");
		//Show_Str(x,y+16,240,16,"网络模式:",16,0);Show_Str(x+72,y+16,240,16,(u8*)ATK_ESP8266_CWMODE_TBL[*(p+1)-'1'],16,0);
  	    atk_8266_send_cmd("AT+CWSAP?","+CWSAP:",20);	//获取wifi配置
		p=atk_8266_check_cmd("\"");
		p1=(u8*)strstr((const char*)(p+1),"\"");
		p2=p1;
		*p1=0;
		//Show_Str(x,y+32,240,16,"SSID号:",16,0);Show_Str(x+56,y+32,240,16,p+1,16,0);
		p=(u8*)strstr((const char*)(p2+1),"\"");
		p1=(u8*)strstr((const char*)(p+1),"\"");
		p2=p1;
		*p1=0;		
		//Show_Str(x,y+48,240,16,"密码:",16,0);Show_Str(x+40,y+48,240,16,p+1,16,0);
		p=(u8*)strstr((const char*)(p2+1),",");
		p1=(u8*)strstr((const char*)(p+1),",");
		*p1=0;
		//Show_Str(x,y+64,240,16,"通道号:",16,0);Show_Str(x+56,y+64,240,16,p+1,16,0);
		//Show_Str(x,y+80,240,16,"加密方式:",16,0);Show_Str(x+72,y+80,240,16,(u8*)ATK_ESP8266_ECN_TBL[*(p1+1)-'0'],16,0);
	}
	myfree(SRAMIN,p);		//释放内存 
	myfree(SRAMIN,p1);		//释放内存 
	myfree(SRAMIN,p2);		//释放内存 
}
//ATK-ESP8266模块WIFI配置参数显示(仅WIFI STA/WIFI AP模式测试时使用)
//x,y:显示信息的起始坐标.
//rmd:提示信息
//ssid,encryption,password:无线网络的SSID,加密方式,密码
void atk_8266_wificonf_show(u16 x,u16 y,u8* rmd,u8* ssid,u8* encryption,u8* password)
{ 
	POINT_COLOR=RED;
	//Show_Str(x,y,240,12,rmd,12,0);//显示提示信息 
	//Show_Str(x,y+20,240,12,"SSID:",12,0);
	//Show_Str(x,y+36,240,12,"加密方式:",12,0);
	//Show_Str(x,y+52,240,12,"密码:",12,0); 
	POINT_COLOR=BLUE;
	//Show_Str(x+30,y+20,240,12,ssid,12,0);
	//Show_Str(x+54,y+36,240,12,encryption,12,0);
	//Show_Str(x+30,y+52,240,12,password,12,0); 	  
}
//工作模式选择
//返回值:
//0,TCP服务器
//1,TCP客户端
//2,UDP模式
u8 atk_8266_netpro_sel(u16 x,u16 y,u8* name) 
{
	u8 key=0,t=0,*p;
	u8 netpro=0;
	LCD_Clear(WHITE);
	POINT_COLOR=RED;
	p=mymalloc(SRAMIN,50);//申请50个字节的内存
	sprintf((char*)p,"%s 工作模式选择",name);
	//Show_Str_Mid(0,y,p,16,240); 				    	 
	//Show_Str(x,y+30,200,16,"KEY0:下一个",16,0); 				    	 
	//Show_Str(x,y+50,200,16,"KEY1:上一个",16,0);				    	 
	//Show_Str(x,y+70,200,16,"WK_UP:确定",16,0); 
	//Show_Str(x,y+100,200,16,"请选择:",16,0); 
	POINT_COLOR=BLUE;
 	//Show_Str(x+16,y+120,200,16,"TCP服务器",16,0); 				    	 
	//Show_Str(x+16,y+140,200,16,"TCP客户端",16,0);				    	 
	//Show_Str(x+16,y+160,200,16,"UDP模式",16,0); 
	POINT_COLOR=RED;
	//Show_Str(x,y+120,200,16,"→",16,0); 
	while(1)
	{
		key=KEY_Scan(0); 
		if(key)
		{
			if(key==WKUP_PRES)break;       //WK_UP按下
			//Show_Str(x,y+120+netpro*20,200,16,"  ",16,0);//清空之前的显示
			if(key==KEY0_PRES)//KEY0按下
			{
				if(netpro<2)netpro++;
				else netpro=0;
			}else if(key==KEY1_PRES)//KEY1按下
			{
				if(netpro>0)netpro--;
				else netpro=2; 
			}
			//Show_Str(x,y+120+netpro*20,200,16,"→",16,0);//指向新条目
				
		} 
		delay_ms(10);
		atk_8266_at_response(1);
		if((t++)==20){t=0;LED0=!LED0;}//LED闪烁
	} 
	myfree(SRAMIN,p);
	return netpro;//返回网络模式选择值 
} 



//IP设置
//title:ip设置标题
//mode:工作模式
//port:端口号
//*ip:ip缓存区(返回IP给上层函数)
//返回值:0,确认连接;1,取消连接.
u8 atk_8266_ip_set(u8* title,u8* mode,u8* port,u8* ip) 
{
	u8 res=0;
// 	u8 key;
//	u8 timex=0;  
//	u8 iplen=0;			//IP长度 
	LCD_Clear(WHITE);  
	POINT_COLOR=RED; 
	//Show_Str_Mid(0,30,title,16,240);		//显示标题	
    LCD_ShowString(0,30,200,16,16,title);	
	//Show_Str(30,90,200,16,"工作模式:",16,0);	//工作模式显示
	LCD_ShowString(30,90,200,16,16,"WorkMode");
	//Show_Str(30,110,200,16,"IP地址:",16,0);	//IP地址可以键盘设置
	LCD_ShowString(30,110,200,16,16,"IP");
	//Show_Str(30,130,200,16,"端口:",16,0);	//端口号
	LCD_ShowString(30,130,200,16,16,"Port");
	//kbd_fn_tbl[0]="连接";
	//kbd_fn_tbl[1]="返回"; 
	//atk_8266_load_keyboard(0,180);			//显示键盘 
	POINT_COLOR=BLUE;
 	//Show_Str(30+72,90,200,16,mode,16,0);	//显示工作模式	
	LCD_ShowString(30+72,90,200,16,16,mode);
 	//Show_Str(30+40,130,200,16,port,16,0);	//显示端口 	
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
//				if(key==13)if(iplen)iplen--;	//删除  
//				if(key==14&&iplen)break; 		//确认连接 
//				if(key==15){res=1;break;}		//取消连接
//			} 
//			ip[iplen]=0; 
//			LCD_Fill(30+56,110,239,110+16,WHITE);
//			Show_Str(30+56,110,200,16,ip,16,0);			//显示IP地址 	
//		} 
//		timex++;
//		if(timex==20)
//		{
//			timex=0;
//			LED0=!LED0;
//		}
//		delay_ms(10);
//		atk_8266_at_response(1);//WIFI模块发过来的数据,及时上传给电脑
//	} 
	return res;
}



















































