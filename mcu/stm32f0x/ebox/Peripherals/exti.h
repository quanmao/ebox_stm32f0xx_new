/**
  ******************************************************************************
  * @file    exti.h
  * @author  shentq
  * @version V1.2
  * @date    2016/08/14
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
/**
 * Modification History:
 * -LQM (2016/9/6)
 *      *��ֲ��STM32F0,����HAL��LL��
 */
/* Define to prevent recursive inclusion -------------------------------------*/

#ifndef __EXTI_H
#define __EXTI_H
#include "common.h"
#include "FunctionPointer.h"

/*
	1.�ṩһ��io�ж�
	*ע�⣺stm32һ���ж���EXTI_Linexֻ�����ӵ�һ��port��GPIO_Pin_x��������PA0Ϊ�ж�Դ֮��Ͳ�������PB0��PC0��Ϊ�ж�Դ
*/

//EXTITrigger_TypeDef����ֵ
//	LL_EXTI_TRIGGER_RISING
//	LL_EXTI_TRIGGER_FALLING
//	LL_EXTI_TRIGGER_RISING_FALLING
class Exti
{
public:
    Exti(Gpio *exti_pin, uint8_t trigger);
    void begin();
    void attach_interrupt(void (*callback_fun)(void));
    void interrupt(FunctionalState enable);

    static void _irq_handler( uint32_t id);
    void attach(void (*fptr)(void));
    template<typename T>
    void attach(T* tptr, void (T::*mptr)(void)) {
        _irq.attach(tptr, mptr);
    }

private:
    Gpio                *exti_pin;
    uint8_t				trigger;
    uint8_t             port_source;
    uint32_t            pin_source;
    uint32_t            exti_line;
    IRQn_Type           irq;

    void init_info(Gpio *exti_pin);
protected:
    FunctionPointer _irq;
};
#ifdef __cplusplus
extern "C" {
#endif


typedef void (*exti_irq_handler)(uint32_t id);

int exti_irq_init(uint8_t index,exti_irq_handler handler,uint32_t id);
	
#ifdef __cplusplus
}
#endif

#endif
