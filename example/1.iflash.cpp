/**
  ******************************************************************************
  * @file    iflash.cpp
  * @author  cat_li
  * @version V2.0
  * @date    2017/7/17
  * @brief   iflash 例程
  ******************************************************************************
  * @attention
  *
  * No part of this software may be used for any commercial activities by any form 
  * or means, without the prior written consent of shentq. This specification is 
  * preliminary and is subject to change at any time without notice. shentq assumes
  * no responsibility for any errors contained herein.
  * <h2><center>&copy; Copyright 2015 shentq. All Rights Reserved.</center></h2>
  ******************************************************************************
  */

#include "ebox.h"

// 从用户区起始地址创建一个闪存，默认为1 page,大小等于1*pagesize
E_Flash  Fh(0);
// 从用户区起始地址+2page处，创建一个闪存，3 page,大小等于3*pagesize
E_Flash	 fl(2,3);

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
	int i = 20,j=0;
	setup();
	Fh.read(0,(uint8_t*)rec,1);
	usart.printf("flash test; 第 %d 次开机！ \r\n",rec[0]);
	rec[0]++;
	Fh.write(0,(uint8_t*)rec,1);
	while (1)
	{	
			while(j<2){
				j++;
				buf[13] = j+0x30;
				i = fl.write(0,buf,sizeof(buf)/sizeof(buf[0]));
				delay_ms(10);
				if(i>0) {
				fl.read(0,(uint8_t*)rec,i);
				usart.write(rec,i-1);
				usart.printf("\r\n");
				}
			 delay_ms(2000);
			}
	}
}




