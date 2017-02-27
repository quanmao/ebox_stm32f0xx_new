/**
  ******************************************************************************
  * @file    spi.cpp
  * @author  shentq
  * @version V1.2
  * @date    2016/08/14
  * @brief
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
/**
 * Modification History:
 * -LQM (2016/9/5)
 *      *移植到STM32F0,基于HAL库LL层,支持SPI1,SPI2
 */

/* Includes ------------------------------------------------------------------*/
#include "spi.h"


Spi::Spi(SPI_TypeDef *SPIx, Gpio *sck, Gpio *miso, Gpio *mosi)
{
	busy = 0;
	spi = SPIx;
	sck->mode(AF_PP_PD,LL_GPIO_AF_0);
	miso->mode(AF_PP_PD,LL_GPIO_AF_0);
	mosi->mode(AF_PP_PD,LL_GPIO_AF_0);

};

void Spi::begin(SPI_CONFIG_TYPE *spi_config)
{
	/* Enable the peripheral clock */
	if (spi == SPI1)
	{
		LL_APB1_GRP2_EnableClock(LL_APB1_GRP2_PERIPH_SPI1);
	}
#ifdef SPI2
    if(spi == SPI2)
    {
		LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_SPI2);
    }
#endif
	config(spi_config);
}

void Spi::config(SPI_CONFIG_TYPE *spi_config)
{
	current_dev_num = spi_config->dev_num;
	LL_SPI_Disable(spi);

	if (spi_config->mode == SPI_MODE0)
	{
		LL_SPI_SetClockPhase(spi, LL_SPI_PHASE_1EDGE);
		LL_SPI_SetClockPolarity(spi, LL_SPI_POLARITY_LOW);
	}
	else if (spi_config->mode == SPI_MODE1)
	{
		LL_SPI_SetClockPhase(spi, LL_SPI_PHASE_1EDGE);
		LL_SPI_SetClockPolarity(spi, LL_SPI_POLARITY_LOW);
	}
	else if (spi_config->mode == SPI_MODE2)
	{
		LL_SPI_SetClockPhase(spi, LL_SPI_PHASE_1EDGE);
		LL_SPI_SetClockPolarity(spi, LL_SPI_POLARITY_HIGH);
	}
	else if (spi_config->mode == SPI_MODE3)
	{
		LL_SPI_SetClockPhase(spi, LL_SPI_PHASE_2EDGE);
		LL_SPI_SetClockPolarity(spi, LL_SPI_POLARITY_HIGH);
	}

	/* Configure SPI1 communication */
	LL_SPI_SetBaudRatePrescaler(spi, spi_config->prescaler);
	LL_SPI_SetTransferBitOrder(spi, spi_config->bit_order);

	LL_SPI_SetTransferDirection(spi,LL_SPI_FULL_DUPLEX);
	LL_SPI_SetDataWidth(spi, LL_SPI_DATAWIDTH_8BIT);
	LL_SPI_SetNSSMode(spi, LL_SPI_NSS_SOFT);
//  LL_SPI_SetRxFIFOThreshold(SPI1, LL_SPI_RX_FIFO_TH_QUARTER);

	LL_SPI_SetMode(spi, LL_SPI_MODE_MASTER);
	LL_SPI_Enable(spi);
}

uint8_t Spi::read_config(void)
{
	return current_dev_num;
}


int8_t Spi::write(uint8_t data)
{
//    __IO uint8_t dummyByte;
	while ((spi->SR & LL_SPI_SR_TXE) == RESET)
		;
	*((__IO uint8_t *)&spi->DR) = data;
//		spi->DR = data;
//		LL_SPI_TransmitData8(spi, data);
//   while ((spi->SR & LL_SPI_SR_RXNE) == RESET)
//       ;
//		dummyByte = LL_SPI_ReceiveData8(spi);

	return 0;
}
int8_t Spi::write(uint8_t *data, uint16_t data_length)
{
	__IO uint8_t dummyByte;
	if (data_length == 0)
		return -1;
	while (data_length--)
	{
		while ((spi->SR & LL_SPI_SR_TXE) == RESET)
			;
		*((__IO uint8_t *)&spi->DR) = *data;
//        while ((spi->SR & SPI_I2S_FLAG_RXNE) == RESET)
//            ;
//        dummyByte = spi->DR;
	}
	return 0;
}
uint8_t Spi::read()
{
	while ((spi->SR & LL_SPI_SR_TXE) == RESET)
		;
	*((__IO uint8_t *)&spi->DR) = 0xff;
//    while ((spi->SR & SPI_I2S_FLAG_TXE) == RESET)
//        ;
//    spi->DR = 0xff;
	while ((spi->SR & LL_SPI_SR_RXNE) == RESET)
		;
	return(spi->DR);

}
int8_t Spi::read(uint8_t *recv_data)
{
	while ((spi->SR & LL_SPI_SR_TXE) == RESET)
		;
	*((__IO uint8_t *)&spi->DR) = 0xff;
	while ((spi->SR & LL_SPI_SR_RXNE) == RESET)
		;
	*recv_data = spi->DR;

	return 0;
}

int8_t Spi::read(uint8_t *recv_data, uint16_t data_length)
{
	if (data_length == 0)
		return -1;
	while (data_length--)
	{
		while ((spi->SR & LL_SPI_SR_TXE) == RESET)
			;
		*((__IO uint8_t *)&spi->DR) = 0xff;
		while ((spi->SR & LL_SPI_SR_RXNE) == RESET)
			;
		*recv_data = spi->DR;
	}
	return 0;
}

int8_t Spi::take_spi_right(SPI_CONFIG_TYPE *spi_config)
{
	while ((busy == 1) && (spi_config->dev_num != read_config()))
		delay_ms(1);
	if (spi_config->dev_num == read_config())
	{
		busy = 1;
		return 0;
	}
	config(spi_config);
	busy = 1;
	return 0;
}
int8_t Spi::release_spi_right(void)
{
	busy = 0;
	return 0;
}



