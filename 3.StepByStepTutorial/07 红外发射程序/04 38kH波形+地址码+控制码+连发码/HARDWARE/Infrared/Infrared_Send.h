#ifndef __INFRARED_SEND_H_
#define __INFRARED_SEND_H_

#include "sys.h"
/*********************�ⲿ��������************************/  
void Infrared_Send_Status(u8 nInfrared_Send_Status);
void Infrared_Send(u8 RemoteID,u8 ControlValue);

#endif
