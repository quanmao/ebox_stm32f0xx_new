/**
  ******************************************************************************
  * @file    ebox_define.h
  * @author  cat_li
  * @version V2.0
  * @date    2016/10/21
  * @brief   ebox���������ļ���ebox����ģ�������ø�ͷ�ļ�
	  2017/7/18  cat_li
	  1 ���ܴ��ڵķ��գ�GetEndTime,WaitTimeOutδ����millis_seconds�������
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

// ��׼�����	
#include "stdio.h"		// ��׼�������
#include "stdlib.h"		// ��׼�� size_t,rand,srand,malloc.....
#include "stdint.h"		// �����������

#define EBOX_VERSION "STM32F0-2.0"

extern volatile uint8_t irqCount;
// ����ɶԳ���
#define interrupts() 			if (--irqCount == 0) __enable_irq(); //���������ж�
#define no_interrupts() 	    __disable_irq();irqCount++;			 //��ֹ�����ж�

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

// �պ����������κβ���������������ָ�븳ֵ����ֹ����ָ�� = nullʱ�����ã�����
void nullFun(void);
typedef void (*fun_noPara_t)(void);
typedef void (*fun_onePara_t)(unsigned int);

#define PI 3.1415926

#define	abs(x)									((x) > 0 ? (x) : -(x))

#define GetEndTime(timeOut)						millis_seconds + timeOut
// ��ʱ,����1 ���򷵻�0
#define IsTimeOut(endTime)						millis_seconds >= endTime

#define SetBit(data,offset)						((date) |= 1U << (offset))
#define ResetBit(data,offset)					((date) &= ~(1U << (offset)))
#define GetBit(data,offset)						(((data)>>(offset)) & 0x01)
// ��ȡ32bit�����еĵ�x��uint8_t,uint16_t
#define GetUint8_t0(data)						((uint8_t*)(&(data)))[0]
#define GetUint8_t1(data)						((uint8_t*)(&(data)))[1]
#define GetUint8_t2(data)						((uint8_t*)(&(data)))[2]
#define GetUint8_t3(data)						((uint8_t*)(&(data)))[3]
#define GetUint16_t0(data)						((uint16_t*)(&(data)))[0]
#define GetUint16_t0(data)						((uint16_t*)(&(data)))[0]

#endif

