/**
  ******************************************************************************
  * @file    pwm.cpp
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
 * -shentq                  -version 1.2(2016/08/17)
 *      *修复了set_duty时导致波形输出畸形
 *      *修复了芯片主频变动导致定时器PWM频率变化的bug，支持任意主频下正常工作
 *      *增加了get_timer_clock(),get_max_frq(),get_accuracy()三个函数接口
 * -LQM (2016/9/1)
 *      *移植到STM32F0,基于HAL库LL层
 */
/* Includes ------------------------------------------------------------------*/
#include "pwm.h"

#define TIMxCH1 0x01
#define TIMxCH2 0x02
#define TIMxCH3 0x03
#define TIMxCH4 0x04



Pwm::Pwm(Gpio *pwm_pin)
{
    //	if(isPwmPin(PWMpin))
    //	{
    this->pwm_pin = pwm_pin;
    //	}
}
void Pwm::begin(uint32_t frq, uint16_t duty)
{
	this->duty = duty;

	init_info(pwm_pin);
	pwm_pin->mode(AF_PP,af_configration);

	set_oc_polarity(1);
	set_frq(frq);
	set_duty(duty);
}

/**
 *@name     void PWM::base_init(uint16_t period, uint16_t prescaler)
 *@brief    TIM PWM模式输出基本设置
 *@param    period:  周期
 *		   prescaler: 预分频
 *@retval   None
*/
void Pwm::base_init(uint16_t period, uint16_t prescaler)
{
	this->period = period;//更新period

	/* Enable the timer peripheral clock */
	LL_APB1_GRP1_EnableClock(rcc);
    /***************************/
	/* Time base configuration */
	/***************************/
	/* Set counter mode */
	/* Reset value is LL_TIM_COUNTERMODE_UP */
	//LL_TIM_SetCounterMode(TIM3, LL_TIM_COUNTERMODE_UP);
	/* (1)select clock Source */
	LL_TIM_SetClockSource(TIMx,LL_TIM_CLOCKSOURCE_INTERNAL);

	/* Set the pre-scaler value to have TIM3 counter clock equal to 10 kHz */
	LL_TIM_SetPrescaler(TIMx, prescaler);

	/* Enable TIM2_ARR register preload. Writing to or reading from the         */
	/* auto-reload register accesses the preload register. The content of the   */
	/* preload register are transferred into the shadow register at each update */
	/* event (UEV).                                                             */
	LL_TIM_EnableARRPreload(TIMx);

	/* Set the auto-reload value to have a counter frequency of 100 Hz */
	//LL_TIM_SetAutoReload(TIMx, __LL_TIM_CALC_ARR(SystemCoreClock, LL_TIM_GetPrescaler(TIMx), 100));
	LL_TIM_SetAutoReload(TIMx, period);

	/*********************************/
	/* Output waveform configuration */
	/*********************************/
	/* Set output mode */
	/* Reset value is LL_TIM_OCMODE_FROZEN */
	LL_TIM_OC_SetMode(TIMx, TIM_Channel, LL_TIM_OCMODE_PWM1);
	/* Set output channel polarity */
	/* Reset value is LL_TIM_OCPOLARITY_HIGH */
	LL_TIM_OC_SetPolarity(TIMx, TIM_Channel, oc_polarity);

	// /* Set compare value to half of the counter period (50% duty cycle ) */
	// LL_TIM_OC_SetCompareCH2(TIMx, pulse);

	/* Enable TIM2_CCR1 register preload. Read/Write operations access the      */
	/* preload register. TIM2_CCR1 preload value is loaded in the active        */
	/* at each update event.                                                    */
	LL_TIM_OC_EnablePreload(TIMx, TIM_Channel);

}

