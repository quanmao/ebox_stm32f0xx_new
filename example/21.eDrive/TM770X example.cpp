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

TM770X tm7707(&spi1,TM05,PA_1,PB_1,PB_2);
////TM7707 tm7707(&spi1,NULL,&PB1);
E_UART usart(USART1,PA_9,PA_10);

void setup()
{
	ebox_init();
	usart.begin(115200);
	usart.print("ceshi");
	tm7707.begin();	

}

int main(void)
{
	uint32_t uiADCValue;
	float uiVoltage;
	setup();

//	tm7707.calibself(CH_1);
//	tm7707.calibself(CH_2);
	usart.println();
	usart.print("自校准后结果");

	while (1)
	{

		uiADCValue = tm7707.read(CH_1);
		//uiVoltage =  (2.5*uiADCValue/16777216)*2;
		uiVoltage = (5.0*uiADCValue/(65536));
		//uart1.printf("CH1:%6x, Voltage:%1.9f | ", uiADCValue, uiVoltage);
		usart.print("CH1电压：");
		usart.print(uiVoltage);
		usart.print("V");

		uiADCValue = tm7707.read(CH_2);
		//uiVoltage = 5.0*uiADCValue/16777216;
		uiVoltage = (2.5*uiADCValue/65536)*2;
		//uart1.printf("CH2:%6x, Voltage:%1.9f \r\n", uiADCValue, uiVoltage);
		usart.print(" | CH2电压：");
		usart.print(uiVoltage);
		usart.print("V");
		usart.println();
		delay_ms(5000);
	}
}