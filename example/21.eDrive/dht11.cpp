/*
file   : *.cpp
author : shentq
version: V1.0
date   : 2015/7/5

Copyright 2015 shentq. All Rights Reserved.
*/

//STM32 RUN IN eBox


#include "ebox.h"
#include "dht11.h"

E_UART usart(USART1,PA_2,PA_3);

E_GPIO PB1(PB_1);
E_GPIO p(PB_1);
Dht11 sensor(&PB1);

#define DHTLIB_OK				0
#define DHTLIB_ERROR_CHECKSUM	-1
#define DHTLIB_ERROR_TIMEOUT	-2

	int humidity;
	int temperature;




void setup()
{
    ebox_init();
    usart.begin(115200);
}
int main(void)
{
		uint8_t i,j = 250;
    setup();

    usart.printf("DHT11 TEST PROGRAM \r\n");
    usart.printf("LIBRARY VERSION: ");
    usart.printf(Dht11::VERSION);
    usart.printf("\r\n");
	while (1)
	{
		no_interrupts();
		if (read(PB_0) == 0)
		{
			interrupts();
			usart.printf("h = %d t = %d \r\n",humidity,temperature);
		}
		delay_ms(2000);
	}
	
//		while(1){
//			for(i = 251;i!=250;i++){
//				usart.printf("test over = %d,i=%d\r\n",(uint8_t)(i-j),i);
//				delay_ms(1000);
//			}
//		}
	
    while(1)
    {
        switch (sensor.read())
        {
        case Dht11::OK:
            usart.printf("Humidity: %d\%% \r\n", sensor.getHumidity());
            usart.printf("Temperature: %d'C \r\n ", sensor.getTemperature());
            break;

        case Dht11::ERROR_CHECKSUM:
            usart.printf("Checksum error\r\n");
            break;

        case Dht11::ERROR_TIMEOUT:
            usart.printf("Timeout error\r\n");
            break;

        default:
            usart.printf("Unknown error\r\n");
            break;
        }

        delay_ms(1000);
    }
}




