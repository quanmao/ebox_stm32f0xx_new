/**
  ******************************************************************************
  * @file   : *.cpp
  * @author : shentq
  * @version: V1.2
  * @date   : 2016/08/14

  * @brief   ebox application example .
  *
  * Copyright 2016 shentq. All Rights Reserved.
  ******************************************************************************
 */
/**
 * -LQM (2016/9/18)
 *     STM32F0RTC例程，注意，和F1系列不通用
 *     1 创建rtc对象
 * 	   2 调用begin()初始化时钟。1 LSE  0 LSI
 *	   3 设置时钟
 *	   4 设置闹铃，附加闹铃中断回调函数
 */

#include "ebox.h"
#include "bsp.h"

Rtc rtc;

void exit()
{
	Time_T time;
	uart1.printf("\r\n 闹铃");
	rtc.get_time(&time);
	uart1.printf(" %2d:%02d:%2d ",time.Hours,time.Minutes,time.Seconds);
}

void setup()
{
	Date_T date = {LL_RTC_WEEKDAY_WEDNESDAY, 13, LL_RTC_MONTH_SEPTEMBER, 16};
	Time_T time = {LL_RTC_TIME_FORMAT_AM_OR_24, 12, 1, 1};

	ebox_init();
	uart1.begin(115200);

	uart1.printf("\r\n");
	uart1.printf("\n\r **** RTC Test**** ");
// EOK,初始化成功，并且RTC时间在运行，不需要设置日期，时间。否则需要设置
	if (rtc.begin(1) != EOK)
	{
		rtc.set_Date(date);
		rtc.set_time(time);
	}
	delay_ms(100);
	// 设置闹铃
	time.Minutes += 2;
	rtc.set_alarm(time);
	rtc.attach_alarm_interrupt(&exit);
}

uint8_t Week[7][3] = {{"一"},{"二"},{"三"},{"四"},{"五"},{"六"},{"日"}};

int main(void)
{
	// date_time_t 声明在common.h中，包含年月日时分秒星期信息
	date_time_t dtime;
	setup();
	while (1)
	{
		rtc.get_date_time(&dtime);
		uart1.printf("\n\r %2d:%02d:%2d秒",dtime.hour,dtime.min,dtime.sec);
		uart1.printf("\n\r 20%2d年%02d月%2d日 星期%02d",dtime.year,dtime.min,dtime.date,dtime.week);
		delay_ms(30000);
	}
}


