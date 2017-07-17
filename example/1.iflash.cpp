/*
file   : *.cpp
author : shentq
version: V1.0
date   : 2015/7/5

Copyright 2015 shentq. All Rights Reserved.
*/

//STM32 RUN IN eBox


/*
一个独立看门狗的示例程序
*/
#include "ebox.h"

E_Flash  Fh(1);

// 串口，led
E_UART usart(USART1,PA_9,PA_10);
E_GPIO led(PA_5);


void setup()
{
	ebox_init();
	usart.begin(115200);
	usart.printf("E_FLASH test \r\n");
}

uint8_t buf[] = "flash test -  ";
char rec[20];

int main(void)
{
	int i,j;
	setup();
//  i = Fh.write(FLASH_USER_START_ADDR,buf,sizeof(buf)/sizeof(buf[0]));
//	i = Fh.write_without_check(FLASH_USER_START_ADDR,buf,22);
//  Fh.read(FLASH_USER_START_ADDR,(uint8_t*)rec,i);
//	usart.write(rec,i);
	while (1)
	{	
			while(j<15){
				j++;
				buf[13] = j+0x30;
				i = Fh.write(FLASH_USER_START_ADDR,buf,sizeof(buf)/sizeof(buf[0]));
				delay_ms(10);
				Fh.read(FLASH_USER_START_ADDR,(uint8_t*)rec,i);
			 usart.write(rec,i-1);
			 usart.printf("\r\n");
			 delay_ms(2000);
			}
	}
}




