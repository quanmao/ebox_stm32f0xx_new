/**
  ******************************************************************************
  * @file    ebox_define.h
  * @author  cat_li
  * @version V2.0
  * @date    2016/10/21
  * @brief   
  ******************************************************************************
  * @attention
  *
  * No part of this software may be used for any commercial activities by any form 
  * or means, without the prior written consent of shentq. This specification is 
  * preliminary and is subject to change at any time without notice. shentq assumes
  * no responsibility for any errors contained herein.
  * <h2><center>&copy; Copyright 2015 shentq. All Rights Reserved.</center></h2>
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/

#ifndef __EBOX_DEFINE_H
#define __EBOX_DEFINE_H

#include "ebox_config.h"

#define EBOX_VERSION "2.0"

#define interrupts() 					__enable_irq()//允许所有中断
#define no_interrupts() 	    __disable_irq()//禁止所有中断

typedef enum {
    E_NG = 0,
	E_OK ,
	E_WATE,
	E_BUSY,
	E_TIMEOUT,
	E_PARA,
	E_2LONG,
	E_OTHER,
	E_NC = 0xFFFFFF
}E_STATE;
    
typedef enum 
{
    locked = 0,
    unlock
}E_LOCK_T;

// 空函数，不做任何操作，用来给函数指针赋值，防止函数指针 = null时被调用，出错
void nullFun(void);
typedef void (*fun_noPara_t)(void);
typedef void (*fun_onePara_t)(unsigned int);

#define PI 3.1415926

#endif

