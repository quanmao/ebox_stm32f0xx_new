/******************************************************************************
程 序 名： stm32_define.h　
编 写 人： cat_li
编写时间： 2017年3月21日
相关平台： STM32F0xx, ebox
接口说明： stm32相关定义
修改日志：　　
	NO.1-  初始版本
	NO.2-  2017/5/16 typedef 定义的struct数据类型统一修改为t后缀,采用驼峰命名法
				   typedef 定义的enum数据类型结尾不带t后缀,采用驼峰命名法
******************************************************************************/

#ifndef __STM32_DEFINE_H_
#define __STM32_DEFINE_H_

#include "stm32f0xx.h"
#include "stm32f0xx_ll_bus.h"
#include "stm32f0xx_ll_gpio.h"
#include "stm32f0xx_ll_tim.h"

#define NC  0xFFFFFFFF
/* structure --------------------------------------------------------------- */

/**
  * @brief  GPIO枚举，利用Pin_ID将端口和pin合并，方便使用。
  * 保持一致
 ********************************************************************************/
typedef enum{
	E_PORTA = 0x00,
	E_PORTB = 0x10,
	E_PORTC = 0x20,
	E_PORTD = 0x30,
	E_PORTE = 0x40,
	E_PORTF = 0x50
}E_Port;

typedef enum{
	PA_0 = E_PORTA|0, PA_1,PA_2,PA_3,PA_4,PA_5,PA_6,PA_7,PA_8,PA_9,PA_10,PA_11,PA_12,PA_13,PA_14,PA_15,
	PB_0 = E_PORTB|0, PB_1,PB_2,PB_3,PB_4,PB_5,PB_6,PB_7,PB_8,PB_9,PB_10,PB_11,PB_12,PB_13,PB_14,PB_15,
	PC_0 = E_PORTC|0, PC_1,PC_2,PC_3,PC_4,PC_5,PC_6,PC_7,PC_8,PC_9,PC_10,PC_11,PC_12,PC_13,PC_14,PC_15,
	PD_0 = E_PORTD|0, PD_1,PD_2,PD_3,PD_4,PD_5,PD_6,PD_7,PD_8,PD_9,PD_10,PD_11,PD_12,PD_13,PD_14,PD_15,
	PE_0 = E_PORTE|0, PE_1,PE_2,PE_3,PE_4,PE_5,PE_6,PE_7,PE_8,PE_9,PE_10,PE_11,PE_12,PE_13,PE_14,PE_15,
	PF_0 = E_PORTF|0, PF_1,PF_2,PF_3,PF_4,PF_5,PF_6,PF_7,PF_8,PF_9,PF_10,PF_11,PF_12,PF_13,PF_14,PF_15,
	P_NC = (int)0xffffffff
}E_PinID;

/**
 * @brief  GPIO mode相关参数定义，注意:E_PinMode(@enum)里的顺序必须和E_PinMode_MAP[]
 * 保持一致;通过E_PinMode_MAP[OUTPUT_OD].mode方式访问
 ********************************************************************************/
typedef struct{
	uint32_t mode;
	uint32_t OutputType;
	uint32_t Pull;
}E_PinMode_T;

typedef enum
{
	OUTPUT_PP  = 0x0,  // 0
	OUTPUT_PP_PD    ,
	OUTPUT_PP_PU    ,

	OUTPUT_OD       ,	 // 3
	OUTPUT_OD_PD    ,
	OUTPUT_OD_PU    ,

	AIN             ,	 // 6

	INPUT           ,  // 7
	INPUT_PD        ,
	INPUT_PU        ,

	AF_OD           ,  //10
	AF_OD_PD        ,
	AF_OD_PU        ,

	AF_PP           ,  //11
	AF_PP_PD        ,
	AF_PP_PU        ,
}E_PinMode;///<gpio的模式

