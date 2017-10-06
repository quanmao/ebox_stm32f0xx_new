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

E_GPIO PA0(PA_0);
E_GPIO PA4(PA_4);
E_GPIO PA5(PA_5);

E_GPIO PA2(PA_2);
Dht11 sensor(&PA0);

E_UART usart(USART1,PA_2,PA_3);

void setup()
{
    ebox_init();
    usart.begin(115200);
		PA0.mode(OUTPUT_PP);
	while(1){
		//PA0 = 1;
//		PA0.write(1);
//		PA0.set();
//		delay_us(5);
		PA0.toggle();
		//delay_us_32(5);
		//PA0 = 0;
//		PA0.write(0);
//		PA0.reset();
		//delay_us_32(1);
		//delay_us(1);
	}

}

E_GPIO PA7(PA_4);

void 
DHT11Reset(void)
{
    //
    // Set DHT11_PIN_DATA as OUTPUT,Host send start singal to DHT11
    //
    //xGPIOSPinTypeGPIOOutput(DHT11_PIN_DATA);
	PA7.mode(OUTPUT_OD);
    //
    // Set DHT11_PIN_DATA as low, Hold at least 18ms
    //
	PA7.reset();
    //xGPIOSPinWrite(DHT11_PIN_DATA, 0);
    //xSysCtlDelay(150000);
	delay_ms(18);
    //
    // Set DHT11_PIN_DATA as high, Hold 20~40us
    //	
    //xGPIOSPinWrite(DHT11_PIN_DATA, 1);
	PA7.set();
    //xSysCtlDelay(150);
	delay_us(20);
}

unsigned char 
DHT11Check(void)
{
    unsigned char ucRetry=0;

    //
    // Set DHT11_PIN_DATA as Input
    //
    //xGPIOSPinTypeGPIOInput(DHT11_PIN_DATA);
	
	PA7.mode(INPUT);
	if(PA7.read()== 0){
    //
    // Wait DHT11_PIN_DATA is low, Hold 40~80us
	// 复位后80us内可以检测到响应（低电平）
    //	
//    while(PA7.read() && (ucRetry < 8))
//    {
//        ucRetry++;
//        delay_us(10);
//    };
		while(!PA7.read());

//    if(ucRetry >= 8) 
//    {
//    	return 1;	// 设备不存在
//    }
//    else
//    {
//    	ucRetry=0;
//    }

    //
    // Wait DHT11_PIN_DATA is high, Hold 40~80us
	// DHT拉高，准备读取数据
    //	
//    while((!PA7.read()) && (ucRetry < 8))
//    {
//        ucRetry++;
//        delay_us(10);
//    }

//    if(ucRetry >= 8)
//    {
//    	return 1;
//    }
		while(PA7.read());
		return 0;
	}else{
    return 1;
	}
}



//*****************************************************************************
//
//! \brief Read one bit from DHT11.
//!
//! \param None.
//!
//! \return 1 or 0 according to physical truth.
//
//*****************************************************************************
unsigned char DHT11ReadBit(void)
{
    unsigned char ucRetry = 0;
    //
    // Wait DHT11_PIN_DATA is low, Hold 40~80us
    //
    // while(PA7.read() && (ucRetry<6))
    // {
        // ucRetry++;
        // delay_us(10);
    // }
    
    //Wait DHT11_PIN_DATA is high, Hold 40~80us
    
    // ucRetry = 0;
    // while(!PA7.read() && (ucRetry<6))
    // {
        // ucRetry++;
        // delay_us(10);
    // }
	while (!PA7.read());	// 度过50us低电平起始信号

    //
	// Delay 40us
	//
    //xSysCtlDelay(210);
	delay_us(30);
    if(PA7.read())
    {
    	return 1;
    }
    else
    {
    	return 0;
    }
}

//*****************************************************************************
//
//! \brief Read one byte from DHT11.
//!
//! \param None.
//!
//! \return one byte according to physical truth.
//
//*****************************************************************************
unsigned char
DHT11ReadByte(void)
{
    unsigned char i, ucData;
    ucData = 0;

    for(i=0; i<8; i++)
    {
    	ucData <<= 1;
    	ucData |= DHT11ReadBit();
    }

    return ucData;
}

//*****************************************************************************
//
//! \brief Read the values of temperature and humidity.
//!
//! \param ucpTemp is the storage of temperature value.
//! \param ucpHumi is the storage of temperature value.
//!
//! \return xtrue if read DHT11 successfully, xfalse if not.
//
//*****************************************************************************
unsigned char
DHT11ReadData(unsigned char *ucpTemp, unsigned char *ucpHumi)
{
    unsigned char ucBuf[5];
    unsigned char i;

    DHT11Reset();
    if(DHT11Check()==0)
    {
        for(i=0; i<5; i++)
        {
            ucBuf[i] = DHT11ReadByte();
        }

        if((ucBuf[0] + ucBuf[1] + ucBuf[2] + ucBuf[3]) == ucBuf[4])
        {
            *ucpHumi = ucBuf[0];
            *ucpTemp = ucBuf[2];
        }
    }
    else
    {
    	return 1;
    }

    return 0;
}

uint8_t read(){
	uint8_t i,in_data,k;
	
	for (i=0;i<8;i++){
		
		while (PA7==0);
		delay_us(45);
		k = PA7;
		
		if (k==1)
		{
			in_data = in_data|0x01;
			while (PA7 == 1);
			//delay_us(20);
		}else{
			in_data = in_data|0x00;
		}
		
		in_data = in_data<<1;
	}
	return in_data;
}


int main(void)
{
	uint8_t i,j;
    setup();
//		DHT11Reset();
	  unsigned char ucBuf[5];
    //unsigned char i;

    usart.printf("DHT11 TEST PROGRAM \r\n");
//    usart.printf("LIBRARY VERSION: ");
//    usart.printf(Dht11::VERSION);
//    usart.printf("\r\n");
		PA7.mode(OUTPUT_OD);
    while(1)
    {
//					if( DHT11ReadData(&i,&j)== 0){
//			    usart.printf("DHT11 is %d %d \r\n",i,j);
//					}
			PA7 = 0;
			delay_ms(20);
			PA7 = 1;
			delay_us(40);
			if(PA7.read() == 0)
			{
				usart.printf("read 0 ");
				while(!PA7);
				usart.printf("end 0 ");
				while(PA7);
				usart.printf("end 1 \r\n");
				
				for(i=0; i<5; i++)
        {
            //ucBuf[i] = DHT11ReadByte();
					 ucBuf[i] = read();
        }

        if((ucBuf[0] + ucBuf[1] + ucBuf[2] + ucBuf[3]) == ucBuf[4])
        {
            //*ucpHumi = ucBuf[0];
						usart.printf("humi = %d  | ",ucBuf[0]);
						usart.printf("TEMP = %d  | ",ucBuf[2]);
            //*ucpTemp = ucBuf[2];
        }
			}
			delay_ms(2000);
					
					
//        switch (sensor.read())
//        {
//        case Dht11::OK:
//            usart.printf("Humidity: %d\%% \r\n", sensor.getHumidity());

//            usart.printf("Temperature: %d'C \r\n ", sensor.getTemperature());
//            break;

//        case Dht11::ERROR_CHECKSUM:
//            usart.printf("Checksum error\r\n");
//            break;

//        case Dht11::ERROR_TIMEOUT:
//            usart.printf("Timeout error\r\n");
//            break;

//        default:
//            usart.printf("Unknown error\r\n");
//            break;
//        }

        delay_ms(1000);
    }
}




