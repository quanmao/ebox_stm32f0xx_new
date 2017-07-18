/**
  ******************************************************************************
  * @file    ebox_rtc.h
  * @author  cat_li
  * @version V2.0
  * @date    2017/7/18
  * @brief   STM32F0 @ HAL lib
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
/* Includes ------------------------------------------------------------------*/
#include "ebox_rtc.h"
#include "stm32f0xx_ll_exti.h"
#include "ebox.h"
#include "ebox_debug.h"


#define RTC_TIMEOUT	5000	// 5s
/* Define used to indicate date/time updated */
#define RTC_BKP_DATE_TIME_UPDTATED ((uint32_t)0x32F2)

//Rtc rtc;
fun_noPara_t rtc_callback;//

/**
  * @brief  �ȴ�Time&Date�Ĵ�����RTC(APB)ʱ��ͬ��
  * @param  None
  * @retval RTC_ERROR_NONE if no error (RTC_ERROR_TIMEOUT will occur if RTC is 
  *         not synchronized)
  */
__INLINE uint32_t ebox_WaitForSynchro_RTC(void)
{
	uint64_t end = GetEndTime(5000);
  /* Clear RSF flag */
  LL_RTC_ClearFlag_RS(RTC);
  /* Wait the registers to be synchronised */
  while(LL_RTC_IsActiveFlag_RS(RTC) != 1)
  {
	  if (IsTimeOut(end))
	  {
		  DBG("ʱ��ͬ����ʱ\r\n");
			return 0;
	  }
  }
  return 0;
}
/**
  * @brief  ���븳ֵģʽ
  * @note   ��ģʽ������ֹͣ�����Ը���RTCֵ
  * @param  None
  * @retval RTC_ERROR_NONE if no error
  */
uint32_t ebox_Enter_RTC_InitMode(void)
{
	uint64_t end = GetEndTime(RTC_TIMEOUT);
  /* Set Initialization mode */
  LL_RTC_EnableInitMode(RTC);  
  /* Check if the Initialization mode is set */
  while (LL_RTC_IsActiveFlag_INIT(RTC) != 1)
  {
		if (IsTimeOut(end))
	  {
		  DBG("���븳ֵģʽ��ʱ\r\n");
			return 0;
	  }
  }  
  return 0;
}

/**
  * @brief  �˳���ֵģʽ
  * @param  None
  * @retval RTC_ERROR_NONE if no error
  */
uint32_t ebox_Exit_RTC_InitMode(void)
{
  LL_RTC_DisableInitMode(RTC);
  /* Wait for synchro */
  /* Note: Needed only if Shadow registers is enabled LL_RTC_IsShadowRegBypassEnabled function can be used */
  return (ebox_WaitForSynchro_RTC());
}





int E_RTC::begin(uint8_t clock_source)
{
	int ret = E_OTHER;

	if (_clocks)	//LSE
	{
		if ((is_config() == 0) )	//ʱ�ӵ��磬��Ҫ��������
		//if ((is_config() == 0) || (LL_RCC_LSE_IsReady() == 0))	//ʱ�ӵ��磬��Ҫ��������
		{
			if (_config(clock_lse) != E_OK)
			{
				_config(clock_lsi);
				DBG("LSEʱ������ʧ��,ʹ��LSIʱ��\r\n");
				//LL_RTC_BAK_SetRegister(RTC, LL_RTC_BKP_DR1, 0x00);
				ret = E_OTHER;
			}			
		}
		else	// ʱ�ӱ��ֹ���������Ҫ����
		{
			ret = E_OK;
		}
	}else{	// ��������ʱ��ԴVDD�����RTC״̬������������Ҫ��ʼ��
		_config(clock_lsi);
		//LL_RTC_BAK_SetRegister(RTC, LL_RTC_BKP_DR1, 0x00);
	}
	return ret;
}