// 常量数组，程序中直接通过数组下标访问。一定要确保数组E_PinMode_MAP[]和枚举变量E_PinMode中的数据顺序一致
const E_PinMode_T E_PinMode_MAP[] ={
	LL_GPIO_MODE_OUTPUT,LL_GPIO_OUTPUT_PUSHPULL,LL_GPIO_PULL_NO,			//OUTPUT_PP
	LL_GPIO_MODE_OUTPUT,LL_GPIO_OUTPUT_PUSHPULL,LL_GPIO_PULL_DOWN,		//OUTPUT_PP_PD
	LL_GPIO_MODE_OUTPUT,LL_GPIO_OUTPUT_PUSHPULL,LL_GPIO_PULL_UP,			//OUTPUT_PP_PU

	LL_GPIO_MODE_OUTPUT,LL_GPIO_OUTPUT_OPENDRAIN,LL_GPIO_PULL_NO,			//OUTPUT_OD
	LL_GPIO_MODE_OUTPUT,LL_GPIO_OUTPUT_OPENDRAIN,LL_GPIO_PULL_DOWN,		//OUTPUT_OD_PD
	LL_GPIO_MODE_OUTPUT,LL_GPIO_OUTPUT_OPENDRAIN,LL_GPIO_PULL_UP,			//OUTPUT_OD_PU

	LL_GPIO_MODE_ANALOG,LL_GPIO_OUTPUT_PUSHPULL,LL_GPIO_PULL_NO,			//AIN
	LL_GPIO_MODE_INPUT,LL_GPIO_OUTPUT_PUSHPULL,LL_GPIO_PULL_NO,				//INPUT
	LL_GPIO_MODE_INPUT,LL_GPIO_OUTPUT_PUSHPULL,LL_GPIO_PULL_DOWN,			//INPUT_PD
	LL_GPIO_MODE_INPUT,LL_GPIO_OUTPUT_PUSHPULL,LL_GPIO_PULL_UP,				//INPUT_PU
// 如果没有使用af功能，可以屏蔽掉，减少空间占用
	LL_GPIO_MODE_ALTERNATE,LL_GPIO_OUTPUT_OPENDRAIN,LL_GPIO_PULL_NO,	//AF_OD
	LL_GPIO_MODE_ALTERNATE,LL_GPIO_OUTPUT_OPENDRAIN,LL_GPIO_PULL_DOWN,//AF_OD_PD
	LL_GPIO_MODE_ALTERNATE,LL_GPIO_OUTPUT_OPENDRAIN,LL_GPIO_PULL_UP,	//AF_OD_PU

	LL_GPIO_MODE_ALTERNATE,LL_GPIO_OUTPUT_PUSHPULL,LL_GPIO_PULL_NO,		//AF_PP
	LL_GPIO_MODE_ALTERNATE,LL_GPIO_OUTPUT_PUSHPULL,LL_GPIO_PULL_DOWN,	//AF_PP_PD
	LL_GPIO_MODE_ALTERNATE,LL_GPIO_OUTPUT_PUSHPULL,LL_GPIO_PULL_UP,		//AF_PP_PU
};

/**
 *@brief  GPIO PORT相关
 *
 ********************************************************************************/
typedef struct{
	GPIO_TypeDef* port;
	uint32_t Periphs;
}E_PORT_T;

#define EGPIOA {GPIOA,LL_AHB1_GRP1_PERIPH_GPIOA}
#define EGPIOB {GPIOB,LL_AHB1_GRP1_PERIPH_GPIOB}
#define EGPIOC {GPIOC,LL_AHB1_GRP1_PERIPH_GPIOC}

const E_PORT_T E_PORT_MAP[] =
{
	EGPIOA,
	EGPIOB,
	EGPIOC
};

/**
 *@brief  外设相关
 *
 ********************************************************************************/
typedef struct{
	uint32_t id;
	uint32_t periph;
	uint32_t channel;
	uint8_t  alternate;
}E_PIN_FUN_T;

#define	PWM_3_1				PB4, TIM3_BASE, LL_TIM_CHANNEL_CH1,LL_GPIO_AF_1
#define PIN_FUN_NC		(uint32_t)NC,NC,NC,(uint8_t)NC

//const E_PIN_FUN_T PWM_MAP[] ={
//		PWM_3_1,
//		PIN_FUN_NC
//};

//const E_PIN_FUN_T E_UART_Tx[] ={
//		PA9,USART1_BASE,0,LL_GPIO_AF_1,
//		(uint32_t)NC,NC,(uint8_t)NC
//};

