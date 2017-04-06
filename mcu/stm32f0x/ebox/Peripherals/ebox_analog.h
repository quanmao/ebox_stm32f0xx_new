/**
  ******************************************************************************
  * @file    ebox_analog.h
  * @author  cat_li
  * @version V2.0
  * @date    2016/10/21
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

#ifndef __EBOX_ANALOG_H_
#define __EBOX_ANALOG_H_

#include "stm32_define.h"
#include "ebox_define.h"
#include "ebox_gpio.h"
#include "stm32f0xx_ll_adc.h"
#include "stm32f0xx_ll_dma.h"

/* Definitions of environment analog values */
/* Value of analog reference voltage (Vref+), connected to analog voltage   */
/* supply Vdda (unit: mV).                                                  */
#define VDDA_APPLI                       ((uint32_t)2499)


extern void ADC1_init(void);
extern int get_channel(PIN_ID pin);
extern uint16_t analogin_read(uint16_t *channel);
//extern void analogin_read(uint32_t *channel,uint16_t *buffer, uint16_t size);
extern uint16_t analogin_read_voltage(uint16_t *channel);
extern void DMA_configuration(void);
extern uint16_t analog_read_temperature(void);


class E_Analog{
	public:
	E_Analog(E_PinBase *pin){
		channel = get_channel(pin->_id);
		ADC1_init();
	}
	
	uint16_t read() {		
        return analogin_read(&channel);
  }
  
  uint16_t getVoltage(){
	  return analogin_read_voltage(&channel);
  }
	private:
	uint16_t channel;	
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