/**
 *@name     setDate
 *@brief    ��������
 *@param    Date_T date ����
 *@retval   none
*/
int E_RTC::_config(ClockS clock)
{
	uint32_t ret = 50000;
	// Ĭ��Ϊ�ڲ�LSI
	uint32_t RTC_ASYNCH_PREDIV = LSI_ASYNCH_PREDIV;
	uint32_t RTC_SYNCH_PREDIV = LSI_SYNCH_PREDIV;

	/*##-1- Enables the PWR Clock and Enables access to the backup domain #######*/
	/* To change the source clock of the RTC feature (LSE, LSI), you have to:
	   - Enable the power clock
	   - Enable write access to configure the RTC clock source (to be done once after reset).
	   - Reset the Back up Domain
	   - Configure the needed RTC clock source */
	LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_PWR);
	LL_PWR_EnableBkUpAccess();
	/*##-2- Configure LSE/LSI as RTC clock source ###############################*/
	if (clock)		// 1 LSE
	{
		if (LL_RCC_LSE_IsReady() == 0)
		{
			LL_RCC_ForceBackupDomainReset();
			LL_RCC_ReleaseBackupDomainReset();
			LL_RCC_LSE_Enable();

			while (LL_RCC_LSE_IsReady() != 1)
			{
				if (ret-- == 0)
				{
					return E_TIMEOUT;
				}
			}
			// �ⲿ����LSE����
			RTC_ASYNCH_PREDIV = LSE_ASYNCH_PREDIV;
			RTC_SYNCH_PREDIV = LSE_SYNCH_PREDIV;
			/* Enable LSE only if disabled.*/
			LL_RCC_SetRTCClockSource(LL_RCC_RTC_CLKSOURCE_LSE);
			/* Enable RTC Clock */
			LL_RCC_EnableRTC();
		}
	}else{				// 0 LSI
		/* Enable LSI */
		LL_RCC_LSI_Enable();
		while (LL_RCC_LSI_IsReady() != 1)
		{
			if (ret-- == 0)
			{
				return E_TIMEOUT;
			}
		}
		LL_RCC_ForceBackupDomainReset();
		LL_RCC_ReleaseBackupDomainReset();
		LL_RCC_SetRTCClockSource(LL_RCC_RTC_CLKSOURCE_LSI);
		/*##-3- Enable RTC peripheral Clocks #######################################*/
		/* Enable RTC Clock */
		LL_RCC_EnableRTC();
	}

	/*##-4- Disable RTC registers write protection ##############################*/
	LL_RTC_DisableWriteProtection(RTC);
	/*##-5- Enter in initialization mode #######################################*/
	if (ebox_Enter_RTC_InitMode() != 0){}
	/*##-6- Configure RTC ######################################################*/
	/* Configure RTC prescaler and RTC data registers */
	/* Set Hour Format */
	LL_RTC_SetHourFormat(RTC, LL_RTC_HOURFORMAT_24HOUR);
	/* Set Asynch Prediv (value according to source clock) */
	LL_RTC_SetAsynchPrescaler(RTC, RTC_ASYNCH_PREDIV);
	/* Set Synch Prediv (value according to source clock) */
	LL_RTC_SetSynchPrescaler(RTC, RTC_SYNCH_PREDIV);

	/*##-7- Exit of initialization mode #######################################*/
	ebox_Exit_RTC_InitMode();
	/*##-8- Enable RTC registers write protection #############################*/
	LL_RTC_EnableWriteProtection(RTC);
	nvic(ENABLE);
	return E_OK;
}

uint8_t E_RTC::is_config(void)
{
#if defined(RTC_BACKUP_SUPPORT)
	return (LL_RTC_BAK_GetRegister(RTC, LL_RTC_BKP_DR1) == RTC_BKP_DATE_TIME_UPDTATED);
	//return 0;
#else
	return 0;
#endif
}


void E_RTC::set_config_flag(void)
{
#if defined(RTC_BACKUP_SUPPORT)
	/*##-8- Writes a data in a RTC Backup data Register1 #######################*/
	LL_RTC_BAK_SetRegister(RTC, LL_RTC_BKP_DR1, RTC_BKP_DATE_TIME_UPDTATED);
#endif
}

