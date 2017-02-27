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


#define EBOX_VERSION "2.0"

#define interrupts() 			__enable_irq()//允许所有中断
#define no_interrupts() 	    __disable_irq()//禁止所有中断

typedef enum {
	E_OK = 1,
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

typedef void (*callback_fun_type)(void);

////////系统debug支持////////////////////////////////////////////////////////////////

#if EBOX_DEBUG
//extern Uart uart1;//根据不同的串口名称此处需要做相应的修改
#define DBG(...) uart1.printf(__VA_ARGS__)
#else
#define  DBG(...)
#endif

#endif

