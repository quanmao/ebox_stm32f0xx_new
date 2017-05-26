/**
  ******************************************************************************
  * @file    ebox_uart.cpp
  * @author  cat_li
  * @version V2.0
  * @date    2016/10/21
  * @brief   
	1  2017/5/25  移除E_PinBase的构造函数，使用PIN_ID
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

static uint32_t serial_irq_ids[UART_NUM];  // 保存对象地址，供静态成员识别对象，并访问对象的普通成员
static uart_irq_handler irq_handler;	   // 声明函数指针变量，指向类的静态成员

void serial_irq_init(uint8_t index, uart_irq_handler handler, uint32_t id)
{
	irq_handler = handler;
	serial_irq_ids[index] = id;
}

/**
 *@brief    uart构造函数
 *@param    USART_TypeDef *UARTx -- uart端口; E_PinID PinTx -- Tx pin id; IN_ID PinRx -- rx pin id;
 *@retval   NONE
*/
E_UART::E_UART(USART_TypeDef *UARTx,E_PinID PinTx,E_PinID PinRx)
{
		this->UARTx = UARTx;
		Rx = new E_PinBase(PinRx);
		Tx = new E_PinBase(PinTx);
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
	serial_irq_init(0, (&E_UART::_irq_handler), (uint32_t)this);
	/* Polling USART initialisation */
	while ((!(LL_USART_IsActiveFlag_TEACK(UARTx))) || (!(LL_USART_IsActiveFlag_REACK(UARTx))))
	{
	}
}

/**
 *@name     size_t Uart::write(uint8_t c)
 *@brief    串口方式发送一个字节
 *@param    ch：要发送的字符
 *@retval   已发送的数据
*/
size_t E_UART::write(uint8_t ch)
{
	while (!LL_USART_IsActiveFlag_TXE(UARTx)){};//单字节等待，等待寄存器空
	LL_USART_TransmitData8(UARTx,ch);
	return 1;
}

/**
 *@name     uint16_t USART::read()
 *@brief    串口接收数据，此函数只能在用户接收中断中调用
 *@param    NONE
 *@retval   串口所接收到的数据
*/
uint16_t E_UART::read()
{
    return (uint16_t)(UARTx->RDR & (uint16_t)0x01FF);
}

void E_UART::enable_irq(IrqType type){
	// 附加中断回掉函数时，打开中断
	NVIC_SetPriority(USART1_IRQn, 0);
	NVIC_EnableIRQ(USART1_IRQn);
	LL_USART_EnableIT_TC(UARTx);
	/*ENABLE USART RXNE interrupts */
	type == RxIrq?LL_USART_EnableIT_RXNE(UARTx):LL_USART_EnableIT_TC(UARTx);
}

void E_UART::disable_irq(IrqType type){
	/*DISABLE USART RXNE interrupts */
	type == RxIrq?LL_USART_DisableIT_RXNE(UARTx):LL_USART_DisableIT_TC(UARTx);
}

void E_UART::attach(void (*fptr)(void), IrqType type) {
    if (fptr) {
        _irq[type].attach(fptr);
    }
}

void E_UART::_irq_handler(uint32_t id, IrqType irq_type) {
    E_UART *handler = (E_UART*)id;
    handler->_irq[irq_type].call();
}

extern "C" {
	// 串口1中断
	void USART1_IRQHandler(void)
	{
		if (LL_USART_IsActiveFlag_RXNE(USART1) == SET )
		{
			// 调用接收中断回调函数
			irq_handler(serial_irq_ids[0],RxIrq);
			// if (usart_callback_table[0][0] != 0)
				// usart_callback_table[0][0]();
			// 如果回调函数中没有读取数据，则将当前数据抛弃，准备下一次接收
			if (LL_USART_IsActiveFlag_RXNE(USART1) == SET )
			{
				LL_USART_RequestRxDataFlush(USART1);
			}
		}
		if (LL_USART_IsActiveFlag_TC(USART1) == SET)
		{
			// 清除忙标志，调用发送结束回调函数
			//busy[0] = 0;
			// if (usart_callback_table[0][1] != 0)
				// usart_callback_table[0][1]();
			irq_handler(serial_irq_ids[0],TcIrq);
			// 清除发送结束中断标志
			LL_USART_ClearFlag_TC(USART1);
//			LL_DMA_DisableChannel(DMA1, LL_DMA_CHANNEL_2);
		}
	}

#if !(defined(STM32F030x6)||defined(STM32F031x6))
//	void USART2_IRQHandler(void)
//	{
//		if (LL_USART_IsActiveFlag_RXNE(USART2) == SET )
//		{
//			// 调用接收中断回调函数
//			if (usart_callback_table[1][0] != 0)
//				usart_callback_table[1][0]();
//			// 如果回调函数中没有读取数据，则将当前数据抛弃，准备下一次接收
//			if (LL_USART_IsActiveFlag_RXNE(USART2) == SET )
//			{
//				LL_USART_RequestRxDataFlush(USART2);
//			}
//		}
//		if (LL_USART_IsActiveFlag_TC(USART2) == SET)
//		{
//			// 清除忙标志，调用发送结束回调函数
//			busy[1] = 0;
//			if (usart_callback_table[1][1] != 0)
//				usart_callback_table[1][1]();
//			// 清除发送结束中断标志
//			LL_USART_ClearFlag_TC(USART2);
//			LL_DMA_DisableChannel(DMA1, LL_DMA_CHANNEL_4);
//		}
//	}
#endif
}