//const E_PIN_FUN_T E_UART_Rx[] ={
//		PA10,USART1_BASE,0,LL_GPIO_AF_1,
//		(uint32_t)NC,NC,(uint8_t)NC
//};

/**
 *@brief  外设相关
 *
 ********************************************************************************/
typedef struct{
	uint32_t name;
	uint32_t rcc;
	uint8_t	 irq;
}Periph_T;


__STATIC_INLINE uint8_t PeriphIndex(uint32_t Periphx,const Periph_T *emap)
{
	uint8_t i = 0;
	while (!((emap+i)->name == Periphx ))
	{
		if ((emap+i)->name == (uint16_t)NC){
			return (uint8_t)NC;
		}
		i++;
	}
	return i;
}

__STATIC_INLINE uint8_t PinIndex(uint16_t pin,const E_PIN_FUN_T *emap)
{
	uint8_t i = 0;
	while (!((emap+i)->id == pin ))
	{
		if ((emap+i)->id == (uint16_t)NC){
			return (uint8_t)NC;
		}
		i++;
	}
	return i;
}

__STATIC_INLINE uint8_t PinAlternate(uint16_t pin,const E_PIN_FUN_T *emap)
{
	while (!((emap->id == pin)))
	{
		if (emap->id == (uint16_t)NC){
			return (uint8_t)NC;
		}
		emap++;
	}
	return emap->alternate;
}

__STATIC_INLINE uint32_t PinCH(uint16_t pin,const E_PIN_FUN_T *emap)
{
	while (!((emap->id == pin)))
	{
		if (emap->id == (uint16_t)NC){
			return (uint8_t)NC;
		}
		emap++;
	}
	return emap->channel;
}


#define TIM_2 			TIM2_BASE,LL_APB1_GRP1_PERIPH_TIM2,TIM2_IRQn
#define TIM_3 			TIM3_BASE,LL_APB1_GRP1_PERIPH_TIM3,TIM3_IRQn
#define TIM_6				TIM6_BASE,LL_APB1_GRP1_PERIPH_TIM6,TIM6_IRQn
#define USART_1			USART1_BASE,LL_APB1_GRP2_PERIPH_USART1,USART1_IRQn
#define	Periph_NC 	(uint32_t)NC,(uint32_t)NC,(uint8_t)NC

const Periph_T Usart_MAP[] = {
	USART_1,
	Periph_NC
};

const Periph_T Periph_MAP[] = {
	TIM_2,
	TIM_3,
	TIM_6,
	Periph_NC
};

/**
  * 结构体，保存Pin_id,以及对应的外设，af信息
  */
typedef struct{
	uint8_t 	pin_id;
	uint8_t		pin_af;
	uint32_t 	Periph;
}Periph_Com_T;

#define Periph_Com_NC (uint8_t)NC,(uint8_t)NC,(uint8_t)NC

const Periph_Com_T SPI_MAP[] = {
	PA_5,0,(uint32_t)SPI1,
	PA_6,0,(uint32_t)SPI1,
	PA_6,0,(uint32_t)SPI1,
	Periph_Com_NC
};

const Periph_Com_T I2C_MAP[] = {
	PB_6,1,LL_APB1_GRP1_PERIPH_I2C1,
	PB_7,1,LL_APB1_GRP1_PERIPH_I2C1,
	PB_8,1,LL_APB1_GRP1_PERIPH_I2C1,
	PB_9,1,LL_APB1_GRP1_PERIPH_I2C1,
	Periph_Com_NC
};

const Periph_Com_T USART_MAP[] = {
	PA_9,LL_GPIO_AF_1,LL_APB1_GRP2_PERIPH_USART1,
	PA_10,LL_GPIO_AF_1,LL_APB1_GRP2_PERIPH_USART1,
	Periph_Com_NC

};

/**
 *@brief    根据Pin_id获取对应外设索引
 *@param    val：1：输出高电平；0：输出低电平
 *@retval   NONE
*/
__STATIC_INLINE uint8_t getIndex(uint8_t pin_id,const Periph_Com_T *emap)
{
	uint8_t i = 0;
	while (!((emap+i)->pin_id == pin_id ))
	{
		if ((emap+i)->pin_id == (uint8_t)NC){
			return (uint8_t)NC;
		}
		i++;
	}
	return i;
}


#endif
