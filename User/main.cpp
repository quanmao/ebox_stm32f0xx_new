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

/**
 * 1  创建GPIO对象，类名E_GPIO, 参数为PIN_ID; 类似于PA_5形式
 * 2	设置GPIO模式，作为通用GPIO使用时不能配置af功能。可用类型在stm32_define.h中，参考枚举变量 PIN_MODE
 * 3  读写操作
*/

//E_GPIO PA5(PA_5);


//E_I2c ic(I2C1,new E_PinBase(PB_8),new E_PinBase(PB_9));
////E_UART uart1(USART1,new E_PinBase(PA_9),new E_PinBase(PA_10));

////E_GPIO PA0(PA_0);
////E_GPIO Bu(PC_13);
//////oneWrite ds18(&PA0);
//LiquidCrystal_I2C lcd1(&ic,0x7e,16,2);


//void setup()
//{
//    ebox_init();
//		PA5.mode(OUTPUT_PP);

////		uart1.begin(115200);
////		printf("________________\r\n");
////		Bu.mode(OUTPUT_PP_PU);
////		Bu.set();
////	   ic.begin(100);

//		lcd1.init();
//		lcd1.backlight();
//		lcd1.cursor();
//	  lcd1.blink();
//		//lcd1.begin(16,2);
//}


//int main(void)
//{
//    setup();

//    while (1)
//    {
//     lcd1.setCursor(1,1);
//			delay_ms(5000);
//			PA5.toggle();			
//    }
//}

// 定义单个端口
E_GPIO PA5(PA_5);
// 定义同一个port的若干端口,最好是同一个端口的连续IO; PA0-PA3
//E_PORT P_PORT(E_PORTA,0x0f);
// 将不同port若干端口打包，作为一个整体进行操作;适合不同port的IO或同一port的IO组合使用
E_BUS P_BUS(PB_0,PB_3,PA_6,PA_7);
// PA1-PA4
E_PORT P_PORT(E_PORTA,3,1);

void setup()
{
    ebox_init();
    PA5.mode(OUTPUT_PP);
    P_PORT.mode(OUTPUT_PP);
    P_BUS.mode(OUTPUT_PP);

}

void operationPort(void){
    uint16_t r;
    P_PORT.resetAll();
    // 需要特别注意，所写入值与IO的对应关系。没有定义的IO也要考虑进来
    P_PORT.write(0x01); //PA0 = 1
    P_PORT.write(0x03); //PA0=1,PA1 = 1
    P_PORT.resetAll();
    r = P_PORT.read();
    P_PORT = 0x01;
    r = P_PORT;
    P_PORT = r+3;//PA3 = 1
}

void operationBus(){
    uint16_t r;
    r = P_BUS.read();
    P_BUS[0].set(); // PB0 = 1
    r = P_BUS.read();
    P_BUS.resetAll(); // PB_0,PB_3,PA_6,PA_7 = 0
    P_BUS = 0x02; // pb3 =1
		r = P_BUS;
    P_BUS.write(0x0f); //    PB_0,PB_3,PA_6,PA_7 = 1
    P_BUS = r;
}

int main(void)
{
    setup();

    while (1)
    {
//				P_PORTB.setAll();
        operationBus();
        operationPort();
        delay_ms(500);
        PA5.toggle();
    }
}



