/**
  ******************************************************************************
  * @file    ebox_define.h
  * @author  cat_li
  * @version V2.0
  * @date    2016/10/21
  * @brief   ebox基础定义文件，ebox所有模块均需调用该头文件
	  2017/7/18  cat_li
	  1 可能存在的风险，GetEndTime,WaitTimeOut未考虑millis_seconds溢出问题
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

// 标准库组件	
#include "stdio.h"		// 标准输入输出
#include "stdlib.h"		// 标准库 size_t,rand,srand,malloc.....
#include "stdint.h"		// 数据类型相关

#define EBOX_VERSION "STM32F0-2.0"

extern volatile uint8_t irqCount;
// 必须成对出现
#define interrupts() 			if (--irqCount == 0) __enable_irq(); //允许所有中断
#define no_interrupts() 	    __disable_irq();irqCount++;			 //禁止所有中断

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

#define	abs(x)									((x) > 0 ? (x) : -(x))

#define GetEndTime(timeOut)						millis_seconds + timeOut
// 超时,返回1 否则返回0
#define IsTimeOut(endTime)						millis_seconds >= endTime

#define SetBit(data,offset)						((date) |= 1U << (offset))
#define ResetBit(data,offset)					((date) &= ~(1U << (offset)))
#define GetBit(data,offset)						(((data)>>(offset)) & 0x01)
// 获取32bit变量中的第x个uint8_t,uint16_t
#define GetUint8_t0(data)						((uint8_t*)(&(data)))[0]
#define GetUint8_t1(data)						((uint8_t*)(&(data)))[1]
#define GetUint8_t2(data)						((uint8_t*)(&(data)))[2]
#define GetUint8_t3(data)						((uint8_t*)(&(data)))[3]
#define GetUint16_t0(data)						((uint16_t*)(&(data)))[0]
#define GetUint16_t0(data)						((uint16_t*)(&(data)))[0]
#define word(a,b)										((a<<8)|b)

#endif

