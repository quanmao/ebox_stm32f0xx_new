/**
  ******************************************************************************
  * @file    ebox_i2c.cpp
  * @author  cat_li
  * @version V2.0
  * @date    2016/11/11
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

#include "ebox_spi.h"

E_SPI::E_SPI(SPI_TypeDef *SPIx, E_PinBase *sck, E_PinBase *miso, E_PinBase *mosi){
	_spi = SPIx;
	sck->mode(AF_PP_PU,0);
	miso->mode(AF_PP_PU,0);
	mosi->mode(AF_PP_PU,0);
}

void E_SPI::config(E_SPI_CONFIG_T *spi_config)
{
    /* Enable the peripheral clock */
    if (_spi == SPI1)
    {
        LL_APB1_GRP2_EnableClock(LL_APB1_GRP2_PERIPH_SPI1);
    }
#ifdef SPI2
    if (_spi == SPI2)
    {
        LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_SPI2);
    }
#endif

    LL_SPI_Disable(_spi);

    switch (spi_config->mode)
    {
    case SPI_MODE0:
        LL_SPI_SetClockPhase(_spi, LL_SPI_PHASE_1EDGE);
        LL_SPI_SetClockPolarity(_spi, LL_SPI_POLARITY_LOW);
        break;
    case SPI_MODE1:
        LL_SPI_SetClockPhase(_spi, LL_SPI_PHASE_2EDGE);
        LL_SPI_SetClockPolarity(_spi, LL_SPI_POLARITY_LOW);
        break;
    case SPI_MODE2:
        LL_SPI_SetClockPhase(_spi, LL_SPI_PHASE_1EDGE);
        LL_SPI_SetClockPolarity(_spi, LL_SPI_POLARITY_HIGH);
        break;
    case SPI_MODE3:
        LL_SPI_SetClockPhase(_spi, LL_SPI_PHASE_2EDGE);
        LL_SPI_SetClockPolarity(_spi, LL_SPI_POLARITY_HIGH);
        break;
    default:
        break;
    }

    /* Configure SPI1 communication */
    LL_SPI_SetBaudRatePrescaler(_spi, spi_config->prescaler);
    LL_SPI_SetTransferBitOrder(_spi, spi_config->bit_order);

    LL_SPI_SetTransferDirection(_spi,LL_SPI_FULL_DUPLEX);
    LL_SPI_SetDataWidth(_spi, LL_SPI_DATAWIDTH_8BIT);
    LL_SPI_SetNSSMode(_spi, LL_SPI_NSS_SOFT);
    LL_SPI_SetRxFIFOThreshold(_spi, LL_SPI_RX_FIFO_TH_QUARTER);

    LL_SPI_SetMode(_spi, LL_SPI_MODE_MASTER);
    LL_SPI_Enable(_spi);
	
		LL_SPI_Enable(SPI1);
}

uint8_t E_SPI::writeChar(uint8_t data)
{
	while ((_spi->SR & LL_SPI_SR_TXE) == RESET)
		;
	*((__IO uint8_t *)&_spi->DR) = data;
////	LL_SPI_TransmitData8(SPI1,data);
//	 _spi->DR = data;
		while ((_spi->SR & LL_SPI_SR_RXNE) == RESET)
		;
	return LL_SPI_ReceiveData8(SPI1);
	//return 0;
}

uint8_t E_SPI::writeBuf(uint8_t *data, uint16_t data_length)
{
	__IO uint8_t dummyByte;
	if (data_length == 0)
		return -1;
	while (data_length--)
	{
		while ((_spi->SR & LL_SPI_SR_TXE) == RESET)
			;
		*((__IO uint8_t *)&_spi->DR) = *data;
	}
	return 0;
}

uint8_t E_SPI::read()
{
	while ((_spi->SR & LL_SPI_SR_TXE) == RESET)
		;
	*((__IO uint8_t *)&_spi->DR) = 0xff;
	while ((_spi->SR & LL_SPI_SR_RXNE) == RESET)
		;
	 return (LL_SPI_ReceiveData8(SPI1));
	//return(_spi->DR);

}

uint8_t E_SPI::read(uint8_t *recv_data)
{
	while ((_spi->SR & LL_SPI_SR_TXE) == RESET)
		;
	*((__IO uint8_t *)&_spi->DR) = 0xff;
	while ((_spi->SR & LL_SPI_SR_RXNE) == RESET)
		;
	*recv_data = _spi->DR;

	return 0;
}

uint8_t E_SPI::read(uint8_t *recv_data, uint16_t data_length)
{
	if (data_length == 0)
		return -1;
	while (data_length--)
	{
		while ((_spi->SR & LL_SPI_SR_TXE) == RESET)
			;
		*((__IO uint8_t *)&_spi->DR) = 0xff;
		while ((_spi->SR & LL_SPI_SR_RXNE) == RESET)
			;
		*recv_data = _spi->DR;
	}
	return 0;
}
