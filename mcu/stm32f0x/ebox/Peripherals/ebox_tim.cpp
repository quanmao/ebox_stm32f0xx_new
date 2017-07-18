/**
  ******************************************************************************
  * @file    ebox_tim.cpp
  * @author  cat_li
  * @version V2.0
  * @date    2016/10/23
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
#include "ebox_tim.h"
#include "ebox_core.h"
#include "stm32f0xx_ll_tim.h"
#include "stm32f0xx_ll_rcc.h"
#include "ebox_debug.h"

E_TIMBase1::E_TIMBase1(TIM_TypeDef *TIMx){
	timx = TIMx;
}

//void E_TIMBase1::init(uint16_t _period, uint16_t _prescaler){

//		uint8_t index = PeriphIndex((uint32_t)timx,Periph_MAP);
//    /* Enable the timer peripheral clock */
//    LL_APB1_GRP1_EnableClock(Periph_MAP[index].rcc);
//
//		/* Set counter mode -- Reset value is LL_TIM_COUNTERMODE_UP*/
//		//LL_TIM_SetCounterMode(TIM2, LL_TIM_COUNTERMODE_UP);

//		/* Set the pre-scaler value */
////		prescaler = __LL_TIM_CALC_PSC(SystemCoreClock, 10000);	//调试用
//		LL_TIM_SetPrescaler(timx, prescaler);
//
//		/* Set the auto-reload value to have an initial update event frequency of 10 Hz */
////		period = __LL_TIM_CALC_ARR(SystemCoreClock, LL_TIM_GetPrescaler(timx), 1); //调试用
//		LL_TIM_SetAutoReload(timx, period);
//
//		/* Enable the update interrupt */
//		LL_TIM_EnableIT_UPDATE(timx);
//
//		/* Configure the NVIC to handle TIM2 update interrupt */
//		NVIC_SetPriority((IRQn_Type)Periph_MAP[index].irq , 0);
//		NVIC_EnableIRQ((IRQn_Type)Periph_MAP[index].irq);
//
//		/* Enable counter */
//		LL_TIM_EnableCounter(timx);
//
//		/* Force update generation */
//		LL_TIM_GenerateEvent_UPDATE(timx);
//}

