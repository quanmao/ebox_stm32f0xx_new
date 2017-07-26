/*
file   : *.cpp
author : shentq
version: V1.0
date   : 2015/7/5

Copyright 2015 shentq. All Rights Reserved.
*/

//STM32 RUN IN eBox


/*
һ���������Ź���ʾ������
*/
#include "ebox.h"

E_WDG dog;
// ���ڣ�led
E_UART usart(USART1,PA_9,PA_10);
E_GPIO led(PA_5);


void setup()
{
	ebox_init();
	usart.begin(115200);
	usart.printf("reset !!!\r\n");
	dog.begin(26208);
}

int main(void)
{

	setup();

	while (1)
	{
		dog.feed();//�ڳ�����Ӧ����ι��Ӧ���ڶ�ʱ���ж��ж�ʱι��
		usart.printf("running!\r\n");
//        delay_ms(1000);
//        delay_ms(1900);
		delay_ms(25000);
		//        delay_ms(2000);
		//        delay_ms(3000);
	}
}



