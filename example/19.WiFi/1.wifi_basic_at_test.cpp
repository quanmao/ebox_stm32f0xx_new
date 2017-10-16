/*
file   : *.cpp
author : shentq
version: V1.0
date   : 2015/7/5

Copyright 2015 shentq. All Rights Reserved.
*/

//STM32 RUN IN eBox
#include "ebox.h"
#include "ESP8266.h"
#include "esp8266_udp.h"

E_UART usart(USART1,PA_9,PA_10);
E_UART usart2(USART2,PA_2,PA_3);
E_GPIO	PA4(PA_4);


WIFI_UDP udp(&wifi);

char    buf[1024] = {0};

void setup()
{
    ebox_init();
    usart.begin(115200);
    usart.printf("esp8266 basic at cmd test\r\n");
    usart.printf("--------------\r\n");
		
		usart2.begin(115200);
		usart2.printf("test");
    wifi.begin(&PA4, &usart2, 115200);

}


uint8_t mode;
bool ret;
int main(void)
{
    setup();

    ret = wifi.kick();
    if(ret)
        usart.printf("kick ok\r\n");
    else
        usart.printf("kick failed\r\n");

    ret = wifi.get_version(buf);
    if(ret)
        usart.printf("%s\r\n", buf);
    else
        usart.printf("get version failed\r\n");

    while(1);

}