uint32_t E_TIMBase1::GetSourceClock(void){
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

void E_TIMBase1::calculate(uint32_t frq){
	uint32_t timer_clock = GetSourceClock();

	for (prescaler = 1; prescaler <= 0xffff; prescaler++)
	{
		// 计数器频率 = 时钟/分频; 计数个数 = 计数器频率/需要的频率
		period = timer_clock / (prescaler) / frq;
		if ((0xffff >= period) && (period >= 1))
		{
			prescaler--;
			break;
		}
	}
}

E_TIMBase::E_TIMBase(TIM_TypeDef *TIMx,E_PinID id){

	_pin = new E_PinBase(id);
	_index = getIndex(id,TIM_MAP);
	_pin->mode(TIM_MAP[_index]._pin_date,TIM_MAP[_index]._pin_af);
	_timx = TIMx;
	// 初始化通道信息
	switch (TIM_MAP[_index]._periph_base)
	{
	case TIMxCH1:
		_ch = LL_TIM_CHANNEL_CH1;
		_OCsetCompare = &LL_TIM_OC_SetCompareCH1;
		break;
	case TIMxCH2:
		_ch = LL_TIM_CHANNEL_CH2;
		_OCsetCompare = &LL_TIM_OC_SetCompareCH2;
		break;
	case TIMxCH3:
		_ch = LL_TIM_CHANNEL_CH3;
		_OCsetCompare = &LL_TIM_OC_SetCompareCH3;
		break;
	case TIMxCH4:
		_ch = LL_TIM_CHANNEL_CH4;
		_OCsetCompare = &LL_TIM_OC_SetCompareCH4;
		break;
	}
}

void E_TIMBase::init(uint16_t period, uint16_t prescaler){
	
	_index = getPeriphIndex1((uint32_t)_timx,TIM_INFO);
	TIM_INFO[_index]._EnableClock(TIM_INFO[_index]._rcc);
	// 以下代码来自pwm.cpp
	LL_TIM_SetClockSource(_timx,LL_TIM_CLOCKSOURCE_INTERNAL);
	LL_TIM_EnableARRPreload(_timx);
	/* Reset value is LL_TIM_OCPOLARITY_HIGH */
	//LL_TIM_OC_SetPolarity(TIMx, TIM_Channel, oc_polarity);
	//LL_TIM_OC_EnablePreload(_timx, _ch);
	// PWM end
	LL_TIM_SetCounterMode(_timx, LL_TIM_COUNTERMODE_UP);
	LL_TIM_SetPrescaler(_timx, prescaler);
	LL_TIM_SetAutoReload(_timx, period);
}

/**
 *@brief    获取时钟频率
 *@param    NONE
 *@retval   当前TIM频率
*/
uint32_t E_TIMBase::GetSourceClock(void){
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

/**
 *@name     begin(uint32_t frq,uint16_t duty)
 *@brief    启动PWM输出
 *@param    frq  要输出的频率，单位hz. 0-最大频率。
 *		  duty 占空比 0-1000
 *@retval   None
*/
void E_PWM::begin(uint32_t frq,uint16_t duty){
	SetPorlicy(1);
	SetFrequency(frq);
	SetDutyCycle(duty);
}

/**
 *@name     void PWM::SetPorlicy(uint8_t flag)
 *@brief    设置极性
 *@param    flag:  1 HIGH  0 LOW
 *@retval   None
*/
void E_PWM::SetPorlicy(uint8_t porlicy)
{
	LL_TIM_OC_SetPolarity(_timx,_ch,(porlicy == 1)?(LL_TIM_OCPOLARITY_HIGH):(LL_TIM_OCPOLARITY_LOW));
	SetDutyCycle(_duty);
}

/**
 *@name     void PWM::set_frq(uint32_t frq)
 *@brief    设置频率，最大频率可通过GetMaxFrequency()获得
 *@param    frq  要输出的频率，单位hz. 0-最大频率。
 *@retval   None
*/
void E_PWM::SetFrequency(uint32_t frq)
{
	uint32_t period  = 0;		// 周期
	uint32_t prescaler = 1;		// 预分频
	uint32_t ii = GetSourceClock();

	if (frq >= GetMaxFrequency())//控制频率，保证其有1%精度
		frq = GetMaxFrequency();

	//千分之一精度分配方案
	for (; prescaler <= 0xffff; prescaler++)
	{
		period = ii / prescaler / frq;
		if ((0xffff >= period) && (period >= 1000))
		{
			_accuracy = 0;
			break;
		}
	}
	//上述算法分配失败
	if (prescaler == 65536)
	{
		//百分之一分配方案
		for (prescaler = 1; prescaler <= 0xffff; prescaler++)
		{
			period = GetSourceClock() / (prescaler) / frq;
			if ((0xffff >= period) && (period >= 100))
			{
				_accuracy = 1;
				break;
			}
		}
	}
	_period = period;
	
	init(_period, prescaler-1);
	SetPorlicy(1);
	SetDutyCycle(_duty);
}

/**
 *@name     SetDutyCycle(uint16_t duty)
 *@brief    设置PWM占空比.频率小于等于十分之一最大频率是，分辨率为千分之一
 *		   频率在十分之一最大频率和最大频率之间，分辨率为百分之一
 *@param    duty 0-1000 对应0%-100%
 *@retval   none
*/
void E_PWM::SetDutyCycle(uint16_t duty)
{
	uint16_t pulse = 0;
	float percent;		// 百分比

	_duty = duty>1000?1000:duty;
	// 百分之一精度
	if(_accuracy){
		_duty = (_duty<10 && _duty!=0)?10:duty;
	}
	
	percent = (_duty/1000.0);
	DBG("DutyCycle is : %f \r\n",percent);
	
	pulse = (uint16_t) (( percent  * _period ));
	DBG("pulse is : %d \r\n",pulse);
	
	_OCsetCompare(_timx, pulse);
	LL_TIM_CC_EnableChannel(_timx,_ch);
	LL_TIM_OC_SetMode(_timx, _ch, LL_TIM_OCMODE_PWM1);
	LL_TIM_OC_EnablePreload(_timx, _ch);
	/* Enable counter */
	LL_TIM_EnableCounter(_timx);
}

/**
 *@name     GetMaxFrequency
 *@brief    获取PWM最大频率
 *@param    none
 *@retval   最大频率
*/
uint32_t E_PWM::GetMaxFrequency(void)
{
	return GetSourceClock()/100;
}


