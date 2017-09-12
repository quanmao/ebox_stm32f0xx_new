/**
 ******************************************************************************
 * @file   : tm770x example.cpp
 * @author : cat_li
 * @version: V1.2
 * @date   : 2017/07/8
 *
 * @brief   stm32f0.
 *
 * Copyright 2016 shentq. All Rights Reserved.
 ******************************************************************************
 */

//STM32 RUN IN eBox
#include "ebox.h"
#include "TM770x.h"

//// DIN  PA7(MOSI)
//// DOUT PA6(MISO)
//// CLK  PA5(SCLK)
//// DRDY PA1(低电平可以读取数据,高电平正在转换)
//// CS   PB1(片选)
//// RST  PB2(低电平复位)

E_SPI spi1(SPI1,PA_5,PA_6,PA_7);

//TM770X tm7707(&spi1,TM05,PA_1,PB_1,PB_2);
TM770X tm7707(&spi1,TM05,PB_1);
////TM7707 tm7707(&spi1,NULL,&PB1);
//E_UART usart(USART1,PA_9,PA_10);
E_UART usart(USART1,PA_2,PA_3);

void setup()
{
	ebox_init();
	usart.begin(115200);
	usart.printf("测试\r\n");
	tm7707.begin();	

}

int main(void)
{
	uint32_t uiADCValue;
	float uiVoltage;
	setup();

	tm7707.calibself(CH_1);
	tm7707.calibself(CH_2);
	usart.printf("自校准后结果\r\n");
	while (1)
	{

		uiADCValue = tm7707.read(CH_1);
		//uiVoltage =  (2.5*uiADCValue/16777216)*2;
		uiVoltage = (5.0*uiADCValue/(65536));
		usart.printf("CH1:%6x, Voltage:%1.9f | ", uiADCValue, uiVoltage);

//		uiADCValue = tm7707.read(CH_2);
//		//uiVoltage = 5.0*uiADCValue/16777216;
//		uiVoltage = (2.5*uiADCValue/65536)*2;
//		usart.printf("CH2:%6x, Voltage:%1.9f | ", uiADCValue, uiVoltage);
		
		uiADCValue = tm7707.read(CH_2);
		//uiVoltage = 5.0*uiADCValue/16777216;
		uiVoltage = (2.5*uiADCValue/65536)*2;
		usart.printf("CH2:%6x, Voltage:%1.9f \r\n", uiADCValue, uiVoltage);
		delay_ms(5000);
	}
}