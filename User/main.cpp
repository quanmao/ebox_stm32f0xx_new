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
#include "LiquidCrystal_I2C.h"
#include "INA219.h"
#include "ebox_debug.h"

#define IC1  I2C1,I2C1_SCL,I2C1_SDA

E_I2C ic(IC1);
INA219 ina219(&ic,0x80);
LiquidCrystal_I2C lcd1(&ic,0x4e,16,2);
E_UART usart(USART1,PA_2,PA_3);

float bv;
int32_t current;
uint32_t p;
/**
 *@brief    打印功率值到屏幕，根据数值大小调整显示位数及单位
 *@param    none
 *@retval   NONE
*/
void pPrint(float power){
	if (power < 1000)	// mw
	{
		if (power<10)
		{
			lcd1.printf("%.2fmW",power);
		}else if (power<100)
		{
			lcd1.printf("%.1fmW",power);
		}else {
			lcd1.printf("%dmW",(uint16_t)power);
		}
	}else{				// w
		power = power/1000;
		if (power<10)
		{
			lcd1.printf("%.2fW",power);
		}else if (power<100)
		{
			lcd1.printf("%.1fW",power);
		}else {
			lcd1.printf("%dW",(uint16_t)power);
		}
	}
}
/**
 *@brief    打印电压值到屏幕，根据数值大小调整显示位数及单位
 *@param    none
 *@retval   NONE
*/
void vPrint(float v){
	if (v<10){
		lcd1.printf("%.2fV",v);
	}else{
		lcd1.printf("%.1fV",v);
	}
}
/**
 *@brief    打印电流值到屏幕，根据数值大小调整显示位数及单位
 *@param    none
 *@retval   NONE
*/
void cPrint(float c){
	if (c<1000)		// mA
	{
		if (c<10){
			lcd1.printf("%.2fmA",c);
		}else if (c <100){
			lcd1.printf("%.1fmA",c);
		}else{
			lcd1.printf("%dmA",(uint16_t)c);
		}
	}else{			// A,ina219 此配置最大3.2A
		lcd1.printf("%.2fA",c/1000);
	}
}

/**
 *@brief    更新可变值到屏幕，电压，电流，功率
 *@param    none
 *@retval   NONE
*/
void displayUpdate(){
	static float obv;
	static int32_t ocurrent;
	static uint32_t op;

	if (obv != bv){
		lcd1.setCursor(2,0);
		lcd1.printf("     ");
		lcd1.setCursor(2,0);
		vPrint(bv);
		obv = bv;
	}

	if (ocurrent != current){
		lcd1.setCursor(10,0);
		lcd1.printf("     ");
		lcd1.setCursor(10,0);
		cPrint((float)current/1000);
		ocurrent= current;
	}

	if (op != p){
		lcd1.setCursor(2,1);
		lcd1.printf("      ");
		lcd1.setCursor(2,1);
		pPrint((float)p/1000);
		op = p;
	}
}


/**
 *@brief    初始化lcd1602,并将不变的部分写入
 *@param    none
 *@retval   NONE
*/
void displayInit(){
	lcd1.init();
	lcd1.backlight();
	lcd1.begin(16,2);

	lcd1.clear();

	lcd1.setCursor(0,0);
	lcd1.printf("V:");

	lcd1.setCursor(8,0);
	lcd1.printf("I:");

	lcd1.setCursor(0,1);
	lcd1.printf("P:");
}

int main(){
//
	ebox_init();
	usart.begin(115200);
//	PA5.mode(OUTPUT_PP);
	ic.begin(100);
	ina219.begin();
	ina219.autoCalibration(16,1000,100);
	displayInit();


	while (1){
		bv = ina219.getBusVoltage_V();
		while (ina219.getState()!=VALID){
			usart.printf("转换失败：\r\n");
			delay_ms(1000);
		}

		current = ina219.getCurrent_uA();
		p = ina219.getPower_uw();

		displayUpdate();
//		usart.printf("负载电压：%.2f V | ",bv);
//		usart.printf("负载电流：%.2f ma | ",(float)current/1000);
//		usart.printf("负载功率：%.2f mw RAW:%d \r\n",(float)p/1000,p);
		delay_ms(2000);
	}
}










