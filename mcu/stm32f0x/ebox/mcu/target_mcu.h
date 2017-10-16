/******************************************************************************
程 序 名： target_mcu.h　
编 写 人： cat_li
编写时间： 2017年9月11日
相关平台： STM32F0xx, ebox
修改日志：　　
	NO.1-  初始版本
******************************************************************************/

#ifndef __MCU_SELECT_H_
#define __MCU_SELECT_H_

#ifdef __cplusplus
 extern "C" {
#endif /* __cplusplus */

#if 	defined(STM32F030x6)
	#include "stm32f030_define.h"
#elif defined(STM32F072xB)
	#include "stm32f072_define.h"	
#else
	#error 	 "Please select first the target STM32F0xx device used in your application (in stm32f0xx.h file)"
#endif

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif
