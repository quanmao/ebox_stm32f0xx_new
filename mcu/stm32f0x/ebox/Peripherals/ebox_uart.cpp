/**
  ******************************************************************************
  * @file    ebox_uart.cpp
  * @author  cat_li
  * @version V2.0
  * @date    2016/10/21
  * @brief   
	1  2017/5/25  移除E_PinBase的构造函数，使用PIN_ID
	2  2017/5/30	完善uart接口
	3  2017/7/12	添加printf输出
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

static uint32_t serial_irq_ids[UART_NUM];  // 保存对象地址，供静态成员识别对象，并访问对象的普通成员
static uart_irq_handler irq_handler;	   // 声明函数指针变量，指向类的静态成员

/**
 *@brief    uart中断初始化函数
 *@param    index 中断索引，handler 回调函数地址，id （类）对象地址
 *@retval   NONE
*/
void serial_irq_init(IrqIndex_t index, uart_irq_handler handler, uint32_t id)
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

/**
 *@brief    	uart 初始化
 *@param    	uint32_t speed 串口波特率;
 *@param		uint8_t data_bit 数据位 7,8,9，
 *@param		uint8_t parity 校验位 0（NONE）,1（EVEN）,2（ODD），
 *@param		float stop_bit 停止位 0.5, 1, 1.5, 2
 *@param		uint32_t use_dma 是否使用DMA 1 使用，0 不使用。F0中没有实现
 *@retval   	NONE
*/
void begin(uint32_t speed,uint8_t data_bit, uint8_t parity, float stop_bit,uint8_t _use_dma){
	uint32_t data,par,stop;
	switch (data_bit)
	{
#if !defined(STM32F030x6)
	case 7:
		data = LL_USART_DATAWIDTH_7B;
		break;
#endif
	case 8:
		data = LL_USART_DATAWIDTH_8B;
		break;
	case 9:
		data = LL_USART_DATAWIDTH_9B;
		break;
	default :
		data = LL_USART_DATAWIDTH_8B;
		break;
	}
	switch (parity)
	{
	case 0:
		par = LL_USART_PARITY_NONE;
		break;
	case 1:
		par = LL_USART_PARITY_EVEN;
		break;
	case 2:
		par = LL_USART_PARITY_ODD;
		break;
	default :
		par = LL_USART_PARITY_NONE;
		break;
	}
#if !defined(STM32F030x6)
	if (stop_bit == 0.5)
		stop = LL_USART_STOPBITS_0_5;
	else 
#endif
			 if (stop_bit == 1)
		stop = LL_USART_STOPBITS_1;
	else 
#if !defined(STM32F030x6)
		if (stop_bit == 1.5)
		stop = LL_USART_STOPBITS_1_5;
	else 
#endif		
		if (stop_bit == 2)
		stop = LL_USART_STOPBITS_2;
	else
		stop = LL_USART_STOPBITS_1;
	begin(speed,data,par,stop,1);
}

/**
 *@brief    uart 初始化
 *@param    uint32_t speed 串口波特率;
 *@param		uint32_t data_bit 数据位 LL_USART_DATAWIDTH_8B，
 *@param		uint32_t parity 校验位 LL_USART_PARITY_NONE，
 *@param		uint32_t stop_bit 停止位 LL_USART_STOPBITS_1
 *@param		uint32_t use_dma 是否使用DMA 1 使用，0 不使用。F0中没有实现
 *@retval   NONE
*/
void E_UART::begin(uint32_t speed,uint32_t data_bit, uint32_t parity, uint32_t stop_bit,uint8_t use_dma)
{
	uint8_t index = getPeriphIndex((uint32_t)UARTx,UART_INFO);
	// 使能时钟，初始化中断信息
	LL_APB1_GRP2_EnableClock(UART_INFO[index]._rcc);
	serial_irq_init(UART_INFO[index]._irqIndex , (&E_UART::_irq_handler), (uint32_t)this);
	NVIC_SetPriority(UART_INFO[index]._irq, 0);
	NVIC_EnableIRQ(UART_INFO[index]._irq);
	// 初始化相关pin
	index = getIndex(Tx->_id,UART_MAP);
	Tx->mode(UART_MAP[index]._pin_date,UART_MAP[index]._pin_af);
	index = getIndex(Rx->_id,UART_MAP);
	Rx->mode(UART_MAP[index]._pin_date,UART_MAP[index]._pin_af);
	// 设置串口参数
	LL_USART_SetTransferDirection(UARTx, LL_USART_DIRECTION_TX_RX);
	/* 8 data bit, 1 start bit, 1 stop bit, no parity */
	LL_USART_ConfigCharacter(UARTx, data_bit, parity, stop_bit);
	LL_USART_SetBaudRate(UARTx, SystemCoreClock, LL_USART_OVERSAMPLING_16, speed);
	/* (4) Enable USARTx **********************************************************/
	LL_USART_Enable(UARTx);	
	/* Polling USART initialisation */
	while ((!(LL_USART_IsActiveFlag_TEACK(UARTx))) || (!(LL_USART_IsActiveFlag_REACK(UARTx))))
	{
	}
}

/**
 *@name     size_t Uart::write(uint8_t c)
 *@brief    串口方式发送一个字节
 *@param    ch：要发送的字符
 *@retval   固定返回1
*/
uint8_t E_UART::write(uint8_t c)
{
	while (!LL_USART_IsActiveFlag_TXE(UARTx)){};//单字节等待，等待寄存器空
	LL_USART_TransmitData8(UARTx,c);
	return 1;
}

/**
 *@name     size_t Uart::write(const uint8_t *buffer, size_t size)
 *@brief    串口方式发送字符串
 *@param    buffer：要发送的字符串
 *@retval   已发送的数据长度
*/
uint8_t E_UART::write(const char *buffer, int size){
	while (size--)
	{
		while (!LL_USART_IsActiveFlag_TXE(UARTx)){};//单字节等待，等待寄存器空
		LL_USART_TransmitData8(UARTx,*buffer++);
	}
	return size;
}

/**
 *@name     E_UART::printf(const char *fmt, ...)
 *@brief    串口格式化输出
 *@param    fmt：请百度printf函数
 *@retval   none
*/
//void E_UART::printf(const char *fmt, ...)
//{
//	int  size1 = 0;
//	uint8_t  size2 = BUF_BLOCK;

////    wait_busy();
//	if (_buf != NULL)
//		free(_buf);
////       ebox_free(_buf);

////    set_busy();

//	va_list va_params;
//	va_start(va_params, fmt);

//	do{
//		//分配内存
//		_buf = (char *)malloc(size2);
//		if (_buf == NULL)
//			return ;
//		//格式化到缓冲区
//		size1 = vsnprintf(_buf,size2, fmt, va_params);
//		//如果内存不够，重新申请更大的内存空间
//		if (size1 == -1  || size1 > size2)
//		{
//			size2+=BUF_BLOCK;
//			size1 = -1;
//			free(_buf);			
//		}
//	}while (size1 == -1);

//	va_end(va_params);
//	write(_buf, size1);
//}

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

/**
 *@brief    uart 中断使能
 *@param    IrqType type 中断类型，RxIrq,TxIrq;
 *@retval   NONE
*/
void E_UART::enable_irq(IrqType type){
	/*ENABLE USART RXNE interrupts */
	type == RxIrq?LL_USART_EnableIT_RXNE(UARTx):LL_USART_EnableIT_TC(UARTx);
}

/**
 *@brief    uart 中断失能
 *@param    IrqType type 中断类型，RxIrq,TxIrq;
 *@retval   NONE
*/
void E_UART::disable_irq(IrqType type){
	/*DISABLE USART RXNE interrupts */
	type == RxIrq?LL_USART_DisableIT_RXNE(UARTx):LL_USART_DisableIT_TC(UARTx);
}

/**
 *@brief    uart 绑定中断
 *@param    IrqType type 中断类型，RxIrq,TxIrq; void (*fptr)(void) 回调函数指针
 *@retval   NONE
*/
void E_UART::attach(void (*fptr)(void), IrqType type) {
    if (fptr != NULL) {
        _irq[type].attach(fptr);
    }
}

/**
 *@brief    uart 静态成员函数，在中断中调用，解析执行相关回调函数
 *@param    uint32_t id 对象地址，用来识别对象；IrqType irq_type 中断类型 
 *@retval   NONE
*/
void E_UART::_irq_handler(uint32_t id, IrqType irq_type) {
    E_UART *handler = (E_UART*)id;
    handler->_irq[irq_type].call();
}


// 中断入口
extern "C" {
	// 串口1中断
	void USART1_IRQHandler(void)
	{
		if (LL_USART_IsActiveFlag_RXNE(USART1) == SET )
		{
			// 调用接收中断回调函数
			irq_handler(serial_irq_ids[Irq1],RxIrq);
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
			irq_handler(serial_irq_ids[Irq1],TxIrq);
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

