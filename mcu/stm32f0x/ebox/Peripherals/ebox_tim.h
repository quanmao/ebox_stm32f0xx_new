/**
  ******************************************************************************
  * @file    ebox_tim.h
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
#ifndef	__EBOX_TIM_H_
#define	__EBOX_TIM_H_

#include "ebox_define.h"
#include "stm32_define.h"
#include "ebox_gpio.h"

class E_TIMBase1{
	public:
		E_TIMBase1(TIM_TypeDef *TIMx);
		
		void calculate(uint32_t frq);
		void init(uint16_t _period, uint16_t _prescaler);
		
		uint32_t GetSourceClock(void);
	
		TIM_TypeDef  *timx;
	  uint32_t 		period;
		uint32_t		prescaler;
};



class E_TIMBase{
	public:
		E_TIMBase(E_PinBase *_pin){pin = _pin;}
		E_TIMBase(TIM_TypeDef *TIMx);
		
		void init(uint16_t period, uint16_t prescaler);
		uint32_t GetSourceClock(void);
	
		uint8_t index;
	  TIM_TypeDef *timx;
		E_PinBase *pin;
};


//class E_TIMER:E_TIMBase{
//	E_TIMER(TIM_TypeDef *TIMx):E_TIMBase(TIMx){};
//		
//		void start(void);
//		void stop(void);		
//		void begin(uint16_t frq);		
//};

//class E_CAPTURE:E_TIMBase{
//	E_CAPTURE(PIN_ID_T pin);
//	
//	void SetPorlicy(uint8_t porlicy);
//	
//	uint32_t GetDetectMaxFrq(void);
//	uint32_t GetDetectMinFrq(void);
//	uint32_t GetTimerClock(void);
//	
//	
//	private:
//		uint32_t channel;
//};

class E_PWM : E_TIMBase{
	public:
		E_PWM(E_PinBase *pin);
		
		void SetPorlicy(uint8_t porlicy);
		void SetFrequency(uint16_t frq);
		void SetDutyCycle(uint16_t duty);		
		
	private:
		uint32_t GetMaxFrequency(void);
		uint32_t channel;
		
		uint16_t period;
	  uint16_t duty;
};



#endif
