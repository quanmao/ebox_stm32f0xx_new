/**
  ******************************************************************************
  * @file    ebox_IRQ.cpp
  * @author  cat_li
  * @version V2.0
  * @date    2016/10/26
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
#include "ebox_irq.h"
#include "stm32f0xx_ll_exti.h"
#include "stm32f0xx_ll_bus.h"
#include "stm32f0xx_ll_system.h"

typedef void (*exti_irq_handler)(uint32_t id);

static exti_irq_handler  irq_handler;   // 声明函数指针变量，指向类的静态成员
static uint32_t  exti_irq_ids[16];    	// 保存对象地址，供静态成员识别对象，并访问对象的普通成员

int exti_irq_init(uint8_t index,exti_irq_handler handler,uint32_t id)
{
    exti_irq_ids[index] = id;      // 保存对象地址
    irq_handler =  handler;        // 指向回调函数
    return 0;
}


void IRQ::IRQ_ENABLE(void){
    
    exti_irq_init(13,(&IRQ::irq_handler),(uint32_t)this);
	
    LL_APB1_GRP2_EnableClock(LL_APB1_GRP2_PERIPH_SYSCFG);
   
    LL_SYSCFG_SetEXTISource(LL_SYSCFG_EXTI_PORTC, LL_SYSCFG_EXTI_LINE13);
	  LL_EXTI_EnableIT_0_31(LL_EXTI_LINE_13);
    LL_EXTI_EnableFallingTrig_0_31(LL_EXTI_LINE_13);
//    LL_EXTI_EnableRisingTrig_0_31(LL_EXTI_LINE_13);
    
    NVIC_EnableIRQ(EXTI4_15_IRQn);
    NVIC_SetPriority(EXTI4_15_IRQn,0);
}

void IRQ::irq_handler(uint32_t id)
{ 
    IRQ *handler = (IRQ*)id;  // 指向回调函数地址
    handler->pirq.call();     // 调用回调函数
}

extern "C" {
void EXTI4_15_IRQHandler(void)
{
    if (LL_EXTI_IsActiveFlag_0_31(LL_EXTI_LINE_13) != RESET)
    {
        LL_EXTI_ClearFlag_0_31(LL_EXTI_LINE_13);
        /* Manage code in main.c.*/
        irq_handler(exti_irq_ids[13]);
    }
    if (LL_EXTI_IsActiveFlag_0_31(LL_EXTI_LINE_1) != RESET)
    {
        LL_EXTI_ClearFlag_0_31(LL_EXTI_LINE_1);
        /* Manage code in main.c.*/
        irq_handler(exti_irq_ids[1]);
    }
}
}
