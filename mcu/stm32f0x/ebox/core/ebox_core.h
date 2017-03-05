/**
  ******************************************************************************
  * @file    ebox_core.h
  * @author  cat_li
  * @version V2.0
  * @date    2016/10/21
  * @brief   ���ļ�Ϊebox�����ļ�,�ṩ�����ǳ���Ҫ�Ľӿ�,���Ա���׼��c�ļ����á�
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
// ebox ���
#include "ebox_define.h"
#include "ebox_config.h"
// Ŀ��cpu���
#include "stm32f0xx.h"
#include "stm32f0xx_ll_rcc.h"
#include "stm32f0xx_ll_utils.h"
#include "stm32f0xx_ll_cortex.h"
#include "stm32f0xx_ll_pwr.h"
#include "stm32f0xx_ll_bus.h"
	

	
typedef struct
{
//	uint32_t pll_vco;
	uint32_t core;		// SYSCLK ϵͳʱ��
	uint32_t hclk;		// AHB����ʱ�ӣ���ϵͳʱ�ӷ�Ƶ�õ���һ�㲻��Ƶ������ϵͳʱ��
	//uint32_t pclk2;		// APB2����ʱ��,F0û��
	uint32_t pclk1;		// APB1����ʱ��
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

///////ȫ�ֱ���������///////////////////////////////////////////////
extern __IO uint64_t millis_seconds;//�ṩһ��mills()��Ч��ȫ�ֱ���������cpu���ÿ���

void ebox_init(void); //eboxϵͳ��ʼ��
uint32_t get_cpu_calculate_per_sec(void);//��ȡcpu�ڲ��ܲ���ϵͳ������µļ�������������ͳ��cpu����ʹ����

uint64_t micros(void);//��ȡϵͳ��ǰ�����˶೤ʱ�䡣��λ��us
uint64_t millis(void);//��ȡϵͳ��ǰ�����˶೤ʱ�䡣��λ��ms
void delay_ms(uint64_t ms);//��ʱn���롣�������������ʹ�ã�������ֹ�����жϵ��������������1us��
void delay_us(uint16_t us);//��ʱn΢�롣�������������ʹ�ã�������ֹ�����жϵ��������������1us��
//void set_systick_user_event_per_sec(uint16_t frq);//�趨�û��ж��¼�����ӦƵ�ʡ�frq[1,1000],frq�����2�ı���
//void attach_systick_user_event(void (*callback_fun)(void));//��systick�жϵ��ú���������Ƶ�����û�������Ƶ��Ϊ1hz-1000hz


uint8_t SystickCallBackRegister(fun_noPara_t fun); // ���жϻص�������ʹ��Ĭ������ 1ms
uint8_t SystickCallBackRegister1(fun_noPara_t fun,uint16_t multiple); // ���жϻص������������������� = multiple * 1ms; ���65535


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
