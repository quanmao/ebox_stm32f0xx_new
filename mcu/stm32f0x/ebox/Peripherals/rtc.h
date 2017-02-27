/**
  ******************************************************************************
  * @file    rtc.h
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
 * -LQM (2016/9/12)
 *      *��ֲ��STM32F0,����HAL��LL��,��֧�������ж�
 */
/* Define to prevent recursive inclusion -------------------------------------*/

#ifndef __RTC_H
#define __RTC_H
#include "common.h"

/**
 * ˵��:
 * -LQM (2016/9/12)
 *  1 LSEΪʱ�ӣ���Vbatά�֣�VDD���磬RTC��Ȼ��������
 *  2 LSIΪʱ�ӣ�VDD�����RTC״̬����
 *  3 HSE/32Ϊʱ�ӣ�VDD������ڲ���ѹ����V1.8�����磬RTC״̬����
 */

/* ck_apre=LSIFreq/(ASYNC prediv + 1) with LSIFreq=40 kHz RC */
#define LSI_ASYNCH_PREDIV          ((uint32_t)0x7F)
/* ck_spre=ck_apre/(SYNC prediv + 1) = 1 Hz */
#define LSI_SYNCH_PREDIV           ((uint32_t)0x137)

/* ck_apre=LSEFreq/(ASYNC prediv + 1) = 256Hz with LSEFreq=32768Hz */
#define LSE_ASYNCH_PREDIV          ((uint32_t)0x7F)
/* ck_spre=ck_apre/(SYNC prediv + 1) = 1 Hz */
#define LSE_SYNCH_PREDIV           ((uint32_t)0x00FF)


typedef struct{
	uint32_t Format12_24;	//
	uint32_t Hours;			// 0-12 or 0-23
	uint32_t Minutes;		// 0-59
	uint32_t Seconds;		// 0-59
}Time_T;

typedef struct{
	Time_T time;
	uint32_t Day;
	uint32_t WeekDaySel;
}Alarm_T;

typedef struct{
	uint32_t WeekDay;	// 1-7
	uint32_t Day;		// 1-31
	uint32_t Month;		// 1-12
	uint32_t Year;		// 0-99
}Date_T;

class Rtc
{

public:
	int begin(uint8_t clock_source);//1:LSE;0:LSI���ʹ���ⲿ����
	//��������EOK
	//ʧ�ܺ�᷵��һ��ETIMEOUT����,���Զ�תΪ�ڲ�����

//	void attach_overflow_interrupt(void (*cb_fun)(void));
	void attach_alarm_interrupt(void (*cb_fun)(void));
//	void attach_sec_interrupt(void (*cb_fun)(void));

//	void overflow_interrupt(FunctionalState state);
	void alarm_ON_OFF(FunctionalState state);
//	void sec_interrupt(FunctionalState state);


	void set_Date(Date_T date);
	void set_time(Time_T time);
	
	void set_alarm(Time_T time);
	
	void get_date_time(date_time_t *datetime);
	void get_time(Time_T *time);
	void get_date(Date_T *date);


private:
	int    config(uint8_t flag);
	uint8_t is_config(void);
	void    set_config_flag(void);
	void    nvic(FunctionalState state);
};

// extern Rtc 	rtc;

#endif

