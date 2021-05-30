#include "timer.h"
#include "led.h"
#include "usart.h"
#include "Infrared_Send.h"

/*******************************************内部变量**********************************************************************/
static u32 g_Timer2_Delay=0;
u8 g_KeyIsOn=0;   

/*********************宏定义************************/
//红外开关状态
#define INFRARED_SEND_STATUS_ON             0       //红外打开状态
#define INFRARED_SEND_STATUS_OFF            1       //红外关闭状态

//******************************************************************************        
//name:             TIM2_Delay50us       
//introduce:        定时器延时50us   
//parameter:        n_Time     
//return:           none                           
//****************************************************************************** 
void TIM2_Delay50us(u32 n_Time)
{
	g_Timer2_Delay = n_Time;
	while(g_Timer2_Delay != 0);
}

//=========================================================================================================================== 	  
//通用定时器2中断初始化
//这里时钟选择为APB1的2倍，而APB1为36M
//arr：自动重装值。
//psc：时钟预分频数
//这里使用的是定时器3!
void TIM2_Int_Init(u16 arr,u16 psc)
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE); //时钟使能

	TIM_TimeBaseStructure.TIM_Period = arr; //设置在下一个更新事件装入活动的自动重装载寄存器周期的值	 计数到50为50us
	TIM_TimeBaseStructure.TIM_Prescaler =psc; //设置用来作为TIMx时钟频率除数的预分频值  1Mhz的计数频率  
	TIM_TimeBaseStructure.TIM_ClockDivision = 0; //设置时钟分割:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM向上计数模式
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure); //根据TIM_TimeBaseInitStruct中指定的参数初始化TIMx的时间基数单位
 
	TIM_ITConfig(TIM2,TIM_IT_Update,ENABLE ); //使能指定的TIM2中断,允许更新中断

	NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;  //TIM2中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  //先占优先级0级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;  //从优先级3级
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQ通道被使能
	NVIC_Init(&NVIC_InitStructure);  //根据NVIC_InitStruct中指定的参数初始化外设NVIC寄存器

	TIM_Cmd(TIM2, ENABLE);  //使能TIMx外设							 
}

//定时器2中断服务程序
void TIM2_IRQHandler(void)   //TIM3中断
{
	u8 i = 0;
	u32 nData;
	static u8 ProessControl = 0;
	
	if(TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET) //检查指定的TIM中断发生与否:TIM 中断源 
	{
		TIM_ClearITPendingBit(TIM2, TIM_IT_Update  );  //清除TIMx的中断待处理位:TIM 中断源 
//		if(g_Timer2_Delay != 0X00)
//		{
//			g_Timer2_Delay--;  //每次到达50us时间，定时基数自动减1，直至减完到达计时时间
//		}
//屏蔽上面段使用下面程序
		if(g_KeyIsOn==1)
		{ 
		  switch(ProessControl)
		  {
			  case 0: nData = (u32) ( 0xFF0000 + (0x66<<8) + ((u8) ~(0x66)));
					  //初始化红外，默认为关
					  Infrared_Send_Status(INFRARED_SEND_STATUS_OFF);
					  //9ms起始信号低电平
		              Infrared_Send_Status(INFRARED_SEND_STATUS_ON);
			          TIM2->ARR = 9000;
			          ProessControl = 1;
					  break;
			  
			  case 1: //TIM2_Delay50us(180);  		  
					  //4.5ms起始信号高电平，
					  Infrared_Send_Status(INFRARED_SEND_STATUS_OFF);
			          TIM2->ARR = 4500;
			          ProessControl = 2;
					  break;

			  case 2://TIM2_Delay50us(90);    //2019.01.13 22:33 今天由于时间原因，没有把程序写完，下次打时间再写
		  
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
					  while(g_KeyIsOn==1)
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
					  break;
		  //----------------------------------------------------------------------------------
			  default: break;
		}
	  }
	}
}
//===========================================================================================================================
//TIM3 PWM部分初始化 
//PWM输出初始化
//arr：自动重装值
//psc：时钟预分频数
void TIM3_PWM_Init(u16 arr,u16 psc)
{  
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);	//使能定时器3时钟
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB  | RCC_APB2Periph_AFIO, ENABLE);  //使能GPIO外设和AFIO复用功能模块时钟
	
	GPIO_PinRemapConfig(GPIO_PartialRemap_TIM3, ENABLE); //Timer3部分重映射  TIM3_CH2->PB5    
 
   //设置该引脚为复用输出功能,输出TIM3 CH2的PWM脉冲波形	GPIOB.5
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5; //TIM_CH2
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  //复用推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);//初始化GPIO
 
   //初始化TIM3
	TIM_TimeBaseStructure.TIM_Period = arr; //设置在下一个更新事件装入活动的自动重装载寄存器周期的值
	TIM_TimeBaseStructure.TIM_Prescaler =psc; //设置用来作为TIMx时钟频率除数的预分频值 
	TIM_TimeBaseStructure.TIM_ClockDivision = 0; //设置时钟分割:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM向上计数模式
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure); //根据TIM_TimeBaseInitStruct中指定的参数初始化TIMx的时间基数单位
	
	//初始化TIM3 ChanneL_2 PWM模式	   //PWM1模式时，比较输出极性为高设定下，技术大于设定值时，输出为高。及占空比为1/3
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; //选择定时器模式:TIM脉冲宽度调制模式1
 	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //比较输出使能
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; //输出极性:TIM输出比较极性高
	TIM_OC2Init(TIM3, &TIM_OCInitStructure);  //根据T指定的参数初始化外设TIM3 OC2

	TIM_OC2PreloadConfig(TIM3, TIM_OCPreload_Enable);  //使能TIM3在CCR2上的预装载寄存器
 
	//TIM_Cmd(TIM3, ENABLE);  
	TIM_Cmd(TIM3, DISABLE);  //禁能TIM3
	
	TIM_SetCompare2(TIM3,672);//设置占空比316/947=1/3。
}
