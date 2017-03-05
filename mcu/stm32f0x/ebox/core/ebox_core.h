/**
  ******************************************************************************
  * @file    ebox_core.h
  * @author  cat_li
  * @version V2.0
  * @date    2016/10/21
  * @brief   此文件为ebox核心文件,提供了诸多非常重要的接口,可以被标准的c文件调用。
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



/* Define to prevent recursive inclusion -------------------------------------*/

#ifndef __CORE_H
#define __CORE_H


#ifdef __cplusplus
extern "C" {
#endif
// ebox 组件
#include "ebox_define.h"
#include "ebox_config.h"
// 目标cpu组件
#include "stm32f0xx.h"
#include "stm32f0xx_ll_rcc.h"
#include "stm32f0xx_ll_utils.h"
#include "stm32f0xx_ll_cortex.h"
#include "stm32f0xx_ll_pwr.h"
#include "stm32f0xx_ll_bus.h"
	

	
typedef struct
{
//	uint32_t pll_vco;
	uint32_t core;		// SYSCLK 系统时钟
	uint32_t hclk;		// AHB总线时钟，有系统时钟分频得到，一般不分频，等于系统时钟
	//uint32_t pclk2;		// APB2外设时钟,F0没有
	uint32_t pclk1;		// APB1外设时钟
}cpu_clock_t; 

typedef struct
{
	uint32_t    ability;//cpu calculate plus per second;
	cpu_clock_t	clock;
//	uint32_t    chip_id[3];
//	uint16_t    flash_size;
//	char        company[8];
}cpu_t;

extern cpu_t cpu;

///////全局变量、函数///////////////////////////////////////////////
extern __IO uint64_t millis_seconds;//提供一个mills()等效的全局变量。降低cpu调用开销

void ebox_init(void); //ebox系统初始化
uint32_t get_cpu_calculate_per_sec(void);//获取cpu在不跑操作系统的情况下的计算能力。用于统计cpu绝对使用率

uint64_t micros(void);//获取系统当前运行了多长时间。单位：us
uint64_t millis(void);//获取系统当前运行了多长时间。单位：ms
void delay_ms(uint64_t ms);//延时n毫秒。可在任意情况下使用，包括禁止所有中断的情况。精度正负1us。
void delay_us(uint16_t us);//延时n微秒。可在任意情况下使用，包括禁止所有中断的情况。精度正负1us。
//void set_systick_user_event_per_sec(uint16_t frq);//设定用户中断事件的相应频率。frq[1,1000],frq最好是2的倍数
//void attach_systick_user_event(void (*callback_fun)(void));//绑定systick中断调用函数，调用频率由用户决定，频率为1hz-1000hz


uint8_t SystickCallBackRegister(fun_noPara_t fun); // 绑定中断回调函数，使用默认周期 1ms
uint8_t SystickCallBackRegister1(fun_noPara_t fun,uint16_t multiple); // 绑定中断回调函数，允许则定义周期 = multiple * 1ms; 最大65535


__STATIC_INLINE void  delay_us_32(__IO uint16_t us)
{
		while(us--);
}




static void get_system_clock(cpu_clock_t *clock);
static void get_chip_info(void);
#ifdef __cplusplus
}

class eBox{
	public:
    eBox(int i){};
	  void sleep(void);
		void stop(void);
		void standby(void);		
};

#endif
#endif
