//******************************************************************************              
//name:             Infrared_Send.c             
//introduce:        红外传感器发送驱动      
//author:           源程序参考————甜甜的大香瓜                     
//******************************************************************************   
#include "sys.h"
#include "key.h"
#include "timer.h"
#include "Infrared_Send.h"

/*********************宏定义************************/
//红外开关状态
#define INFRARED_SEND_STATUS_ON             0       //红外打开状态
#define INFRARED_SEND_STATUS_OFF            1       //红外关闭状态

//******************************************************************************        
//name:             Infrared_Send_Status        
//introduce:        红外发送的开关开关    
//parameter:        Timer2_PWM_Status: TIMER2_PWM_STATUS_ON or TIMER2_PWM_STATUS_OFF      
//return:           none                           
//******************************************************************************  
void Infrared_Send_Status(u8 nInfrared_Send_Status)
{
  //打开PWM
  if(nInfrared_Send_Status == INFRARED_SEND_STATUS_ON)
  {
	  TIM_SelectOCxM(TIM3,TIM_Channel_2,TIM_OCMode_PWM2); //每次强制关闭了PWM，再次启动需配置
	  TIM_CCxCmd(TIM3,TIM_Channel_2,TIM_CCx_Enable);
	  TIM_Cmd(TIM3, ENABLE);  //使能TIM3
	  TIM_SetCompare2(TIM3,672);//设置占空比316/947=1/3。
  }
  //关闭PWM
  else
  {
	  TIM_Cmd(TIM3, DISABLE);  //禁能TIM3,关闭PWM输出
	  TIM_ForcedOC2Config(TIM3,TIM_ForcedAction_InActive); //将PWM输出强制为低，避免关闭PWM时的输出不确定性
  }
}

//******************************************************************************        
//name:             Infrared_Send        
//introduce:        红外发送     
//parameter:        nData:要发送的数据       
//return:           none             
//******************************************************************************  
void Infrared_Send(u8 RemoteID,u8 ControlValue)
{
  u8 i = 0;
  u32 nData;
  nData = (u32) ( (RemoteID<<24) + (((u8) ~RemoteID)<<16) + (ControlValue<<8) + ((u8) ~ControlValue));
  
  //初始化红外，默认为关
  Infrared_Send_Status(INFRARED_SEND_STATUS_OFF);
  
  //9ms起始信号低电平
  Infrared_Send_Status(INFRARED_SEND_STATUS_ON);
  TIM2_Delay50us(180);  
  
  //4.5ms起始信号高电平，
  Infrared_Send_Status(INFRARED_SEND_STATUS_OFF);
  TIM2_Delay50us(90); 
  
  //循环发送32bit的数据
  for(i = 0; i < 32; i++)
  {
    //通用的0.56ms低电平
    Infrared_Send_Status(INFRARED_SEND_STATUS_ON);
    TIM2_Delay50us(11);    
  
    //数值0
    if((nData & 0x80000000) == 0)
    {
      //0.565ms高电平
      Infrared_Send_Status(INFRARED_SEND_STATUS_OFF);
      TIM2_Delay50us(11);       
    }
    //数值1
    else
    {
      //1.69ms高电平，
      Infrared_Send_Status(INFRARED_SEND_STATUS_OFF);
      TIM2_Delay50us(34); 
    }

    //数据移位
    nData <<= 1;
  }
  
  //----------------------------------------------------------------------------------
  //如果按键还没有抬起来，就发联发码
  // NEC 码规定的连发码(由 9ms 低电平+2.5ms 高电平+0.56ms 低电平+97.94ms 高电平组成)
  while(KEY0==0||KEY1==0||KEY2==0||WK_UP==1)
  {
	 //9ms低电平
     Infrared_Send_Status(INFRARED_SEND_STATUS_ON);
     TIM2_Delay50us(180);  
  
     //2.5ms高电平，
	Infrared_Send_Status(INFRARED_SEND_STATUS_OFF);
	TIM2_Delay50us(50); 
  
	 //0.56ms低电平
     Infrared_Send_Status(INFRARED_SEND_STATUS_ON);
     TIM2_Delay50us(11);  
  
     //97.94ms高电平，
	Infrared_Send_Status(INFRARED_SEND_STATUS_OFF);
	TIM2_Delay50us(1899); 
  }
  //----------------------------------------------------------------------------------
  //0.263ms低电平作为结束位
  Infrared_Send_Status(INFRARED_SEND_STATUS_ON);
  TIM2_Delay50us(5); 
  
  //关闭红外
  Infrared_Send_Status(INFRARED_SEND_STATUS_OFF);  
}