void Pwm::init_info(Gpio *pwm_pin)
{
	switch (pwm_pin->id)
	{
//#ifdef TIM2
//		case PA1_ID:
//			TIMx = TIM2;
//			af_configration = LL_GPIO_AF_2;
//			rcc = LL_APB1_GRP1_PERIPH_TIM2;
//			ch = TIMxCH2;//irq = TIM2_IRQn;
//			TIM_Channel = LL_TIM_CHANNEL_CH2;
//			break;
//		case PA2_ID:
//			TIMx = TIM2;
//			af_configration = LL_GPIO_AF_2;
//			rcc = LL_APB1_GRP1_PERIPH_TIM2;
//			ch = TIMxCH3;//irq = TIM2_IRQn;
//			TIM_Channel = LL_TIM_CHANNEL_CH3;
//			break;	
//		case PA3_ID:
//			TIMx = TIM2;
//			af_configration = LL_GPIO_AF_2;
//			rcc = LL_APB1_GRP1_PERIPH_TIM2;
//			ch = TIMxCH4;//irq = TIM2_IRQn;
//			TIM_Channel = LL_TIM_CHANNEL_CH4;
//			break;
//#endif
#ifdef TIM14
		case PA4_ID:  //TIM14CH1
			TIMx = TIM14;
			af_configration = LL_GPIO_AF_4;
			rcc = LL_APB1_GRP1_PERIPH_TIM14;
			ch = TIMxCH1;
			TIM_Channel = LL_TIM_CHANNEL_CH1;
			break;
#endif
#ifdef TIM3
		case PA6_ID:	//TIM3CH1
			TIMx = TIM3;
			af_configration = LL_GPIO_AF_1;
			rcc = LL_APB1_GRP1_PERIPH_TIM3;
			ch = TIMxCH1;
			TIM_Channel = LL_TIM_CHANNEL_CH1;
			break;
		case PA7_ID:	//TIM3CH2
			TIMx = TIM3;
			af_configration = LL_GPIO_AF_1;
			rcc = LL_APB1_GRP1_PERIPH_TIM3;
			ch = TIMxCH2;
			TIM_Channel = LL_TIM_CHANNEL_CH2;
			break;
		case PB0_ID:	//TIM3CH3
			TIMx = TIM3;
			af_configration = LL_GPIO_AF_1;
			rcc = LL_APB1_GRP1_PERIPH_TIM3;
			ch = TIMxCH3;
			TIM_Channel = LL_TIM_CHANNEL_CH3;
			break;
		case PB1_ID:	//TIM3CH4
			TIMx = TIM3;
			af_configration = LL_GPIO_AF_1;
			rcc = LL_APB1_GRP1_PERIPH_TIM3;
			ch = TIMxCH4;
			TIM_Channel = LL_TIM_CHANNEL_CH4;
			break;
		case PB4_ID:	//TIM3CH1
			TIMx = TIM3;
			af_configration = LL_GPIO_AF_1;
			rcc = LL_APB1_GRP1_PERIPH_TIM3;
			ch = TIMxCH1;//
			TIM_Channel = LL_TIM_CHANNEL_CH1;
			break;
		case PB5_ID:	//TIM3CH2
			TIMx = TIM3;
			af_configration = LL_GPIO_AF_1;
			rcc = LL_APB1_GRP1_PERIPH_TIM3;
			ch = TIMxCH2;
			TIM_Channel = LL_TIM_CHANNEL_CH2;
			break;
			default:
		break;
		}			
#endif	
}

/**
 *@name     void PWM::set_oc_polarity(uint8_t flag)
 *@brief    设置极性
 *@param    flag:  1 HIGH  0 LOW
 *@retval   None
*/
void Pwm::set_oc_polarity(uint8_t flag)
{
	if (flag == 1)
		this->oc_polarity = LL_TIM_OCPOLARITY_HIGH;
	else if (flag == 0)
		this->oc_polarity = LL_TIM_OCPOLARITY_LOW;
	/* Set output channel polarity */
	/* Reset value is LL_TIM_OCPOLARITY_HIGH */
	LL_TIM_OC_SetPolarity(TIMx, TIM_Channel, oc_polarity);
	set_duty(duty);
}


