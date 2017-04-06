/**
  ******************************************************************************
  * @file    ebox_uart.cpp
  * @author  cat_li
  * @version V2.0
  * @date    2016/10/21
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
#include "ebox_uart.h"
#include "stm32f0xx_ll_usart.h"
#include <stdio.h>

#ifdef __cplusplus
extern "C" {
#endif
	
//uint8_t find(uint16_t pin, uint32_t UARTx,const E_PIN_FUN_T *emap)
//{
//		while(!((emap->pin_id == pin) && (emap->fun == UARTx)))
//		{
//				if(emap->pin_id == (uint16_t)NC){return (uint8_t)E_NC;}
//				emap++;
//		}
//		return emap->alternate;
//}

//uint8_t Find(uint32_t Periphx,const E_PIN_FUN_T *emap)
//{
//		uint8_t i = 0;
//		while(!((emap+i)->name == Periphx ))
//		{
//				if((emap+i)->name == (uint16_t)NC){return (uint8_t)E_NC;}
//				i++;
//		}
//		return i;
//}

/*******************************************************************************
	函数名：fputc
	输  入:
	输  出:
	功能说明：
	重定义putc函数，这样可以使用printf函数从串口1打印输出
*/
int fputc(int ch, FILE *f)
{
		while(!LL_USART_IsActiveFlag_TXE(USART1)){};
	  //LL_USART_ClearFlag_TC(UARTx); 
		LL_USART_TransmitData8(USART1,ch);

	return ch;
}
#ifdef __cplusplus
}
#endif

E_UART::E_UART(USART_TypeDef *UARTx,E_PinBase *PinTx,E_PinBase *PinRx)
{
		this->UARTx = UARTx;
		Rx = PinRx;
		Tx = PinTx;
}

void E_UART::begin(uint32_t speed)
{
//	uint8_t tx_af =	PinAlternate(Tx->id,E_UART_Tx);	
//	uint8_t rx_af = PinAlternate(Rx->id,E_UART_Rx);
	LL_APB1_GRP2_EnableClock(USART_MAP[getIndex(Tx->_id,USART_MAP)].Periph);
	
	Tx->mode(AF_PP_PU,USART_MAP[getIndex(Tx->_id,USART_MAP)].pin_af);
	Rx->mode(AF_PP_PU,USART_MAP[getIndex(Rx->_id,USART_MAP)].pin_af);
	
	LL_USART_SetTransferDirection(UARTx, LL_USART_DIRECTION_TX_RX);
	/* 8 data bit, 1 start bit, 1 stop bit, no parity */
	LL_USART_ConfigCharacter(UARTx, LL_USART_DATAWIDTH_8B, LL_USART_PARITY_NONE, LL_USART_STOPBITS_1);
	LL_USART_SetBaudRate(UARTx, SystemCoreClock, LL_USART_OVERSAMPLING_16, speed);
	/* (4) Enable USARTx **********************************************************/
	LL_USART_Enable(UARTx);
	/* Polling USART initialisation */
	while ((!(LL_USART_IsActiveFlag_TEACK(UARTx))) || (!(LL_USART_IsActiveFlag_REACK(UARTx))))
	{
	}
}

