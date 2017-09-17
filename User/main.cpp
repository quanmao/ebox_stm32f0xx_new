///**
//  ******************************************************************************
//  * @file   : *.cpp
//  * @author : cat_li
//  * @version: V2.0
//  * @date   : 2016/11/10

//  * @brief   ebox application example .
//  *
//  * Copyright 2016 shentq. All Rights Reserved.
//  ******************************************************************************
// */


#include "ebox.h"
#include <stdio.h>
#include "LiquidCrystal_I2C.h"
#include "INA219.h"
#include "ebox_debug.h"

#define IC1  I2C1,I2C1_SCL,I2C1_SDA


E_GPIO PA5(PA_5);
E_GPIO led(PA_5);
E_I2C ic(IC1);
INA219 ina219(&ic,0x80);
LiquidCrystal_I2C lcd1(&ic,0x4e,16,2);
E_UART usart(USART1,PA_2,PA_3);

int main(){
	float bv;
	int32_t current;
	uint32_t p;
//	
	  ebox_init();
	  usart.begin(115200);
	  PA5.mode(OUTPUT_PP);
		ic.begin(100);
		ina219.begin();
		ina219.autoCalibration(16,100,100);
	
		lcd1.init();
		lcd1.backlight();

		lcd1.begin(16,2);

		while(1){
			delay_ms(2000);

			
			bv = ina219.getBusVoltage_V();
			while(ina219.getState()!=VALID){
			usart.printf("转换失败：\r\n");
				delay_ms(1000);
			}
//			x = ina219.getShuntVoltage_uV();
//			usart.printf("分流电压：%.2f mV | ",(float)x/1000);
////	
			
			current = ina219.getCurrent_uA();			
			p = ina219.getPower_uw();
			
			lcd1.clear();
			lcd1.setCursor(0,0);
			lcd1.print("V:");
			lcd1.print(bv);
			lcd1.print("V");
			usart.printf("负载电压：%.2f V | ",bv);
			
			lcd1.setCursor(8,0);
			lcd1.print("I:");
			lcd1.print((float)current/1000);			
			lcd1.print("mA");
			usart.printf("负载电流：%.2f ma | ",(float)current/1000);
			
			lcd1.setCursor(0,1);
			lcd1.print("Power:");
			lcd1.print((float)p/1000,1);			
			lcd1.print("mW");
			usart.printf("负载功率：%.2f mw RAW:%d \r\n",(float)p/1000,p);
		}		
}