/**
 *@name     void PWM::set_frq(uint32_t frq)
 *@brief    设置频率，要设定的频率计算 period(周期) prescaler（预分频）
 *@param    frq  要输出的频率
 *@retval   None
*/
void Pwm::set_frq(uint32_t frq)
{
	uint32_t period  = 0;		// 周期
	uint32_t prescaler = 1;		// 预分频

	if (frq >= get_max_frq())//控制频率，保证其有1%精度
		frq = get_max_frq();

	//千分之一精度分配方案
	for (; prescaler <= 0xffff; prescaler++)
	{
		period = get_timer_source_clock() / (prescaler) / frq;
		if ((0xffff >= period) && (period >= 1000))
		{
			accuracy = 1;
			break;
		}
	}

	if (prescaler == 65536)//上述算法分配失败
	{
		//百分之一分配方案
		for (prescaler = 1; prescaler <= 0xffff; prescaler++)
		{
			period = get_timer_source_clock() / (prescaler) / frq;
			if ((0xffff >= period) && (period >= 100))
			{
				accuracy = 2;
				break;
			}
		}
	}
	base_init(period, prescaler-1);
  set_duty(duty);

}


//duty:0-1000对应0%-100.0%
void Pwm::set_duty(uint16_t duty)
{
	this->duty = duty;

	uint16_t pulse = 0;
	float percent;

	if (this->duty > 1000)
		this->duty = 1000;
	
	percent = this->duty / 1000.0;
	pulse = (uint16_t) (( percent  * period ));

	switch (ch)
	{
	case TIMxCH1:
		LL_TIM_OC_SetCompareCH1(TIMx, pulse);
		break;
	case TIMxCH2:
		LL_TIM_OC_SetCompareCH2(TIMx, pulse);
		break;
	case TIMxCH3:
		LL_TIM_OC_SetCompareCH3(TIMx, pulse);
		break;
	case TIMxCH4:
		LL_TIM_OC_SetCompareCH4(TIMx, pulse);
		break;
	}
	/**********************************/
	/* Start output signal generation */
	/**********************************/
	/* Enable output channel 1 */
	LL_TIM_CC_EnableChannel(TIMx, TIM_Channel);
	/* Enable counter */
	LL_TIM_EnableCounter(TIMx);
}



uint32_t Pwm::get_timer_source_clock()
{
    uint32_t timer_clock = 0x00;
    
  	// 计算TIM时钟频率
	if (LL_RCC_GetAPB1Prescaler() == 0)
	{
		//timer_clock = SystemCoreClock;
		timer_clock = cpu.clock.pclk1;
	}else{
		// ((SystemCoreClock/AHBDiv)/APBDiv)*2,即PCLK*2
		//timer_clock = (__LL_RCC_CALC_PCLK1_FREQ(SystemCoreClock/((LL_RCC_GetAHBPrescaler()>>8)+1),LL_RCC_GetAPB1Prescaler()))*2;
		timer_clock = cpu.clock.pclk1*2;
	} 
    return timer_clock;
}


uint32_t Pwm::get_max_frq()
{
    return get_timer_source_clock()/100;
}

float Pwm::get_accuracy()
{
    
    switch(accuracy)
    {
        case 0:
            return 0;
        case 1:
            return 0.001;
        case 2:
            return 0.01;

    }
    return 0.001;


}


//duty:0-1000对应0%-100.0%
void analog_write(Gpio *pwm_pin, uint16_t duty)
{
    //	if(isPwmPin(PWMpin))
    //	{
    Pwm p(pwm_pin);
    p.begin(1000, duty);
    //p.SetFrq(1000,1);
    //			p._set_duty(duty);

    //	}
    //	else
    //	{
    //	;
    //	}
}

//////////////////////////////////////////////////////////////
uint8_t isPwmPin(uint8_t pin)
{

    return 0;
}

