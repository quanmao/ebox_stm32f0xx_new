/**
  ******************************************************************************
  * @file    ebox_analog.h
  * @author  cat_li
  * @brief   
	*		2017/7/31	 LQM
			1	单通道ADC转换模式
	* 已知bug
			1 内部参考电压第一次读取正确，之后每次读取需要间隔800ms，才能保证参考电压正确
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

#ifndef __EBOX_ANALOG_H_
#define __EBOX_ANALOG_H_

#include "stm32_define.h"
#include "ebox_config.h"
#include "ebox_define.h"
#include "ebox_gpio.h"
#include "stm32f0xx_ll_adc.h"
#include "stm32f0xx_ll_dma.h"

#ifdef	USE_TIMEOUT
	#define TIMEOUT		1000	// 默认超时时间为1000us
#endif




extern void ADC1_init(void);
extern int get_channel(E_PinID pin);
extern uint16_t analogin_read(uint32_t *channel);
//extern void analogin_read(uint32_t *channel,uint16_t *buffer, uint16_t size);
extern uint16_t analogin_read_voltage(uint16_t *channel);
extern void DMA_configuration(void);
extern uint16_t analog_read_temperature(void);

typedef enum 
{
	ADC_TEMP = 0, 
#if defined(ADC_CCR_VBATEN)
	ADC_BAT
#endif
}ADC_CH;

class E_ADC{
public:
	E_ADC(E_PinID id,uint16_t refVoltage = 3300);
	E_ADC(ADC_CH ch);
	
	void setRefVoltage(uint16_t refVoltage);

	uint16_t read();
	uint16_t getVoltage();
	float 	 getTemperature();
private:
	uint32_t _channel;		// adc通道	
	void _setInterChannel(void);
};


class E_AnalogDMA{
	public:
	  E_AnalogDMA(E_PinBase *pin){
		pin->mode(AIN,40);
		channel |= get_channel(pin->_id);
		channelNum = 1;
//		ADC1_init();
	};
		
		E_AnalogDMA(E_PinBase *pin,uint8_t deep){
		pin->mode(AIN,40);
		channel |= get_channel(pin->_id);
		channelNum = 1;
		_deep = deep;
//		ADC1_init();
	};
		
	
	void add(E_PinBase *pin){
		channel |= get_channel(pin->_id);
		channelNum ++;
	}
	
	void dmaConfig();
	void dmaStart();
	
	volatile uint16_t buffer[4];
	private:
	uint32_t channel;	
	uint8_t _deep;
	static uint8_t  channelNum;

};

#endif
