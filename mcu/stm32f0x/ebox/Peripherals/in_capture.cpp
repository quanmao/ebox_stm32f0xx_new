/**
  ******************************************************************************
  * @file    incapture.cpp
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
 * cat_li			2016年10月2日
 *      *移植到STM32F0平台
 */

/* Includes ------------------------------------------------------------------*/
#include "in_capture.h"
#include "timer_it.h"

#define TIMxCH1 0x01
#define TIMxCH2 0x02
#define TIMxCH3 0x03
#define TIMxCH4 0x04

extern uint16_t t1_overflow_times ;
extern uint16_t t2_overflow_times ;
extern uint16_t t3_overflow_times ;
extern uint16_t t4_overflow_times ;

/**
 * @brief    InCapture构造函数，实例化一个对象
 * @param    *capture_pin: 指定用来捕获信号用的Pin
 * @return   NONE
 */
InCapture::InCapture(Gpio *capture_pin)
{
    this->capture_pin = capture_pin;
    this->period = 0xffff;
    this->prescaler = 1;
    this->polarity = TIM_ICPOLARITY_FALLING;
    high_capture = 0;
    low_capture = 0;
    _capture = 0;
}

/**
 * @brief    begin，启动捕获
 * @param    prescaler: 分频系数
 * @return   NONE
 */
void InCapture::begin(uint16_t prescaler )
{
	uint8_t index;
	// 初始化TIM基本信息 TIMx,Channel
	init_info(this->capture_pin);
	capture_pin->mode(AF_PP,pin_AFx);

	this->prescaler = prescaler;
	base_init(this->period, this->prescaler);
	timer_clock = get_timer_clock();

	switch (ch)
	{
	case TIMxCH1:
		_get_capture = LL_TIM_IC_GetCaptureCH1;
//        _set_polarity = TIM_OC1PolarityConfig;
		break;
	case TIMxCH2:
		_get_capture = LL_TIM_IC_GetCaptureCH2;
//        _set_polarity = TIM_OC2PolarityConfig;
		break;
	case TIMxCH3:
		_get_capture = LL_TIM_IC_GetCaptureCH3;
//        _set_polarity = TIM_OC3PolarityConfig;
		break;
	case TIMxCH4:
		_get_capture = LL_TIM_IC_GetCaptureCH4;
//        _set_polarity = TIM_OC4PolarityConfig;
		break;
	}
    switch((uint32_t)TIMx)
    {
    case (uint32_t)TIM1_BASE:
	overflow_times = &t1_overflow_times;
				if(ch== TIMxCH1) index = TIM1_IT_CC1;
				if(ch== TIMxCH2) index = TIM1_IT_CC2;
				if(ch== TIMxCH3) index = TIM1_IT_CC3;
				if(ch== TIMxCH4) index = TIM1_IT_CC4;
        break;
    case (uint32_t)TIM2_BASE:
        overflow_times = &t2_overflow_times;
				if(ch== TIMxCH1) index = TIM2_IT_CC1;
				if(ch== TIMxCH2) index = TIM2_IT_CC2;
				if(ch== TIMxCH3) index = TIM2_IT_CC3;
				if(ch== TIMxCH4) index = TIM2_IT_CC4;
        break;
    case (uint32_t)TIM3_BASE:
        overflow_times = &t3_overflow_times;
				if(ch== TIMxCH1) index = TIM3_IT_CC1;
				if(ch== TIMxCH2) index = TIM3_IT_CC2;
				if(ch== TIMxCH3) index = TIM3_IT_CC3;
				if(ch== TIMxCH4) index = TIM3_IT_CC4;
        break;
//    case (uint32_t)TIM4_BASE:
//        overflow_times = &t4_overflow_times;
//				if(ch== TIMxCH1) index = TIM4_IT_CC1;
//				if(ch== TIMxCH2) index = TIM4_IT_CC2;
//				if(ch== TIMxCH3) index = TIM4_IT_CC3;
//				if(ch== TIMxCH4) index = TIM4_IT_CC4;
//        break;
	}
	//index = TIM1_IT_CC1;
	tim_irq_init(index,(&InCapture::_irq_handler),(uint32_t)this);
}

