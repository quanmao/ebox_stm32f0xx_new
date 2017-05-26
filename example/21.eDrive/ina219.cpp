/**
  ******************************************************************************
  * @file   : *.cpp
  * @author : cat_li
  * @version: V2.0
  * @date   : 2016/11/10

  * @brief   ebox application example .
  *
  * Copyright 2016 shentq. All Rights Reserved.
  ******************************************************************************
 */
 
#include "ebox.h"
#include <stdio.h>
#include "LiquidCrystal_I2C.h"
#include "driver_led.h"
#include "filter.h"
#include "INA219.h"

E_GPIO PA5(PA_5);
E_I2c ic(I2C1,new E_PinBase(PB_8),new E_PinBase(PB_9));
INA219 ina219(&ic,0x80);
E_UART usart(USART1,new E_PinBase(PA_9),new E_PinBase(PA_10));

int main(){
	float v;
	
	  ebox_init();
	  usart.begin(115200);
	  PA5.mode(OUTPUT_PP);
		ina219.begin();
		ina219.autoCalibration(16,1000,100);
		while(1){
			v = ina219.getBusVoltage_V();
			printf("负载电压：%.2f v \r\n",v);

			while(ina219.getState()!=VALID){
				printf("转换失败 \r\n");
				delay_ms(1000);
			}
			v = ina219.getShuntVoltage_uV();
			printf("分流电压：%d uV \r\n",(int16_t)v);
			
			v = ina219.getCurrent_uA();
			printf("负载电流：%d ua \r\n",(int16_t)v);
			v = ina219.getPower_uw();
			printf("负载功率：%d uw \r\n",(int16_t)v);
			delay_ms(2000);
			PA5.toggle();
		}
		
}


