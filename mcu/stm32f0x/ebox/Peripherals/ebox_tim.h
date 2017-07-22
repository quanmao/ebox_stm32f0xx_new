/**
  ******************************************************************************
  * @file    ebox_tim.h
  * @author  cat_li
  * @version V2.0
  * @date    2016/10/23
  * @brief
	*		2017/7/15 cat_li
			1	E_PWM ������ɣ�TIM3��֤OK
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
#include "ebox_Template.h"

// ����ָ��,ָ��LL_TIM_OC_SetCompareCH4(TIM_TypeDef *TIMx, uint32_t CompareValue) ����
typedef void (*pfun)(TIM_TypeDef *,uint32_t);
typedef uint32_t (*pGetFun)(TIM_TypeDef *);


class E_base{
public:
	E_base(TIM_TypeDef *TIMx);
	void calculate(uint32_t frq);
	void init();
	void setCountMode(uint32_t CounterMode);
	
	uint32_t GetSourceClock(void);
protected:	
	TIM_TypeDef  *_timx;		// ͨ��
	uint8_t		 _tIndex;		// TIM����
	uint32_t 	 _period;		// ����
	uint32_t	 _prescaler;	// ��Ƶ

	void _start();
	void _setPerPsc();
  void _setMode(void);
	void _EnInterrupt();
};

class E_TIME:E_base{
public:
	E_TIME(TIM_TypeDef *TIMx):E_base(TIMx){};
	void setFrequency(uint32_t frq);
	void start();
	void stop();
	
	uint32_t GetMaxFrequency(void);
		
//	void attach(void (*fptr)(void));	
//	template<typename T>
//	void attach(T* tptr, void (T::*mptr)(void)){
//			_pirq.attach(tptr, mptr);
//	}
private:
	FunctionPointer _pirq;
	void _setMode(void);
};

class E_PWM:E_base{
public:
	E_PWM(TIM_TypeDef *TIMx,E_PinID id):E_base(TIMx){
		uint8_t _index;
		E_PinBase *_pin;
		_pin = new E_PinBase(id);
		_index = getIndex(id,TIM_MAP);
		_pin->mode(TIM_MAP[_index]._pin_date,TIM_MAP[_index]._pin_af);
		_timx = TIMx;
		

		switch (TIM_MAP[_index]._periph_base)
		{
		case TIMxCH1:
			_channel = LL_TIM_CHANNEL_CH1;
			_OCsetCompare = &LL_TIM_OC_SetCompareCH1;
			break;
		case TIMxCH2:
			_channel = LL_TIM_CHANNEL_CH2;
			_OCsetCompare = &LL_TIM_OC_SetCompareCH2;
			break;
		case TIMxCH3:
			_channel = LL_TIM_CHANNEL_CH3;
			_OCsetCompare = &LL_TIM_OC_SetCompareCH3;
			break;
		case TIMxCH4:
			_channel = LL_TIM_CHANNEL_CH4;
			_OCsetCompare = &LL_TIM_OC_SetCompareCH4;
			break;
		}
	}
	void begin(uint32_t frq,uint16_t duty);

	void SetPorlicy(uint8_t porlicy);
	void SetFrequency(uint32_t frq);
	void SetDutyCycle(uint16_t duty);

	uint32_t GetMaxFrequency(void);
private:
	uint32_t _channel;
	uint16_t _duty;		// ռ�ձ�
	uint8_t	 _accuracy; // ����

	pfun  _OCsetCompare;

	void _setMode(void);
};










//class E_TIMBase{
//public:
//	E_TIMBase(TIM_TypeDef *TIMx,E_PinID id);
//	E_TIMBase(TIM_TypeDef *TIMx);

//	void init(uint16_t period, uint16_t prescaler);
//	uint32_t GetSourceClock(void);
//protected:
//	uint32_t	_ch;		//ͨ��
//	TIM_TypeDef *_timx;		//tim
//  pfun  _OCsetCompare;
//private:
//	uint8_t 	_index;
//	E_PinBase 	*_pin;
//};


extern uint16_t t1_overflow_times ;
extern uint16_t t2_overflow_times ;
extern uint16_t t3_overflow_times ;
extern uint16_t t4_overflow_times ;

class E_CAPTURE:E_base{
	public:
	E_CAPTURE(TIM_TypeDef *TIMx,E_PinID id):E_base(TIMx){
		uint8_t _index;
		E_PinBase *_pin;
		_pin = new E_PinBase(id);
		_index = getIndex(id,TIM_MAP);
		_pin->mode(TIM_MAP[_index]._pin_date,TIM_MAP[_index]._pin_af);
		_timx = TIMx;
		
		_overflow_times = &t1_overflow_times;
		_last_value 		= 0;
		

		switch (TIM_MAP[_index]._periph_base)
		{
		case TIMxCH1:
			_channel = LL_TIM_CHANNEL_CH1;
			_ICgetCompare = &LL_TIM_IC_GetCaptureCH1;
			break;
		case TIMxCH2:
			_channel = LL_TIM_CHANNEL_CH2;
			_ICgetCompare = &LL_TIM_IC_GetCaptureCH2;
			break;
		case TIMxCH3:
			_channel = LL_TIM_CHANNEL_CH3;
			_ICgetCompare = &LL_TIM_IC_GetCaptureCH3;
			break;
		case TIMxCH4:
			_channel = LL_TIM_CHANNEL_CH4;
			_ICgetCompare = &LL_TIM_IC_GetCaptureCH4;
			break;
		}
	}
	
	

	void SetPorlicy(uint8_t porlicy);
	void begin();
	
	void        set_count(uint16_t count);
	void        set_polarity_falling();
	void        set_polarity_rising();
	
	
	//���εĻ����Ĳ�������
	void        complex_event();//������Ҫ�����ռ�ձȵ������������������ܵ���4us****
	void        simple_event();//���������������ִ��Ч�ʸߣ���߿ɲ�180K,���ǲ��ܲ���ռ�ձ�
	float       get_wave_frq();///<���ε�Ƶ��
	float       get_wave_peroid();///<���ε�����
	float       get_wave_high_duty();///<���εĸߵ�ƽռ�ձ�
	float       get_wave_low_duty();///<���εĵ͵�ƽռ�ձ�
	float       get_wave_high_time();///<���εĸߵ�ƽʱ��
	float       get_wave_low_time();///<���εĵ͵�ƽʱ��
	bool        available();///<���εĲ������

	uint32_t GetDetectMaxFrq(void);
	uint32_t GetDetectMinFrq(void);

private:
	uint32_t _channel;
	uint16_t _duty;		// ռ�ձ�
	uint8_t	 _accuracy; // ����
	uint8_t	 _porlicy;	// ����
	
	uint16_t   *_overflow_times;
	uint32_t   _last_value;
	uint32_t   _capture;	//����ֵ
	bool	   _available;
	uint32_t   _high_capture;	//�ߵ�ƽ����
	uint32_t   _low_capture;	//�͵�ƽ����
	
	uint32_t   _timeClock;
	
	pGetFun  _ICgetCompare;

	void _setMode(void);
};

#endif
