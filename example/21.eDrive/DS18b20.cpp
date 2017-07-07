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
#include "oneWire.h"

/**
 * 1  ����GPIO��������E_GPIO, ����ΪPIN_ID; ������PA_5��ʽ
 * 2	����GPIOģʽ����Ϊͨ��GPIOʹ��ʱ��������af���ܡ�����������stm32_define.h�У��ο�ö�ٱ��� PIN_MODE
 * 3  ��д����
*/

E_GPIO PA5(PA_5);

E_I2c ic(I2C1,PB_8,PB_9);
E_UART uart1(USART1,PA_9,PA_10);

E_GPIO PA0(PA_0);
E_GPIO Bu(PC_13);
//oneWrite ds18(&PA0);
OneWire ds18(PA_0);


void setup()
{
    ebox_init();
		PA5.mode(OUTPUT_PP);
		uart1.begin(115200);
		printf("________________\r\n");
		Bu.mode(OUTPUT_PP_PU);
		Bu.set();
		
}


int main(void)
{
		uint16_t t;
		uint8_t i = 0x10 ,add = 0x46,bu[2],rom[8];
    setup();
//		ic.begin(100);
	
//		PA0.mode(OUTPUT_PP_PU);
    while (1)
    {
//			PA0.toggle();
				//delay_us_32(0);
				//delay_us(1);
//			ic.writeChar(add,0x01);
//			ic.writeChar(add,0x10);

//			delay_ms(500);
//			ic.readBuf(add,bu,2);
//			t = (bu[0]<<8)+ bu[1];
////			t = (float)t/1.2;
//			printf("�ն� = %.2f \r\n",(float)t/1.2);
//			i = Bu; 
//			i = i+1;
//			i = Bu;
////			no_interrupts();
//			
			ds18.reset_search();
			i = ds18.search(rom);
			
			if(i == 1){
					printf("�����ɹ���");
					for(i = 0;i<8;i++){
						printf(" %x",rom[i]); 
					}
					printf("\r\n");
			}
			
			i = ds18.reset();
			ds18.writeChar(0xcc);
			ds18.writeChar(0x44);
			
			i = ds18.reset();
			ds18.writeChar(0xcc);
			ds18.writeChar(0xbe);
			
			bu[0] = ds18.readChar();
			bu[1] = ds18.readChar();
//			interrupts();


if(bu[1]>7)	{
	bu[0] = ~bu[0];
	bu[1] = ~bu[1];	
}
	t = (uint16_t)bu[1]<<8 | bu[0];
			
			printf("�¶� = %.2f \r\n",(float)t*0.0625);
			delay_ms(5000);

			PA5.toggle();			
    }
}




