/******************************************************************************
�� �� ���� target_mcu.h��
�� д �ˣ� cat_li
��дʱ�䣺 2017��9��11��
���ƽ̨�� STM32F0xx, ebox
�޸���־������
	NO.1-  ��ʼ�汾
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