/**
 * @brief    base_init，TIM初始化
 * @param    period: 周期
 * @param    prescaler: 分频系数
 * @return   NONE
 */
void InCapture::base_init(uint16_t period, uint16_t prescaler)
{
	this->period = period;//更新period
	this->prescaler = prescaler;//更新prescaler

	switch ((uint32_t)this->TIMx)
	{
	case (uint32_t)TIM1_BASE:
		LL_APB1_GRP2_EnableClock(LL_APB1_GRP2_PERIPH_TIM1);
		NVIC_SetPriority(TIM1_CC_IRQn, 0);
		NVIC_EnableIRQ(TIM1_CC_IRQn);
		NVIC_EnableIRQ(TIM1_BRK_UP_TRG_COM_IRQn);
		break;
#ifdef TIM2
    case (uint32_t)TIM2_BASE:
		LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_TIM2);
		NVIC_SetPriority(TIM2_IRQn, 0);
		NVIC_EnableIRQ(TIM2_IRQn);
		break;
#endif
#ifdef TIM3
    case (uint32_t)TIM3_BASE:
		LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_TIM3);
		NVIC_SetPriority(TIM3_IRQn, 0);
		NVIC_EnableIRQ(TIM3_IRQn);
        break;
#endif
    }
// 设定TIM溢出参数
	LL_TIM_SetPrescaler(TIMx,prescaler-1);
	LL_TIM_SetAutoReload(TIMx,period - 1);
	LL_TIM_SetCounterMode(TIMx,LL_TIM_COUNTERDIRECTION_UP);

//	LL_TIM_ClearFlag_UPDATE(TIMx); //必须加，否则开启中断会立即产生一次中断
	LL_TIM_EnableIT_UPDATE(TIMx);
// 设定TIM输入捕获信息
	/************************************/
	/* Input capture mode configuration */
	/************************************/
	/* Select the active input: IC1 = TI1FP1 */
	LL_TIM_IC_SetActiveInput(TIMx, channel, LL_TIM_ACTIVEINPUT_DIRECTTI);
	/* Configure the input filter duration: no filter needed */
	LL_TIM_IC_SetFilter(TIMx, channel, LL_TIM_IC_FILTER_FDIV1);
	/* Set input prescaler: prescaler is disabled */
	LL_TIM_IC_SetPrescaler(TIMx, channel, LL_TIM_ICPSC_DIV1);
	/* Select the edge of the active transition on the TI1 channel: rising edge */
	LL_TIM_IC_SetPolarity(TIMx, channel, polarity);
	/**************************/
	/* TIM1 interrupts set-up */
	/**************************/
	/* Enable the capture/compare interrupt for channel 1 */
	LL_TIM_EnableIT_CC1(TIMx);
	/***********************/
	/* Start input capture */
	/***********************/
	/* Enable output channel 1 */
	LL_TIM_CC_EnableChannel(TIMx, channel);

	/* Enable counter */
	LL_TIM_EnableCounter(TIMx);
	//LL_TIM_GenerateEvent_UPDATE(TIMx);
}

/**
 * @brief    init_info 初始化信息
 * @param    *capture_pin: 指定用来捕获信号用的Pin
 * @return   NONE
 */
