/**
  ******************************************************************************
  * @file   : PowerMeter.cpp
  * @author : cat_li
  * @version: V2.0
  * @date   : 2017/9/18
  * @brief   硬件STM32F030,LCD1602@I2C,INA219; soft by ebox
  *
  * Copyright 2016 shentq. All Rights Reserved.
  ******************************************************************************
 */


#include "ebox.h"
#include <stdio.h>

E_GPIO PA5(PA_5);
E_UART usart(USART1,PA_2,PA_3);

int main(){
//
	ebox_init();
	PA5.mode(OUTPUT_PP);
	usart.begin(115200);

	while (1){

//		usart.printf("负载电压：%.2f V | ",bv);
//		usart.printf("负载电流：%.2f ma | ",(float)current/1000);
//		usart.printf("负载功率：%.2f mw RAW:%d \r\n",(float)p/1000,p);
		delay_ms(2000);
//		usart.write(0);
//		usart.printf("ceshi1");
	}
}










