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


//#include "ebox.h"
//#include <stdio.h>
//#include "LiquidCrystal_I2C.h"
////#include "driver_led.h"
////#include "filter.h"
//#include "INA219.h"


///**
// * 1  创建GPIO对象，类名E_GPIO, 参数为PIN_ID; 类似于PA_5形式
// * 2	设置GPIO模式，作为通用GPIO使用时不能配置af功能。可用类型在stm32_define.h中，参考枚举变量 PIN_MODE
// * 3  读写操作
//*/

////E_GPIO PA5(PA_5);


////E_I2c ic(I2C1,new E_PinBase(PB_8),new E_PinBase(PB_9));
//////E_UART uart1(USART1,new E_PinBase(PA_9),new E_PinBase(PA_10));

//////E_GPIO PA0(PA_0);
//////E_GPIO Bu(PC_13);
////////oneWrite ds18(&PA0);
////LiquidCrystal_I2C lcd1(&ic,0x7e,16,2);


////void setup()
////{
////    ebox_init();
////		PA5.mode(OUTPUT_PP);

//////		uart1.begin(115200);
//////		printf("________________\r\n");
//////		Bu.mode(OUTPUT_PP_PU);
//////		Bu.set();
//////	   ic.begin(100);

////		lcd1.init();
////		lcd1.backlight();
////		lcd1.cursor();
////	  lcd1.blink();
////		//lcd1.begin(16,2);
////}


//E_GPIO PA5(PA_5);
//E_I2c ic(I2C1,PB_8,PB_9);
//INA219 ina219(&ic,0x80);
//LiquidCrystal_I2C lcd1(&ic,0x7e,16,2);
//E_UART usart(USART1,PA_9,PA_10);

//void rxirq(void){
//	PA5 = 1;
//}

//int main(){
//	float v;
//	
//	  ebox_init();
//	  usart.begin(115200);
//	  PA5.mode(OUTPUT_PP);
//		ic.begin(100);
//		ina219.begin();
//		ina219.autoCalibration(16,400,100);
//		usart.attach(&PA5,&E_GPIO::reset,TxIrq);
//		usart.attach(&rxirq,RxIrq);
//		usart.enable_irq(TxIrq);
//		usart.enable_irq(RxIrq);
//	
//		lcd1.init();
//		lcd1.backlight();
////		lcd1.cursor();
////	  lcd1.blink();
////		lcd1.begin(16,2);
////		lcd1.scrollDisplayRight();
//	
//		//uint8_t i;
//		while(1){
//			//usart.print("test \r\n");
//			lcd1.clear();
//			v = ina219.getBusVoltage_V();
//			lcd1.setCursor(0,0);
//			lcd1.print("Voltage: ");
//			lcd1.print(v);
//			lcd1.print(" V");
//			
//			usart.print("负载电压：");
//			usart.print(v);
//			usart.println();

//			while(ina219.getState()!=VALID){
//			usart.print("转换失败：\r\n");
//				delay_ms(1000);
//			}
//			v = ina219.getShuntVoltage_uV();
//			usart.print("分流电压：");
//			usart.print((uint16_t)v);
//			usart.print("uV");
//			usart.println();
////			printf("分流电压：%d uV \r\n",(int16_t)v);
////			
//			v = ina219.getCurrent_uA();
//			lcd1.setCursor(0,1);
//			lcd1.print("Current: ");
//			lcd1.print((int16_t)v/1000);			
//			lcd1.print(" mA");
//			usart.print("负载电流：");
//			usart.print((uint16_t)(v/1000));
//				usart.print("ma");
//			usart.println();
////			printf("负载电流：%d ua \r\n",(int16_t)v);
////			v = ina219.getPower_uw();
////			printf("负载功率：%d uw \r\n",(int16_t)v);
//			delay_ms(2000);
//			//PA5.toggle();
//			//PA5.toggle();
//		}
//		
//}


