/**
  ******************************************************************************
  * @file    ebox_exti.cpp
  * @author  cat_li
  * @version V2.0
  * @date    2016/10/26
  * @brief   
		 1	2017/7/13	cat_li	修改文件名为ebox_exti
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
#include "ebox_exti.h"
#include "stm32f0xx_ll_exti.h"
#include "stm32f0xx_ll_bus.h"
#include "stm32f0xx_ll_system.h"

typedef void (*exti_irq_handler)(uint32_t id);

static exti_irq_handler  exti_handler;   // 声明函数指针变量，指向类的静态成员
static uint32_t  exti_irq_ids[16];    	 // 保存对象地址，供静态成员识别对象，并访问对象的普通成员
static E_GPIO		 *pin_ids[16];		 // 保存外部中断GPIO对象，用来判断上升沿or下降沿

int exti_irq_init(uint8_t index,exti_irq_handler handler,uint32_t id,E_GPIO *pin)
{
		pin_ids[index] = pin;
    exti_irq_ids[index] = id;      // 保存对象地址
    exti_handler =  handler;        // 指向回调函数
    return 0;
}
E_EXTI::E_EXTI(E_GPIO *pin,ExtiType type){
	_pin = pin;

	exti_irq_init(13,(&E_EXTI::_irq_handler),(uint32_t)this,pin);
	switch (type)
	{
		case IT:
			LL_EXTI_EnableIT_0_31(LL_EXTI_LINE_13);
			LL_EXTI_DisableEvent_0_31(LL_EXTI_LINE_13);
			break;
		case EVENT:
			LL_EXTI_EnableEvent_0_31(LL_EXTI_LINE_13);
			LL_EXTI_DisableIT_0_31(LL_EXTI_LINE_13);
			break;
		case IT_EVENT:
			LL_EXTI_EnableIT_0_31(LL_EXTI_LINE_13);
			LL_EXTI_EnableEvent_0_31(LL_EXTI_LINE_13);
			break;
	default:
		break;
	}
}

E_EXTI::E_EXTI(E_PinID id,ExtiType type){
	_pin = new E_GPIO(id);
	_pin->mode(INPUT);
	exti_irq_init(13,(&E_EXTI::_irq_handler),(uint32_t)this,_pin);
	switch (type)
	{
		case IT:
			LL_EXTI_EnableIT_0_31(LL_EXTI_LINE_13);
			LL_EXTI_DisableEvent_0_31(LL_EXTI_LINE_13);
			break;
		case EVENT:
			LL_EXTI_EnableEvent_0_31(LL_EXTI_LINE_13);
			LL_EXTI_DisableIT_0_31(LL_EXTI_LINE_13);
			break;
		case IT_EVENT:
			LL_EXTI_EnableIT_0_31(LL_EXTI_LINE_13);
			LL_EXTI_EnableEvent_0_31(LL_EXTI_LINE_13);
			break;
	default:
		break;
	}
}

/**
 *@brief    使能中断
 *@param    NONE
 *@retval   NONE
*/
//void E_EXTI::ENABLE(ExtiType type){
//    
//    exti_irq_init(13,(&E_EXTI::_irq_handler),(uint32_t)this);
//	
//    LL_APB1_GRP2_EnableClock(LL_APB1_GRP2_PERIPH_SYSCFG);
//   
//    LL_SYSCFG_SetEXTISource(LL_SYSCFG_EXTI_PORTC, LL_SYSCFG_EXTI_LINE13);
//	LL_EXTI_EnableIT_0_31(LL_EXTI_LINE_13);
//    LL_EXTI_EnableFallingTrig_0_31(LL_EXTI_LINE_13);
////    LL_EXTI_EnableRisingTrig_0_31(LL_EXTI_LINE_13);
//	//type == FALLING ? LL_EXTI_EnableFallingTrig_0_31(LL_EXTI_LINE_13):LL_EXTI_EnableRisingTrig_0_31(LL_EXTI_LINE_13);
//    
//    NVIC_EnableIRQ(EXTI4_15_IRQn);
//    NVIC_SetPriority(EXTI4_15_IRQn,0);
//}

void E_EXTI::ENABLE(TrigType type){
	switch (type)
	{
		case FALLING:
			LL_EXTI_EnableFallingTrig_0_31(LL_EXTI_LINE_13);
			LL_EXTI_DisableRisingTrig_0_31(LL_EXTI_LINE_13);			
			break;
		case RISING:
			LL_EXTI_EnableRisingTrig_0_31(LL_EXTI_LINE_13);
			LL_EXTI_DisableFallingTrig_0_31(LL_EXTI_LINE_13);
			break;
		case FALL_RISING:
			LL_EXTI_EnableFallingTrig_0_31(LL_EXTI_LINE_13);
			LL_EXTI_EnableRisingTrig_0_31(LL_EXTI_LINE_13);
			break;
	default:
		break;
	}
	
	
	
    LL_APB1_GRP2_EnableClock(LL_APB1_GRP2_PERIPH_SYSCFG);
   
    LL_SYSCFG_SetEXTISource(LL_SYSCFG_EXTI_PORTC, LL_SYSCFG_EXTI_LINE13);
			
	  NVIC_EnableIRQ(EXTI4_15_IRQn);
    NVIC_SetPriority(EXTI4_15_IRQn,0);
}

/**
 *@brief    EXTI 静态成员函数，在中断中调用，解析执行相关回调函数
 *@param    uint32_t id 对象地址，用来识别对象；IrqType irq_type 中断类型 
 *@retval   NONE
*/
void E_EXTI::_irq_handler(uint32_t id)
{	
    E_EXTI *handler = (E_EXTI*)id;  // 指向回调函数地址	
		
		handler->_pirq[pin_ids[13]->read()].call();
	  //pin_ids[id]->read() == 0 ? handler->_pirq[0].call() : handler->_pirq[1].call();
        // 调用回调函数
}

/**
 *@brief    uart 绑定中断
 *@param    IrqType type 中断类型，RxIrq,TxIrq; void (*fptr)(void) 回调函数指针
 *@retval   NONE
*/
//void E_UART::attach(void (*fptr)(void), IrqType type) {
//    if (fptr != NULL) {
//        _irq[type].attach(fptr);
//    }
//}

/**
 *@brief    uart 静态成员函数，在中断中调用，解析执行相关回调函数
 *@param    uint32_t id 对象地址，用来识别对象；IrqType irq_type 中断类型 
 *@retval   NONE
*/
//void E_UART::_irq_handler(uint32_t id, IrqType irq_type) {
//    E_UART *handler = (E_UART*)id;
//    handler->_irq[irq_type].call();
//}

extern "C" {
void EXTI4_15_IRQHandler(void)
{
    if (LL_EXTI_IsActiveFlag_0_31(LL_EXTI_LINE_13) != RESET)
    {
        LL_EXTI_ClearFlag_0_31(LL_EXTI_LINE_13);
        exti_handler(exti_irq_ids[13]);
    }
    if (LL_EXTI_IsActiveFlag_0_31(LL_EXTI_LINE_1) != RESET)
    {
        LL_EXTI_ClearFlag_0_31(LL_EXTI_LINE_1);
        exti_handler(exti_irq_ids[1]);
    }
}
}
