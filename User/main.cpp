/**
  ******************************************************************************
  * @file   : PowerMeter.cpp
  * @author : cat_li
  * @version: V2.0
  * @date   : 2017/9/18
  * @brief   硬件STM32F030,LCD1602@I2C,INA219; soft by ebox
  *			2017/10/6 	add humidity and temperature
  * Copyright 2016 shentq. All Rights Reserved.
  ******************************************************************************
 */


#include "ebox.h"
#include <stdio.h>
#include "LiquidCrystal_I2C.h"
#include "INA219.h"
#include "ebox_debug.h"
#include "dht11.h"

#define IC1  I2C1,I2C1_SCL,I2C1_SDA

// i2c 
E_I2C ic(IC1);
// i2c device ina219 and lcd1602
INA219 ina219(&ic,0x80);
LiquidCrystal_I2C lcd1(&ic,0x4e,16,2);
// dht11
dht11 dht(PA_6);
// usart
//E_UART usart(USART1,PA_2,PA_3);

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
			lcd1.printf("%4dmA",(uint16_t)c);
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
	lcd1.clear();

	lcd1.setCursor(0,0);
	lcd1.printf("V:");

	lcd1.setCursor(8,0);
	lcd1.printf("I:");

	lcd1.setCursor(0,1);
	lcd1.printf("P:");
}

void displayht(void){
		if(dht.read() == DHTLIB_OK)
		{			
			lcd1.clear();
			lcd1.setCursor(0,0);
			lcd1.printf("H=%2d%% T=%2dC",dht.humidity,dht.temperature);
		}
}

#define DISPLAYHT	0		//显示环境温度
#define GETCURRENT	1		//获取电压电流参数
#define DISPLAYCVP	2		//显示电压，电流参数
#define SWITCH		3		//切换量程

void swtichCalibration()
{
	uint32_t c = abs(current/1000);
	if (c < 300 )
	{
		bv >= 16 ? (ina219.autoCalibration(32,400,100)) : (ina219.autoCalibration(16,400,100));
	}else if ( c < 700)
	{
		bv >= 16 ?(ina219.autoCalibration(32,800,100)) : (ina219.autoCalibration(16,800,100));
	}else if ( c < 1500)
	{
		bv >= 16 ?(ina219.autoCalibration(32,1600,100)) : (ina219.autoCalibration(16,1600,100));
	}else if( c < 3200 ){
		bv >= 16 ?(ina219.autoCalibration(32,3200,100)) : (ina219.autoCalibration(16,3200,100));
	}else if( c > 3200){
		lcd1.setCursor(10,0);
		lcd1.printf("     ");
		lcd1.setCursor(10,0);
		lcd1.printf("OVER!");
	}
}

int main(){

	uint8_t state = DISPLAYHT;
	bool flag = true;

	ebox_init();
//	usart.begin(115200);

	ic.begin(100);


	lcd1.init();
	displayht();
	//lcd1.begin(16,2);
	lcd1.backlight();


	ina219.begin();
	ina219.autoCalibration(16,1000,100);

	while (1){

		switch (state)
		{
		case DISPLAYHT:			
			displayht();
			state = GETCURRENT;
			break;
		case GETCURRENT:
			bv = ina219.getBusVoltage_V();
			current = ina219.getCurrent_uA();
			p = ina219.getPower_uw();
			if (current/1000 > 5)
			{
				state = DISPLAYCVP;
			}else{
				flag = true;
				state = DISPLAYHT;
			}
			break;
		case DISPLAYCVP:
			if (flag)
			{
				displayInit();
				flag = false;
			}
			displayUpdate();
			state = SWITCH;
			break;
		case SWITCH:
			swtichCalibration();
			state = GETCURRENT;
			break;
		default:
			break;
		}

		while (ina219.getState()!=VALID){
			delay_ms(1000);
		}
		delay_ms(2000);
	}
}










