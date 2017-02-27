/**
  ******************************************************************************
  * @file    timer.cpp
  * @author  shentq
  * @version V2.0
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
 * -shentq                  -version 1.2(2016/08/17)
 *      *增加了TIM1的支持；
 *      *增加了get_timer_clock(),get_max_frq()两个函数接口
 *      *修复了芯片主频变动导致定时器中断频率变化的bug，支持任意主频下正常工作
 * -LQM (2016/9/6)
 *      *移植到STM32F0,基于HAL库LL层  最大输出频率为 TIM时钟频率/400
 */

/* Includes ------------------------------------------------------------------*/
#include "timer.h"
#include "timer_it.h"


static tim_irq_handler irq_handler;
static uint32_t tim_irq_ids[TIM_IRQ_ID_NUM];

int tim_irq_init(uint8_t index,tim_irq_handler handler,uint32_t id)
{
 tim_irq_ids[index] = id;
 irq_handler =  handler;
 return 0;
}

void tim_irq_callback(uint8_t index)
{
	irq_handler(tim_irq_ids[index]);
}

//////////////////////////////////////

Timer::Timer(TIM_TypeDef *TIMx)
{
    _TIMx = TIMx;
}

void Timer::begin(uint32_t frq)
{
	uint32_t _period  = 0;
	uint32_t _prescaler = 1;
	uint8_t index;

	if (frq >= get_max_frq())frq = get_max_frq();//控制最大频率
	for (; _prescaler <= 0xffff; _prescaler++)
	{
		_period = get_timer_source_clock() / _prescaler / frq;
		if ((0xffff >= _period))break;
	}

	base_init(_period-1, _prescaler-1);

	switch ((uint32_t)_TIMx)
	{
	case (uint32_t)TIM1_BASE:
		index = TIM1_IT_Update;
		break;
#ifdef TIM2
	case (uint32_t)TIM2_BASE:
		index = TIM2_IT_Update;
		break;
#endif
//    case (uint32_t)TIM3_BASE:
//        index = TIM3_IT_Update;
//        break;
//    case (uint32_t)TIM4_BASE:
//        index = TIM4_IT_Update;
//        break;
//    case (uint32_t)TIM5_BASE:
//        index = TIM5_IT_Update;
//        break;
//    case (uint32_t)TIM6_BASE:
//        index = TIM6_IT_Update;
//        break;
//    case (uint32_t)TIM7_BASE:
//        index = TIM7_IT_Update;
//        break;
	}
	tim_irq_init(index,(&Timer::_irq_handler),(uint32_t)this);
}


void Timer::reset_frq(uint32_t frq)
{
    begin(frq);
    interrupt(ENABLE);
    start();
}

void Timer::interrupt(FunctionalState enable)
{
	  LL_TIM_ClearFlag_UPDATE(_TIMx); //必须加，否则开启中断会立即产生一次中断
		if(enable == ENABLE)
			LL_TIM_EnableIT_UPDATE(_TIMx); 
		else
			LL_TIM_DisableIT_UPDATE(_TIMx); 
}

void Timer::start(void)
{
	/* Enable counter */
    LL_TIM_EnableCounter(_TIMx);  
		LL_TIM_GenerateEvent_UPDATE(_TIMx);
}

void Timer::stop(void)
{
		LL_TIM_DisableCounter(_TIMx);
}

void Timer::base_init(uint16_t period, uint16_t prescaler)
{
//    TIM_DeInit(_TIMx);
    switch((uint32_t)_TIMx)
    {
    case (uint32_t)TIM1:
//        RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);
//        //此处和通用定时器不一样 控制定时器溢出多少次产生一次中断
//        TIM_TimeBaseStructure.TIM_RepetitionCounter = 0 ;
//        NVIC_InitStructure.NVIC_IRQChannel = TIM1_UP_IRQn;//
//        break;
#ifdef TIM2
    case (uint32_t)TIM2_BASE:
          /* Enable the timer peripheral clock */
				LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_TIM2);   
          /* Configure the NVIC to handle TIM2 update interrupt */
				NVIC_SetPriority(TIM2_IRQn, 0);
				NVIC_EnableIRQ(TIM2_IRQn); 
        break;
#endif
    case (uint32_t)TIM3_BASE:
          /* Enable the timer peripheral clock */
				LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_TIM3);   
          /* Configure the NVIC to handle TIM2 update interrupt */
				NVIC_SetPriority(TIM3_IRQn, 0);
				NVIC_EnableIRQ(TIM3_IRQn); 
        break;
//    case (uint32_t)TIM4_BASE:
//        RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);
//        NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn;//
//        break;
//#if defined (STM32F10X_HD)
//    case (uint32_t)TIM5_BASE:
//        RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5, ENABLE);
//        NVIC_InitStructure.NVIC_IRQChannel = TIM5_IRQn;//
//        break;
//    case (uint32_t)TIM6_BASE:
//        RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM6, ENABLE);
//        NVIC_InitStructure.NVIC_IRQChannel = TIM6_IRQn;//
//        break;
//    case (uint32_t)TIM7_BASE:
//        RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM7, ENABLE);
//        NVIC_InitStructure.NVIC_IRQChannel = TIM7_IRQn;//
//        break;
//#endif
    }
  /* Set counter mode */
   LL_TIM_SetCounterMode(_TIMx, LL_TIM_COUNTERMODE_UP);

  /*
    In this example TIM2 input clock (TIM2CLK)  is set to APB1 clock (PCLK1),
    since APB1 prescaler is equal to 1.
      TIM2CLK = PCLK1
      PCLK1 = HCLK
      => TIM2CLK = HCLK = SystemCoreClock
    To get TIM2 counter clock at 10 KHz, the Prescaler is computed as following:
    Prescaler = (TIM2CLK / TIM2 counter clock) - 1
    Prescaler = (SystemCoreClock /10 KHz) - 1
  */
  LL_TIM_SetPrescaler(_TIMx, prescaler);  
  /* Set the auto-reload value to have an initial update event frequency of 10 Hz */
  LL_TIM_SetAutoReload(_TIMx, period);
}

void Timer::set_reload(uint16_t auto_reload)
{
		LL_TIM_SetAutoReload(_TIMx, auto_reload);
}
void Timer::clear_count(void)
{
    _TIMx->CNT = 0;
}

uint32_t Timer::get_timer_source_clock()
{
  uint32_t timer_clock = 0x00;
    
  	// 计算TIM时钟频率
	if (LL_RCC_GetAPB1Prescaler() == 0)
	{		
		timer_clock = cpu.clock.pclk1;
	}else{
		timer_clock = cpu.clock.pclk1*2;
	} 
    return timer_clock;	
}

uint32_t Timer::get_max_frq()
{
    return get_timer_source_clock()/400;  //防止频率太高，影响主程序运行
}


void Timer::_irq_handler( uint32_t id){ 
		Timer *handler = (Timer*)id;
		handler->_irq.call();
}

void Timer::attach(void (*fptr)(void)) {
    if (fptr) {
        _irq.attach(fptr);
		}
}




