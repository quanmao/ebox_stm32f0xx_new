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
 * 1  ����GPIO��������E_GPIO, ����ΪPIN_ID; ������PA_5��ʽ
 * 2	����GPIOģʽ����Ϊͨ��GPIOʹ��ʱ��������af���ܡ�����������stm32_define.h�У��ο�ö�ٱ��� PIN_MODE
 * 3  ��д����
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

// ���嵥���˿�
E_GPIO PA5(PA_5);
// ����ͬһ��port�����ɶ˿�,�����ͬһ���˿ڵ�����IO; PA0-PA3
//E_PORT P_PORT(E_PORTA,0x0f);
// ����ͬport���ɶ˿ڴ������Ϊһ��������в���;�ʺϲ�ͬport��IO��ͬһport��IO���ʹ��
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
    // ��Ҫ�ر�ע�⣬��д��ֵ��IO�Ķ�Ӧ��ϵ��û�ж����IOҲҪ���ǽ���
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