/**
 *@name     setDate
 *@brief    ��������
 *@param    Date_T date ����
 *@retval   none
*/
void E_RTC::setDate(Date_T date)
{
	/*##-1- Disable RTC registers write protection ############################*/
	LL_RTC_DisableWriteProtection(RTC);
	ebox_Enter_RTC_InitMode();
	/* Set Date: 2016��9��14*/
	LL_RTC_DATE_Config(RTC, __LL_RTC_CONVERT_BIN2BCD(date.WeekDay), __LL_RTC_CONVERT_BIN2BCD(date.Day), __LL_RTC_CONVERT_BIN2BCD(date.Month), __LL_RTC_CONVERT_BIN2BCD(date.Year));

	ebox_Exit_RTC_InitMode();
	/*##-8- Enable RTC registers write protection #############################*/
	LL_RTC_EnableWriteProtection(RTC);
}

/**
 *@name     setTime
 *@brief    ����ʱ��
 *@param    Time_T time ʱ��
 *@retval   none
*/
void E_RTC::setTime(Time_T time)
{
	/*##-1- Disable RTC registers write protection ############################*/
	LL_RTC_DisableWriteProtection(RTC);
	ebox_Enter_RTC_InitMode();
	/* Set Time: 24Сʱ 00:00:00*/
	LL_RTC_TIME_Config(RTC,__LL_RTC_CONVERT_BIN2BCD(time.Format12_24), __LL_RTC_CONVERT_BIN2BCD(time.Hours), __LL_RTC_CONVERT_BIN2BCD(time.Minutes), __LL_RTC_CONVERT_BIN2BCD(time.Seconds));
	ebox_Exit_RTC_InitMode();
	/*##-8- Enable RTC registers write protection #############################*/
	LL_RTC_EnableWriteProtection(RTC);
	set_config_flag();
}

/**
 *@name     setAlarm
 *@brief    ��������
 *@param    Time_T time ����ʱ��
 *@retval   none
*/
void E_RTC::setAlarm(Time_T time)
{
	DBG("enter set alarm------\r\n");
	/*##-1- Disable RTC registers write protection ############################*/
	LL_RTC_DisableWriteProtection(RTC);
	ebox_Enter_RTC_InitMode();
	/* Set Alarm to 12:00:25
	   RTC Alarm Generation: Alarm on Hours, Minutes and Seconds (ignore date/weekday)*/
	LL_RTC_ALMA_ConfigTime(RTC, __LL_RTC_CONVERT_BIN2BCD(time.Format12_24), __LL_RTC_CONVERT_BIN2BCD(time.Hours), __LL_RTC_CONVERT_BIN2BCD(time.Minutes), __LL_RTC_CONVERT_BIN2BCD(time.Seconds));
	LL_RTC_ALMA_SetMask(RTC, LL_RTC_ALMA_MASK_DATEWEEKDAY);

	/* Enable Alarm*/
	LL_RTC_ALMA_Enable(RTC);
	/* Clear the Alarm interrupt pending bit */
	LL_RTC_ClearFlag_ALRA(RTC);
	/* Enable IT Alarm */
	LL_RTC_EnableIT_ALRA(RTC);
	ebox_Exit_RTC_InitMode();
	/*##-8- Enable RTC registers write protection #############################*/
	LL_RTC_EnableWriteProtection(RTC);
}


void E_RTC::nvic(FunctionalState state)
{
	/* RTC Alarm Interrupt Configuration: EXTI configuration */
	LL_EXTI_EnableIT_0_31(LL_EXTI_LINE_17);
	LL_EXTI_EnableRisingTrig_0_31(LL_EXTI_LINE_17);
	/*## Configure the NVIC for RTC Alarm ##################################*/
	NVIC_SetPriority(RTC_IRQn, 0x0F);
	NVIC_EnableIRQ(RTC_IRQn);
}

