/**
  ******************************************************************************
  * @file    ebox_tim.h
  * @author  cat_li
  * @version V2.0
  * @date    2016/10/23
  * @brief
	*		2017/7/15 cat_li
			1	E_PWM 基本完成，TIM3验证OK
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
// 函数指针,指向LL_TIM_OC_SetCompareCH4(TIM_TypeDef *TIMx, uint32_t CompareValue) 函数
typedef void (*pfun)(TIM_TypeDef *,uint32_t);

class E_TIMBase1{
public:
	E_TIMBase1(TIM_TypeDef *TIMx);

	void calculate(uint32_t frq);
	void init(uint16_t _period, uint16_t _prescaler);

	uint32_t GetSourceClock(void);

	TIM_TypeDef  *timx;
	uint32_t 	 period;
	uint32_t	 prescaler;
};



class E_TIMBase{
public:
	E_TIMBase(TIM_TypeDef *TIMx,E_PinID id);
	E_TIMBase(TIM_TypeDef *TIMx);

	void init(uint16_t period, uint16_t prescaler);
	uint32_t GetSourceClock(void);
protected:
	uint32_t	_ch;		//通道
	TIM_TypeDef *_timx;		//tim
  pfun  _OCsetCompare;
private:
	uint8_t 	_index;
	E_PinBase 	*_pin;
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

class E_PWM:E_TIMBase{
public:
	E_PWM(TIM_TypeDef *TIMx,E_PinID pin):E_TIMBase(TIMx,pin){
	}
	void begin(uint32_t frq,uint16_t duty);
	
	void SetPorlicy(uint8_t porlicy);
	void SetFrequency(uint32_t frq);
	void SetDutyCycle(uint16_t duty);
	
	uint32_t GetMaxFrequency(void);

private:
	uint16_t _period;	// 周期
	uint16_t _duty;		// 占空比
	uint8_t	 _accuracy; // 精度	
};

#endif