void InCapture::init_info(Gpio *capture_pin)
{
	switch (capture_pin->id)
	{
#ifdef TIM2
	case PA0_ID:
		TIMx = TIM2;
		ch = TIMxCH1;
		channel = LL_TIM_CHANNEL_CH1;
		break;
	case PA1_ID:
		TIMx = TIM2;
		ch = TIMxCH2;
		channel = LL_TIM_CHANNEL_CH2;
		break;
	case PA2_ID:
		TIMx = TIM2;
		ch = TIMxCH3;
		channel = LL_TIM_CHANNEL_CH3;
		break;
	case PA3_ID:
		TIMx = TIM2;
		ch = TIMxCH4;
		channel = LL_TIM_CHANNEL_CH4;
		break;
#endif
#ifdef TIM3
	case PA6_ID:
		TIMx = TIM3;
		ch = TIMxCH1;
		channel = LL_TIM_CHANNEL_CH1;
		pin_AFx = LL_GPIO_AF_1;
		break;
	case PA7_ID:
		TIMx = TIM3;
		ch = TIMxCH2;
		channel = LL_TIM_CHANNEL_CH2;
		pin_AFx = LL_GPIO_AF_1;
		break;
	case PB0_ID:
		TIMx = TIM3;
		ch = TIMxCH3;
		channel = LL_TIM_CHANNEL_CH3;
		pin_AFx = LL_GPIO_AF_1;
		break;
	case PB1_ID:
		TIMx = TIM3;
		ch = TIMxCH4;
		channel = LL_TIM_CHANNEL_CH4;
		pin_AFx = LL_GPIO_AF_1;
	case PB4_ID:
		TIMx = TIM3;
		ch = TIMxCH3;
		channel = LL_TIM_CHANNEL_CH1;
		pin_AFx = LL_GPIO_AF_1;
		break;
	case PB5_ID:
		TIMx = TIM3;
		ch = TIMxCH4;
		channel = LL_TIM_CHANNEL_CH2;
		pin_AFx = LL_GPIO_AF_1;
#endif
#ifdef TIM1
	case PA8_ID:
		TIMx = TIM1;
		ch = TIMxCH1;
		channel = LL_TIM_CHANNEL_CH1;
		pin_AFx = LL_GPIO_AF_2;
		break;
	case PA9_ID:
		TIMx = TIM1;
		ch = TIMxCH2;
		channel = LL_TIM_CHANNEL_CH2;
		pin_AFx = LL_GPIO_AF_2;
		break;
	case PA10_ID:
		TIMx = TIM1;
		ch = TIMxCH3;
		channel = LL_TIM_CHANNEL_CH3;
		pin_AFx = LL_GPIO_AF_2;
		break;
	case PA11_ID:
		TIMx = TIM1;
		ch = TIMxCH4;
		channel = LL_TIM_CHANNEL_CH4;
		pin_AFx = LL_GPIO_AF_2;
		break;
#endif
	}
}

void InCapture::set_polarity_falling()
{
    this->polarity = TIM_ICPOLARITY_FALLING;
    //_set_polarity(this->TIMx, this->polarity);//设置为下降沿捕获
	  LL_TIM_IC_SetPolarity(TIMx, channel, this->polarity);

}
void InCapture::set_polarity_rising()
{
    this->polarity = TIM_ICPOLARITY_RISING;
    //_set_polarity(this->TIMx, this->polarity);//设置为下降沿捕获
	  LL_TIM_IC_SetPolarity(TIMx, channel, this->polarity);

}
uint32_t InCapture::get_capture()
{
    uint32_t    now = 0;
    now = _get_capture( this->TIMx ) + (*overflow_times) * this->period;  //get capture value
    if(now > last_value)
        _capture = now - last_value;
    else
        _capture = 0xffffffff + now - last_value;
    last_value = now;
    return _capture;    
}

float InCapture::get_zone_time_us()
{
    return get_capture() * 1000000.0 / timer_clock;
}



void InCapture::complex_event()
{
    uint32_t    capture = 0;
    uint32_t    now = 0;
    now = _get_capture( TIMx ) + (*overflow_times) * this->period;  //get capture value
    if(now > last_value)
        capture = now - last_value;
    else if(now < last_value)
        capture = 0xffffffff + now - last_value;
	else{
		_available = false;
		return ;
	}
    last_value = now;
    
    if(polarity == TIM_ICPOLARITY_FALLING)//检测到下降沿，测量高电平时间完成
    {
        high_capture = capture;
        set_polarity_rising();//切换检测上升沿
    }
    else
    {
        low_capture = capture;
        set_polarity_falling();//切换检测下降沿
    }
    if((high_capture&high_capture) != 0)
        _available = true;
    
}
void InCapture::simple_event()
{
    uint32_t    now = 0;
    now = _get_capture(TIMx) + (*overflow_times) * this->period;  //get capture value
    if(now > last_value)
        _capture = now - last_value;
    else
        _capture = 0xffffffff + now - last_value;
    last_value = now;
    _available = true; 		
}

bool InCapture::available()
{
    return _available;
}

