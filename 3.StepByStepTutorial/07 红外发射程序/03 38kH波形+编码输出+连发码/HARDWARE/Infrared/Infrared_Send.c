//******************************************************************************              
//name:             Infrared_Send.c             
//introduce:        ���⴫������������      
//author:           Դ����ο�������������Ĵ����                     
//******************************************************************************   
#include "sys.h"
#include "key.h"
#include "timer.h"
#include "Infrared_Send.h"

/*********************�궨��************************/
//���⿪��״̬
#define INFRARED_SEND_STATUS_ON             0       //�����״̬
#define INFRARED_SEND_STATUS_OFF            1       //����ر�״̬

//******************************************************************************        
//name:             Infrared_Send_Status        
//introduce:        ���ⷢ�͵Ŀ��ؿ���    
//parameter:        Timer2_PWM_Status: TIMER2_PWM_STATUS_ON or TIMER2_PWM_STATUS_OFF      
//return:           none                           
//******************************************************************************  
void Infrared_Send_Status(u8 nInfrared_Send_Status)
{
  //��PWM
  if(nInfrared_Send_Status == INFRARED_SEND_STATUS_ON)
  {
	  TIM_SelectOCxM(TIM3,TIM_Channel_2,TIM_OCMode_PWM2); //ÿ��ǿ�ƹر���PWM���ٴ�����������
	  TIM_CCxCmd(TIM3,TIM_Channel_2,TIM_CCx_Enable);
	  TIM_Cmd(TIM3, ENABLE);  //ʹ��TIM3
	  TIM_SetCompare2(TIM3,672);//����ռ�ձ�316/947=1/3��
  }
  //�ر�PWM
  else
  {
	  TIM_Cmd(TIM3, DISABLE);  //����TIM3,�ر�PWM���
	  TIM_ForcedOC2Config(TIM3,TIM_ForcedAction_InActive); //��PWM���ǿ��Ϊ�ͣ�����ر�PWMʱ�������ȷ����
  }
}

//******************************************************************************        
//name:             Infrared_Send        
//introduce:        ���ⷢ��     
//parameter:        nData:Ҫ���͵�����       
//return:           none             
//******************************************************************************  
void Infrared_Send(u32 nData)
{
  u8 i = 0;
  
  //��ʼ�����⣬Ĭ��Ϊ��
  Infrared_Send_Status(INFRARED_SEND_STATUS_OFF);
  
  //9ms��ʼ�źŵ͵�ƽ
  Infrared_Send_Status(INFRARED_SEND_STATUS_ON);
  TIM2_Delay50us(180);  
  
  //4.5ms��ʼ�źŸߵ�ƽ��
  Infrared_Send_Status(INFRARED_SEND_STATUS_OFF);
  TIM2_Delay50us(90); 
  
  //ѭ������32bit������
  for(i = 0; i < 32; i++)
  {
    //ͨ�õ�0.56ms�͵�ƽ
    Infrared_Send_Status(INFRARED_SEND_STATUS_ON);
    TIM2_Delay50us(11);    
  
    //��ֵ0
    if((nData & 0x80000000) == 0)
    {
      //0.565ms�ߵ�ƽ
      Infrared_Send_Status(INFRARED_SEND_STATUS_OFF);
      TIM2_Delay50us(11);       
    }
    //��ֵ1
    else
    {
      //1.69ms�ߵ�ƽ��
      Infrared_Send_Status(INFRARED_SEND_STATUS_OFF);
      TIM2_Delay50us(34); 
    }

    //������λ
    nData <<= 1;
  }
  
  //----------------------------------------------------------------------------------
  //���������û��̧�������ͷ�������
  // NEC ��涨��������(�� 9ms �͵�ƽ+2.5ms �ߵ�ƽ+0.56ms �͵�ƽ+97.94ms �ߵ�ƽ���)
  while(KEY0==0||KEY1==0||KEY2==0||WK_UP==1)
  {
	 //9ms�͵�ƽ
     Infrared_Send_Status(INFRARED_SEND_STATUS_ON);
     TIM2_Delay50us(180);  
  
     //2.5ms�ߵ�ƽ��
	Infrared_Send_Status(INFRARED_SEND_STATUS_OFF);
	TIM2_Delay50us(50); 
  
	 //0.56ms�͵�ƽ
     Infrared_Send_Status(INFRARED_SEND_STATUS_ON);
     TIM2_Delay50us(11);  
  
     //97.94ms�ߵ�ƽ��
	Infrared_Send_Status(INFRARED_SEND_STATUS_OFF);
	TIM2_Delay50us(1899); 
  }
  //----------------------------------------------------------------------------------
  //0.263ms�͵�ƽ��Ϊ����λ
  Infrared_Send_Status(INFRARED_SEND_STATUS_ON);
  TIM2_Delay50us(5); 
  
  //�رպ���
  Infrared_Send_Status(INFRARED_SEND_STATUS_OFF);  
}