//void Rtc::attach_sec_interrupt(void (*cb_fun)(void))
//{
//    rtc_callback_table[0] = cb_fun;
//}
void E_RTC::attach_alarm_interrupt(void (*cb_fun)(void))
{
    rtc_callback = cb_fun;
}
//void Rtc::attach_overflow_interrupt(void (*cb_fun)(void))
//{
//    rtc_callback_table[2] = cb_fun;
//}

//void Rtc::sec_interrupt(FunctionalState state)
//{
// 
//}

void E_RTC::alarm_ON_OFF(FunctionalState state)
{
	LL_RTC_DisableWriteProtection(RTC);	
	/* Clear the Alarm interrupt pending bit */
	LL_RTC_ClearFlag_ALRA(RTC);
	/* Enable IT Alarm */
	if (state == ENABLE)
		LL_RTC_EnableIT_ALRA(RTC);
	else
		LL_RTC_DisableIT_ALRA(RTC);	
	LL_RTC_EnableWriteProtection(RTC);
}

void E_RTC::getDateTime(date_time_t *datetime)
{
	datetime->year 	= __LL_RTC_CONVERT_BCD2BIN(LL_RTC_DATE_GetYear(RTC));
	datetime->month = __LL_RTC_CONVERT_BCD2BIN(LL_RTC_DATE_GetMonth(RTC));
	datetime->date 	= __LL_RTC_CONVERT_BCD2BIN(LL_RTC_DATE_GetDay(RTC));
	datetime->week 	= __LL_RTC_CONVERT_BCD2BIN(LL_RTC_DATE_GetWeekDay(RTC));

	datetime->hour 	= __LL_RTC_CONVERT_BCD2BIN(LL_RTC_TIME_GetHour(RTC));
	datetime->min 	= __LL_RTC_CONVERT_BCD2BIN(LL_RTC_TIME_GetMinute(RTC));
	datetime->sec 	= __LL_RTC_CONVERT_BCD2BIN(LL_RTC_TIME_GetSecond(RTC));
};

void E_RTC::getTime(Time_T *time)
{
	time->Format12_24 = __LL_RTC_CONVERT_BCD2BIN(LL_RTC_TIME_GetFormat(RTC));
	time->Hours 		= __LL_RTC_CONVERT_BCD2BIN(LL_RTC_TIME_GetHour(RTC));
	time->Minutes 	= __LL_RTC_CONVERT_BCD2BIN(LL_RTC_TIME_GetMinute(RTC));
	time->Seconds 	= __LL_RTC_CONVERT_BCD2BIN(LL_RTC_TIME_GetSecond(RTC));
}

void E_RTC::getDate(Date_T *date)
{
	date->Year	 = __LL_RTC_CONVERT_BCD2BIN(LL_RTC_DATE_GetYear(RTC));
	date->Month	 = __LL_RTC_CONVERT_BCD2BIN(LL_RTC_DATE_GetMonth(RTC));
	date->Day 	 = __LL_RTC_CONVERT_BCD2BIN(LL_RTC_DATE_GetDay(RTC));
	date->WeekDay = __LL_RTC_CONVERT_BCD2BIN(LL_RTC_DATE_GetWeekDay(RTC));
}


extern "C" {
	void RTC_IRQHandler(void)
	{
		/* Get the Alarm interrupt source enable status */
		if (LL_RTC_IsEnabledIT_ALRA(RTC) != 0)
		{
			/* Get the pending status of the Alarm Interrupt */
			if (LL_RTC_IsActiveFlag_ALRA(RTC) != 0)
			{
				/* Alarm callback */
				rtc_callback();
				/* Clear the Alarm interrupt pending bit */
				LL_RTC_ClearFlag_ALRA(RTC);
			}
		}
		/* Clear the EXTI's Flag for RTC Alarm */
		LL_EXTI_ClearFlag_0_31(LL_EXTI_LINE_17);
	}
}