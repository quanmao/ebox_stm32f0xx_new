/**
  ******************************************************************************
  * @file    analog.h
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
 * -LQM (2016/9/5)
 *      *移植到STM32F0,基于HAL库LL层，此处和其它系列差异较大
 */
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __ANALOG_H
#define __ANALOG_H
#include "gpio.h"
#include "stdlib.h"

/* Definitions of environment analog values */
/* Value of analog reference voltage (Vref+), connected to analog voltage   */
/* supply Vdda (unit: mV).                                                  */
#define VDDA_APPLI                       ((uint32_t)3300)

extern void ADC1_init(void);
extern uint16_t analogin_read(uint32_t *channel);
extern void analogin_read(uint32_t *channel,uint16_t *buffer, uint16_t size);
extern uint16_t analogin_read_voltage(uint32_t *channel);
extern void DMA_configuration(void);
extern uint16_t analog_read_temperature(void);

class Analog{
public:
    /** Create an AnalogIn, connected to the specified pin
     *
     * @param pin AnalogIn pin to connect to
     * @param name (optional) A string to identify the object
     */
	Analog(Gpio *pin) {
		pin->mode(AIN);
		
		switch ((uint32_t)pin->id)
		{
// PA0-7			
			case PA0_ID:
				Channel = LL_ADC_CHANNEL_0 ;
				break;
			case PA1_ID:
				Channel = LL_ADC_CHANNEL_1 ;
				break;
			case PA2_ID:
				Channel = LL_ADC_CHANNEL_2 ;
				break;	
			case PA3_ID:
				Channel = LL_ADC_CHANNEL_3 ;
				break;
			case PA4_ID:
				Channel = LL_ADC_CHANNEL_4 ;
				break;
			case PA5_ID:
				Channel = LL_ADC_CHANNEL_5 ;
				break;
			case PA6_ID:
				Channel = LL_ADC_CHANNEL_6 ;
				break;	
			case PA7_ID:
				Channel = LL_ADC_CHANNEL_7 ;
				break;
// PB0,1				
			case PB0_ID:
				Channel = LL_ADC_CHANNEL_8 ;
				break;
			case PB1_ID:
				Channel = LL_ADC_CHANNEL_9 ;
				break;	
// PC0-5				
			case PC0_ID:
				Channel = LL_ADC_CHANNEL_10 ;
				break;
			case PC1_ID:
				Channel = LL_ADC_CHANNEL_11 ;
				break;
			case PC2_ID:
				Channel = LL_ADC_CHANNEL_12 ;
				break;
			case PC3_ID:
				Channel = LL_ADC_CHANNEL_13 ;
				break;
			case PC4_ID:
				Channel = LL_ADC_CHANNEL_14 ;
				break;
			case PC5_ID:
				Channel = LL_ADC_CHANNEL_15 ;
				break;				
			default:
			break;
		}
		ADC1_init();
	}

    /** Read the input voltage, represented as a float in the range [0.0, 1.0]
     *
     * @returns A floating-point value representing the current input voltage, measured as a percentage
     */
    uint16_t read() {
        return analogin_read( &Channel);
    }

    /** Read the input voltage, represented as an unsigned short in the range [0x0, 0xFFFF]
     *
     * @returns
     *   16-bit unsigned short representing the current input voltage, normalised to a 16-bit value
     */
    uint16_t read_voltage() {
        return analogin_read_voltage(&Channel);
    }

private:
	uint32_t Channel;	
};


class AnalogDMA{
public:
	/** DMA模式采集ADC数据， unit为采集的组数。DMA的实际长度 = 通道数 * 组数
	 */
	AnalogDMA(uint16_t unit)
	{
		UnitNum = unit;
		BufferSize = 0;
		ADC1_init();
		DMA_configuration();
	}

	void Add(Gpio *pin) {
		pin->mode(AIN);
		ChannelNum ++;
		switch ((uint32_t)pin->port)
		{
		case (uint32_t)GPIOA_BASE:
			switch (pin->pin)
			{
			case LL_GPIO_PIN_0:
				Channel |= LL_ADC_CHANNEL_0 ;
				break;
			case LL_GPIO_PIN_1:
				Channel |= LL_ADC_CHANNEL_1 ;
				break;
			case LL_GPIO_PIN_2:
				Channel |= LL_ADC_CHANNEL_2 ;
				break;
			case LL_GPIO_PIN_3:
				Channel |= LL_ADC_CHANNEL_3 ;
				break;
			case LL_GPIO_PIN_4:
				Channel |= LL_ADC_CHANNEL_4 ;
				break;
			case LL_GPIO_PIN_5:
				Channel |= LL_ADC_CHANNEL_5 ;
				break;
			case LL_GPIO_PIN_6:
				Channel |= LL_ADC_CHANNEL_6 ;
				break;
			case LL_GPIO_PIN_7:
				Channel |= LL_ADC_CHANNEL_7 ;
				break;
			default:
				break;
			}
			break;
		case (uint32_t)GPIOB_BASE:
			switch (pin->pin)
			{
			case LL_GPIO_PIN_0:
				Channel |= LL_ADC_CHANNEL_8 ;
				break;
			case LL_GPIO_PIN_1:
				Channel |= LL_ADC_CHANNEL_9 ;
				break;
			default:
				break;
			}
			break;
		case (uint32_t)GPIOC_BASE:
			switch (pin->pin)
			{
			case LL_GPIO_PIN_0:
				Channel |= LL_ADC_CHANNEL_10 ;
				break;
			case LL_GPIO_PIN_1:
				Channel |= LL_ADC_CHANNEL_11 ;
				break;
			case LL_GPIO_PIN_2:
				Channel |= LL_ADC_CHANNEL_12 ;
				break;
			case LL_GPIO_PIN_3:
				Channel |= LL_ADC_CHANNEL_13 ;
				break;
			case LL_GPIO_PIN_4:
				Channel |= LL_ADC_CHANNEL_14 ;
				break;
			case LL_GPIO_PIN_5:
				Channel |= LL_ADC_CHANNEL_15 ;
				break;
			default:
				break;
			}
			break;
		default:
			ChannelNum --;
			break;
		}
		
		free(Buffer);
		BufferSize = ChannelNum*UnitNum;
		Buffer = (uint16_t*)malloc(BufferSize);

	}

	/** 读取指定组ADC值
	 */
	uint16_t update() {
		if (BufferSize == 0)
		{
			return 0;
		}
		analogin_read( &Channel,Buffer,BufferSize);
		return  1;
	}
  
 /** ADC转换结果
	 */
	uint16_t *Buffer;		// 保存都取结果
    uint32_t BufferSize;
private:
	uint32_t Channel;		// 要读取的通道
	uint16_t ChannelNum;	// 要读取的通道数
	uint16_t UnitNum ;		// 读几组数据	
};

#endif
