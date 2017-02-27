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

E_TIMBase1::E_TIMBase1(TIM_TypeDef *TIMx){		
		timx = TIMx;
}

void E_TIMBase1::init(uint16_t _period, uint16_t _prescaler){

		uint8_t index = PeriphIndex((uint32_t)timx,Periph_MAP);
    /* Enable the timer peripheral clock */
    LL_APB1_GRP1_EnableClock(Periph_MAP[index].rcc); 
  
		/* Set counter mode -- Reset value is LL_TIM_COUNTERMODE_UP*/
		//LL_TIM_SetCounterMode(TIM2, LL_TIM_COUNTERMODE_UP);

		/* Set the pre-scaler value */
//		prescaler = __LL_TIM_CALC_PSC(SystemCoreClock, 10000);	//������
		LL_TIM_SetPrescaler(timx, prescaler);
		
		/* Set the auto-reload value to have an initial update event frequency of 10 Hz */
//		period = __LL_TIM_CALC_ARR(SystemCoreClock, LL_TIM_GetPrescaler(timx), 1); //������
		LL_TIM_SetAutoReload(timx, period);
  
		/* Enable the update interrupt */
		LL_TIM_EnableIT_UPDATE(timx);
  
		/* Configure the NVIC to handle TIM2 update interrupt */
		NVIC_SetPriority((IRQn_Type)Periph_MAP[index].irq , 0);
		NVIC_EnableIRQ((IRQn_Type)Periph_MAP[index].irq);
		
		/* Enable counter */
		LL_TIM_EnableCounter(timx);
		
		/* Force update generation */
		LL_TIM_GenerateEvent_UPDATE(timx);
}

uint32_t E_TIMBase1::GetSourceClock(void){
	    uint32_t timer_clock = 0x00;
    
  	// ����TIMʱ��Ƶ��
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
		// ������Ƶ�� = ʱ��/��Ƶ; �������� = ������Ƶ��/��Ҫ��Ƶ��
		period = timer_clock / (prescaler) / frq;		
		if ((0xffff >= period) && (period >= 1))
		{
			prescaler--;
			break;
		}
	}
}

E_TIMBase::E_TIMBase(TIM_TypeDef *TIMx){		
		timx = TIMx;
}

void E_TIMBase::init(uint16_t period, uint16_t prescaler){
	 index = PeriphIndex((uint32_t)timx,Periph_MAP);
	 LL_APB1_GRP1_EnableClock(Periph_MAP[index].rcc);
	 // ���´�������pwm.cpp
	 LL_TIM_SetClockSource(timx,LL_TIM_CLOCKSOURCE_INTERNAL);	
	 LL_TIM_EnableARRPreload(timx);
		/* Reset value is LL_TIM_OCPOLARITY_HIGH */
		//LL_TIM_OC_SetPolarity(TIMx, TIM_Channel, oc_polarity);
	 	LL_TIM_OC_EnablePreload(timx, PinCH(pin->id,PWM_MAP));
	 // PWM end
   LL_TIM_SetCounterMode(timx, LL_TIM_COUNTERMODE_UP);
   LL_TIM_SetPrescaler(timx, prescaler);  
   LL_TIM_SetAutoReload(timx, period);
	
}

uint32_t E_TIMBase::GetSourceClock(void){
    uint32_t timer_clock = 0x00;
    
  	// ����TIMʱ��Ƶ��
	if (LL_RCC_GetAPB1Prescaler() == 0)
	{
		timer_clock = cpu.clock.pclk1;
	}else{
		timer_clock = cpu.clock.pclk1*2;
	} 
    return timer_clock;
}

E_PWM::E_PWM(E_PinBase *pin):E_TIMBase(pin){
		pin->mode(AF_PP,PinAlternate(pin->id,PWM_MAP));		
}

/**
 *@name     void PWM::set_oc_polarity(uint8_t flag)
 *@brief    ���ü���
 *@param    flag:  1 HIGH  0 LOW
 *@retval   None
*/
void E_PWM::SetPorlicy(uint8_t porlicy)
{
	LL_TIM_OC_SetPolarity(timx,PinCH(pin->id,PWM_MAP),(porlicy == 1)?(LL_TIM_OCPOLARITY_HIGH):(LL_TIM_OCPOLARITY_LOW));
	SetDutyCycle(duty);
}

/**
 *@name     void PWM::set_frq(uint32_t frq)
 *@brief    ����Ƶ�ʣ�Ҫ�趨��Ƶ�ʼ��� period(����) prescaler��Ԥ��Ƶ��
 *@param    frq  Ҫ�����Ƶ��
 *@retval   None
*/
void E_PWM::SetFrequency(uint16_t frq)
{
	uint16_t period  = 0;		// ����
	uint32_t prescaler = 1;		// Ԥ��Ƶ
	
	duty = 500;	
	timx = (TIM_TypeDef*)PWM_MAP[PinIndex(pin->id,PWM_MAP)].periph;

	if (frq >= GetMaxFrequency())//����Ƶ�ʣ���֤����1%����
		frq = GetMaxFrequency();

	//ǧ��֮һ���ȷ��䷽��
	for (; prescaler <= 0xffff; prescaler++)
	{
		period = GetSourceClock() / (prescaler) / frq;
		if ((0xffff >= period) && (period >= 1000))
		{
//			accuracy = 1;
			break;
		}
	}

	if (prescaler == 65536)//�����㷨����ʧ��
	{
		//�ٷ�֮һ���䷽��
		for (prescaler = 1; prescaler <= 0xffff; prescaler++)
		{
			period = GetSourceClock() / (prescaler) / frq;
			if ((0xffff >= period) && (period >= 100))
			{
//				accuracy = 2;
				break;
			}
		}
	}
  this ->period = period;
	init(period, prescaler-1);
	SetPorlicy(1);
  SetDutyCycle(duty);
}

void E_PWM::SetDutyCycle(uint16_t duty)
{
		uint16_t pulse = 0;
	float percent;

	if (this->duty > 1000)
		this->duty = 1000;
	
	percent = this->duty / 1000.0;
	pulse = (uint16_t) (( percent  * period ));

	switch (PinCH(pin->id,PWM_MAP))
	{
	case LL_TIM_CHANNEL_CH1:
		LL_TIM_OC_SetCompareCH1(timx, pulse);
		LL_TIM_CC_EnableChannel(timx,LL_TIM_CHANNEL_CH1);
	  LL_TIM_OC_SetMode(timx, LL_TIM_CHANNEL_CH1, LL_TIM_OCMODE_PWM1);
		break;
	case LL_TIM_CHANNEL_CH2:
		LL_TIM_OC_SetCompareCH2(timx, pulse);
		break;
	case LL_TIM_CHANNEL_CH3:
		LL_TIM_OC_SetCompareCH3(timx, pulse);
		break;
	case LL_TIM_CHANNEL_CH4:
		LL_TIM_OC_SetCompareCH4(timx, pulse);
		break;
	}
	/**********************************/
	/* Start output signal generation */
	/**********************************/
	/* Enable output channel 1 */
	//LL_TIM_CC_EnableChannel(timx, PinCH(pin->id,PWM_MAP));
	/* Enable counter */
	LL_TIM_EnableCounter(timx);
}

uint32_t E_PWM::GetMaxFrequency(void)
{
    return GetSourceClock()/100;
}


