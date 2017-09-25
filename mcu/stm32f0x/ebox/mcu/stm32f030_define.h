/******************************************************************************
程 序 名： stm32f030_define.h　
编 写 人： cat_li
编写时间： 2017年9月11日
接口说明： stm32f030x4外设定义(tssop20).
	1个spi,i2c,usart,adc(12ch),15gpios,5tim(4general,1advance)4kRAM,16kFLASH
修改日志：　　
	NO.1-  初始版本
******************************************************************************/

#ifndef __STM32F030_DEFINE_H_
#define __STM32F030_DEFINE_H_

//#include "stm32f0xx.h"
#include "stm32f0xx_ll_bus.h"
#include "stm32f0xx_ll_gpio.h"
#include "stm32f0xx_ll_tim.h"
//#include "ebox_define.h"

#include "stm32_define.h"

#define I2C1_SCL	PA_9
#define	I2C1_SDA	PA_10

#define SPI_SCK		PA_5
#define SPI_MISO	PA_6
#define SPI_MOSI	PA_7

#include "stm32f0xx_ll_adc.h"

static const AF_FUN_S ADC_MAP[] = {
	PA_0,AIN,0,LL_ADC_CHANNEL_0,
	PA_1,AIN,0,LL_ADC_CHANNEL_1,
	PA_2,AIN,0,LL_ADC_CHANNEL_2,
	PA_3,AIN,0,LL_ADC_CHANNEL_3,
	PA_4,AIN,0,LL_ADC_CHANNEL_4,
	PA_5,AIN,0,LL_ADC_CHANNEL_5,
	PA_6,AIN,0,LL_ADC_CHANNEL_6,
	PA_7,AIN,0,LL_ADC_CHANNEL_7,

	PB_1,AIN,0,LL_ADC_CHANNEL_9,

	P_NC
};

#define TIMxCH1 0x01
#define TIMxCH2 0x02
#define TIMxCH3 0x03
#define TIMxCH4 0x04

static const AF_FUN_S TIM_MAP[] ={
	// TIM3,CH1,CH2
	PA_6,AF_PP,LL_GPIO_AF_1,TIMxCH1,
	PA_7,AF_PP,LL_GPIO_AF_1,TIMxCH2,
	// TIM2,CH2,CH3,CH4
	PA_5,AF_PP,LL_GPIO_AF_2,TIMxCH1,
	PA_0,AF_PP,LL_GPIO_AF_2,TIMxCH1,
	PA_1,AF_PP,LL_GPIO_AF_2,TIMxCH2,
	PA_2,AF_PP,LL_GPIO_AF_2,TIMxCH3,
	PA_3,AF_PP,LL_GPIO_AF_2,TIMxCH4,
	// TIM1,CH1
	PA_8,AF_PP,LL_GPIO_AF_2,TIMxCH1,
	P_NC
};

static const Periph_SS TIM_INFO[]={
	TIM1_BASE,LL_APB1_GRP2_EnableClock,LL_APB1_GRP2_PERIPH_TIM1,TIM1_BRK_UP_TRG_COM_IRQn,Irq1,
	TIM3_BASE,LL_APB1_GRP1_EnableClock,LL_APB1_GRP1_PERIPH_TIM3,TIM3_IRQn,Irq3,
	NC
};
#define SPI_SCK		PA_5
#define SPI_MISO	PA_6
#define SPI_MOSI	PA_7

static const AF_FUN_S SPI_MAP[] = {
	PA_5,AF_PP_PU,0,SPI1_BASE,
	PA_6,AF_PP_PU,0,SPI1_BASE,
	PA_7,AF_PP_PU,0,SPI1_BASE,
	P_NC
};
		
static const Periph_S SPI_INFO[] = {
	SPI1_BASE,LL_APB1_GRP2_PERIPH_SPI1,USART1_IRQn,Irq1,
	NC
};

// PA2,9,14 tx; PA3,10 rx
static const AF_FUN_S UART_MAP[] ={
	PA_2,AF_PP_PU,LL_GPIO_AF_1,USART1_BASE,
	PA_3,AF_PP_PU,LL_GPIO_AF_1,USART1_BASE,
	PA_9,AF_PP_PU,LL_GPIO_AF_1,USART1_BASE,
	PA_10,AF_PP_PU,LL_GPIO_AF_1,USART1_BASE,
	PA_14,AF_PP_PU,LL_GPIO_AF_1,USART1_BASE,
	P_NC
};

static const Periph_S UART_INFO[] = {
	USART1_BASE,LL_APB1_GRP2_PERIPH_USART1,USART1_IRQn,Irq1,
	NC
};

// PA9 scl,PA10 SDA	
static const AF_FUN_S I2C_MAP[]={
	// i2c1
	PA_9,AF_PP_PU,LL_GPIO_AF_4,I2C1_BASE,
	PA_10,AF_PP_PU,LL_GPIO_AF_4,I2C1_BASE,
	P_NC
};

static const Periph_S I2C_INFO[] ={
	I2C1_BASE,LL_APB1_GRP1_PERIPH_I2C1,I2C1_IRQn,Irq1,
	NC
};

#endif
