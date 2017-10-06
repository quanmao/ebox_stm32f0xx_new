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

int read(E_PinID id)
{
	// BUFFER TO RECEIVE
	uint8_t bits[5];
	uint8_t cnt = 7;
	uint8_t idx = 0;
	
	//E_GPIO p(id);

	// EMPTY BUFFER
	for (int i=0; i< 5; i++) bits[i] = 0;

	// REQUEST SAMPLE
	//pinMode(pin, OUTPUT);
	p.mode(OUTPUT_OD);
	p.reset();
	//digitalWrite(pin, LOW);
	delay_ms(18);
	p.set();
	//digitalWrite(pin, HIGH);
	delay_us(40);
	//p.mode(INPUT);

	// ACKNOWLEDGE or TIMEOUT
	unsigned int loopCnt = 10000;
	while(p.read() == LOW)
		if (loopCnt-- == 0){ 
			usart.printf(" time out r \r\n");
			return DHTLIB_ERROR_TIMEOUT;
		}

	loopCnt = 10000;
	while(p.read() == HIGH)
		if (loopCnt-- == 0) return DHTLIB_ERROR_TIMEOUT;

	// READ OUTPUT - 40 BITS => 5 BYTES or TIMEOUT
	for (int i=0; i<40; i++)
	{
		loopCnt = 10000;
		while(p.read() == LOW)
			if (loopCnt-- == 0) return DHTLIB_ERROR_TIMEOUT;
		//usart.printf(" load is %d\r\n",loopCnt);
		
		loopCnt = 0;
		while(p.read() == HIGH  && loopCnt++ < 10000){
				delay_us_32(1);
		}
			if (loopCnt>=10000){
				usart.printf("idx =  %d,i = %d\r\n",idx,i);
				return DHTLIB_ERROR_TIMEOUT;
			}
		//usart.printf(" time is %d\r\n",loopCnt);
		if (loopCnt > 20) bits[idx] |= (1 << cnt);
			
		if (cnt == 0)   // next byte?
		{
			cnt = 7;    // restart at MSB
			idx++;      // next byte!
		}
		else cnt--;
	}

	// WRITE TO RIGHT VARS
        // as bits[1] and bits[3] are allways zero they are omitted in formulas.
	humidity    = bits[0]; 
	temperature = bits[2]; 

	uint8_t sum = bits[0] + bits[2];  

	if (bits[4] != sum) return DHTLIB_ERROR_CHECKSUM;
	return DHTLIB_OK;
}


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




