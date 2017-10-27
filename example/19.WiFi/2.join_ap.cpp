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


char recv_buf[1024] = {0};
uint16_t len = 0;
bool ret;


void setup()
{
    ebox_init();
    usart.begin(115200);
    usart.printf("esp8266 join ap test\r\n");
    usart.printf("--------------\r\n");

    ret = wifi.begin(&PA4, &usart2, 115200);
    if(ret)
        usart.printf("esp8266 AT_CMD OK\r\n");
	usart.printf("esp8266 join wifi...\r\n");
    ret = wifi.join_ap();
    if(ret)
		usart.printf("esp8266 join wifi OK\r\n");
		

}

int main(void)
{
    setup();

	usart.printf("esp8266 get ipconfig ...\r\n");
    ret = wifi.query_sta_ip(recv_buf);
    if(ret)
    {
        usart.printf("%s\r\n", recv_buf);
    }
    ret = wifi.query_sta_gateway(recv_buf);
    if(ret)
    {
        usart.printf("%s\r\n", recv_buf);
    }
    ret = wifi.query_sta_netmask(recv_buf);
    if(ret)
    {
        usart.printf("%s\r\n", recv_buf);
    }


    while(1);

}