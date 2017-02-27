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
 *     STM32F0RTC���̣�ע�⣬��F1ϵ�в�ͨ��
 *     1 ����rtc����
 * 	   2 ����begin()��ʼ��ʱ�ӡ�1 LSE  0 LSI
 *	   3 ����ʱ��
 *	   4 �������壬���������жϻص�����
 */

#include "ebox.h"
#include "bsp.h"

Rtc rtc;

void exit()
{
	Time_T time;
	uart1.printf("\r\n ����");
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
// EOK,��ʼ���ɹ�������RTCʱ�������У�����Ҫ�������ڣ�ʱ�䡣������Ҫ����
	if (rtc.begin(1) != EOK)
	{
		rtc.set_Date(date);
		rtc.set_time(time);
	}
	delay_ms(100);
	// ��������
	time.Minutes += 2;
	rtc.set_alarm(time);
	rtc.attach_alarm_interrupt(&exit);
}

uint8_t Week[7][3] = {{"һ"},{"��"},{"��"},{"��"},{"��"},{"��"},{"��"}};

int main(void)
{
	// date_time_t ������common.h�У�����������ʱ����������Ϣ
	date_time_t dtime;
	setup();
	while (1)
	{
		rtc.get_date_time(&dtime);
		uart1.printf("\n\r %2d:%02d:%2d��",dtime.hour,dtime.min,dtime.sec);
		uart1.printf("\n\r 20%2d��%02d��%2d�� ����%02d",dtime.year,dtime.min,dtime.date,dtime.week);
		delay_ms(30000);
	}
}