float InCapture::get_wave_frq()
{
//	  uint32_t TIM1CLK;
//  uint32_t PSC;
//  uint32_t IC1PSC;
//  uint32_t IC1Polarity;
    _available = false;
    if(_capture == 0)
        return  (timer_clock/((high_capture + low_capture)));
    else
        return (timer_clock/_capture);
	    /* The signal frequency is calculated as follows:                         */      
    /* Frequency = (TIM1*IC1PSC) / (Capture*(PSC+1)*IC1Polarity)           */
    /* where:                                                                 */                                                          
    /*  Capture is the difference between two consecutive captures            */
    /*  TIM1CLK is the timer counter clock frequency                           */
    /*  PSC is the timer prescaler value                                      */
    /*  IC1PSC is the input capture prescaler value                           */
    /*  IC1Polarity value depends on the capture sensitivity:                 */
    /*    1 if the input is sensitive to rising or falling edges              */
    /*    2 if the input is sensitive to both rising and falling edges        */
    
    /* Retrieve actual TIM1 counter clock frequency */
//    TIM1CLK = timer_clock;
//    
//    /* Retrieve actual TIM1 prescaler value */
//    PSC = LL_TIM_GetPrescaler(TIM1);    
//    /* Retrieve actual IC1 prescaler ratio */
//    IC1PSC = __LL_TIM_GET_ICPSC_RATIO(LL_TIM_IC_GetPrescaler(TIM1, LL_TIM_CHANNEL_CH1));   
//    /* Retrieve actual IC1 polarity setting */
//    if (LL_TIM_IC_GetPolarity(TIM1, LL_TIM_CHANNEL_CH1) == LL_TIM_IC_POLARITY_BOTHEDGE)
//      IC1Polarity = 2;
//    else
//      IC1Polarity = 1;
//    
//    /* Calculate input signal frequency */
//    fre = (TIM1CLK *IC1PSC) / (peri*(PSC+1)*IC1Polarity);
//    //peri = peri*1000000.0/TIM1CLK;
//	  return fre;
}
float InCapture::get_wave_peroid()
{
    _available = false;
    if(_capture == 0)
        return  ((high_capture + low_capture)*1000000.0/(timer_clock));
    else
        return  (_capture/(timer_clock/1000000.0));
//	return (_capture*1000000.0/timer_clock);
}

float InCapture::get_wave_high_duty()
{
    _available = false;
    if(_capture == 0)
       return  (high_capture*100.0/(high_capture + low_capture));
    else
        return 0;
}
float InCapture::get_wave_low_duty()
{
    _available = false;
    if(_capture == 0)
        return  (low_capture*100.0/(high_capture + low_capture));
    else
        return  0;
}

float InCapture::get_wave_high_time()
{
    _available = false;
    if(_capture == 0)
        return  ((high_capture )*1000000.0/(timer_clock));
    else
        return  (_capture*1000000.0/(timer_clock));
}
float InCapture::get_wave_low_time()
{
    _available = false;
    if(_capture == 0)
        return  ((low_capture)*1000000.0/(timer_clock));
    else
        return  (_capture*1000000.0/(timer_clock));
}


void InCapture::set_count(uint16_t count)
{
    //TIM_SetCounter(this->TIMx, count); //reset couter
	  LL_TIM_SetAutoReload(TIMx,count);
}

uint32_t InCapture::get_timer_clock()
{
    return get_timer_source_clock()/this->prescaler;
}

uint32_t InCapture::get_timer_source_clock()
{
    //uint32_t temp = 0;
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
uint32_t InCapture::get_detect_max_frq()
{
    
    switch(get_timer_source_clock())
    {
        case 72000000:
            return 180000;
        case 8000000:
            return 18000;
        default:
            return 0;
    }

}
uint32_t InCapture::get_detect_min_frq()
{
    return 0;
}

uint8_t InCapture::get_detect_min_pulse_us()
{
    switch(get_timer_source_clock())
    {
        case 72000000:
            return 4;
        case 8000000:
            return 30;
        default:
            return 0;
    }
}


void InCapture::_irq_handler( uint32_t id){ 
		InCapture *handler = (InCapture*)id;
		handler->_irq.call();

}


void InCapture::attach(void (*fptr)(void)) {
    if (fptr) {
        _irq.attach(fptr);
		}
}

