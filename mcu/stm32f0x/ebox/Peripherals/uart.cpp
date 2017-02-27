/**
  ******************************************************************************
  * @file    uart.cpp
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
 * -LQM (2016/8/30)
 *      *移植到STM32F0,基于HAL库LL层
 */

/* Includes ------------------------------------------------------------------*/
#include "uart.h"

#if (defined(STM32F030x6)||defined(STM32F031x6))
volatile uint8_t busy[1];
static uint32_t serial_irq_ids[1] = {};
#else
volatile uint8_t busy[UART_NUM];
static uint32_t serial_irq_ids[UART_NUM] = {};
#endif

static uart_irq_handler irq_handler;


/**
 *@name     Uart::Uart(USART_TypeDef *USARTx,Gpio *tx_pin,Gpio *rx_pin)
 *@brief    串口的构造函数
 *@param    USARTx:  USART1,2,3和UART4,5
 *          tx_pin:  外设所对应的tx引脚
 *          rx_pin:  外设所对应的rx引脚
 *@retval   None
*/
Uart::Uart(USART_TypeDef *USARTx, Gpio *tx_pin, Gpio *rx_pin)
{
    _USARTx = USARTx;
    tx_pin->mode(AF_PP,LL_GPIO_AF_1);
    rx_pin->mode(AF_PP,LL_GPIO_AF_1);
}

/**
 *@name     void Uart::begin(uint32_t baud_rate)
 *@brief    串口初始化函数，除了波特率可控外，其他参数默认：8bit数据，1个停止位，无校验，无硬件控制流
 *@param    baud_rate:  波特率，例如9600，115200，38400等等
 *@retval   None
*/
void Uart::begin(uint32_t baud_rate)
{
	begin(baud_rate,8,0,1);
}
/**
 *@name     void    begin(uint32_t baud_rate,uint8_t data_bit,uint8_t parity,float stop_bit);
 *@brief    串口初始化函数，并带有更多配置参数
 *@param    baud_rate:  波特率，例如9600，115200，38400等等
 *          data_bit:   数据位数，只能输入8或者9
 *          parity:     检验位；0：无校验位，1奇校验，2偶校验
 *          stop_bit:   停止位；0.5,1,1.5,2四个可选参数
 *@retval   None
*/
void Uart::begin(uint32_t baud_rate, uint8_t data_bit, uint8_t parity, float stop_bit)
{
	uint32_t _DataWidth;
	uint32_t _Parity;
	uint32_t _StopBits = 0;

	uint8_t index;

// 配置各串口参数
	switch ((uint32_t)_USARTx)
	{
	case (uint32_t)USART1_BASE:
		// GPIO Parameter
		LL_APB1_GRP2_EnableClock(LL_APB1_GRP2_PERIPH_USART1);
		index = NUM_UART1;
		usart_irq  = USART1_IRQn;
		// DMA Parameter
		_DMA1_Channelx  = LL_DMA_CHANNEL_2;
		break;
#if !(defined(STM32F030x6)||defined(STM32F031x6))
	case (uint32_t)USART2_BASE:
		// GPIO Parameter
		LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_USART2);
		index = NUM_UART2;
		usart_irq  = USART2_IRQn;
		// DMA Parameter
		_DMA1_Channelx  = LL_DMA_CHANNEL_4;
		break;
#endif
	}
//
	switch (data_bit)
	{
	case 8:
		_DataWidth = LL_USART_DATAWIDTH_8B;
		break;
	case 9:
		_DataWidth = LL_USART_DATAWIDTH_9B;
		break;
	default :
		_DataWidth = LL_USART_DATAWIDTH_8B;
		break;
	}
	switch (parity)
	{
	case 0:
		_Parity = LL_USART_PARITY_NONE;
		break;
	case 1:
		_Parity = LL_USART_PARITY_EVEN;
		break;
	case 2:
		_Parity = LL_USART_PARITY_ODD;
		break;
	default :
		_Parity = LL_USART_PARITY_NONE;
		break;
	}

#if defined(USART_SMARTCARD_SUPPORT)
	if (stop_bit == 0.5)
		_StopBits = LL_USART_STOPBITS_0_5;
	else 	if (stop_bit == 1)
		_StopBits = LL_USART_STOPBITS_1;
	else if (stop_bit == 2)
		_StopBits = LL_USART_STOPBITS_2;
	else if (stop_bit == 1.5)
		_StopBits = LL_USART_STOPBITS_1_5;
#else
	if (stop_bit == 1)
		_StopBits = LL_USART_STOPBITS_1;
	else if (stop_bit == 2)
		_StopBits = LL_USART_STOPBITS_2;
#endif

	if (USE_DMA == 1)
		LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_DMA1);	//使能DMA时钟

	serial_irq_handler(index, Uart::_irq_handler, (uint32_t)this);

	LL_USART_SetTransferDirection(_USARTx, LL_USART_DIRECTION_TX_RX);
	/* 8 data bit, 1 start bit, 1 stop bit, no parity */
	LL_USART_ConfigCharacter(_USARTx, _DataWidth, _Parity, _StopBits);
	LL_USART_SetBaudRate(_USARTx, SystemCoreClock, LL_USART_OVERSAMPLING_16, baud_rate);
	/* (4) Enable USARTx **********************************************************/
	LL_USART_Enable(_USARTx);
	/* Polling USART initialisation */
	while ((!(LL_USART_IsActiveFlag_TEACK(_USARTx))) || (!(LL_USART_IsActiveFlag_REACK(_USARTx))))
	{
	}
	interrupt(ENABLE);
}

/**
 *@name     void Uart::interrupt(FunctionalState enable)
 *@brief    串口中断控制函数
 *@param    enable:  ENABLE使能串口的发送完成和接收中断；DISABLE：关闭这两个中断
 *@retval   None
*/
void Uart::interrupt(FunctionalState enable)
{
	// 附加中断回掉函数时，打开中断
	NVIC_SetPriority(usart_irq, 0);
	if (enable == ENABLE)
	{
		NVIC_EnableIRQ(usart_irq);
	}else{
		NVIC_DisableIRQ(usart_irq);
	}
	// DMA发送使用了TC中断，所以在这里启用
	LL_USART_EnableIT_TC(_USARTx);
}

/**
 *@name     uint16_t Uart::receive()
 *@brief    串口接收数据，此函数只能在用户接收中断中调用
 *@param    NONE
 *@retval   串口所接收到的数据
*/
uint16_t Uart::receive()
{
    return (uint16_t)(_USARTx->RDR & (uint16_t)0x01FF);
}

/**
 *@name     uint16_t Uart::dma_send_string(const char *str,uint16_t length)
 *@brief    串口DMA方式发送字符串，缓冲区数据
 *@param    str：       要发送的字符串，数据缓冲区
            length：    缓冲区的长度
 *@retval   发送数据的长度
*/
uint16_t Uart::dma_send_string(const char *str, uint16_t length)
{	
  /* (1) Enable the clock of DMA1 */
  //LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_DMA1);
  /* (2) Configure the DMA functional parameters for transmission */
  LL_DMA_ConfigTransfer(DMA1, _DMA1_Channelx, 
                        LL_DMA_DIRECTION_MEMORY_TO_PERIPH | 
                        LL_DMA_PRIORITY_HIGH              | 
                        LL_DMA_MODE_NORMAL                | 
                        LL_DMA_PERIPH_NOINCREMENT         | 
                        LL_DMA_MEMORY_INCREMENT           | 
                        LL_DMA_PDATAALIGN_BYTE            | 
                        LL_DMA_MDATAALIGN_BYTE);
  LL_DMA_ConfigAddresses(DMA1, _DMA1_Channelx,
                         (uint32_t)str,/*(uint32_t)aTxBuffer,*/
                         LL_USART_DMA_GetRegAddr(_USARTx, LL_USART_DMA_REG_DATA_TRANSMIT),
                         LL_DMA_GetDataTransferDirection(DMA1, _DMA1_Channelx));
  LL_DMA_SetDataLength(DMA1, _DMA1_Channelx,length/*ubNbDataToTransmit*/);
  
  /* Enable DMA TX Request */
  LL_USART_EnableDMAReq_TX(_USARTx);
  LL_DMA_EnableChannel(DMA1,_DMA1_Channelx);	
  return length;
}

/**
 *@name     int Uart::put_char(uint16_t ch)
 *@brief    串口方式发送一个字节
 *@param    ch：    要发送的字符
 *@retval   已发送的数据
*/
int Uart::put_char(uint16_t ch)
{
	while (LL_USART_IsActiveFlag_TXE(_USARTx)== RESET);
	LL_USART_TransmitData8(_USARTx,ch);
    return ch;
}

/**
 *@name     void Uart::put_string(const char *str,uint16_t length)
 *@brief    串口方式发送字符串，缓冲区数据
 *@param    str：       要发送的字符串，数据缓冲区
            length：    缓冲区的长度
 *@retval   NONE
*/
void Uart::put_string(const char *str, uint16_t length)
{
    if(USE_DMA == 1)
    {
        dma_send_string(str, length);
    }
    else
    {
        while(length--)
        {
			while (LL_USART_IsActiveFlag_TXE(_USARTx)== RESET);
			LL_USART_TransmitData8(_USARTx,*str++);
        }
    }
}

/**
 *@name     void Uart::put_string(const char *str)
 *@brief    串口方式发送一个字节
 *@param    str：   要发送的字符串,直到遇到'\0'，如果没有'\0'，则按最大缓冲区计算
 *@retval   已发送的数据
*/
void Uart::put_string(const char *str)
{
    uint16_t i = 0;
    uint16_t length = 0;

    wait_busy();
    set_busy();
    while(str[i++] != '\0')
    {
        length++;
        if(length >= UART_MAX_SEND_BUF)
        {
            length = UART_MAX_SEND_BUF;
            break;
        }
    };
    put_string(str, length);
}

/////////////////////////////////////////////////
/**
 *@name     int Uart::put_char(uint16_t ch)
 *@brief    串口方式发送一个字节
 *@param    str：       要发送的字符串，数据缓冲区
            length：    缓冲区的长度
 *@retval   已发送的数据
*/
void Uart::printf_length(const char *str, uint16_t length)
{
    wait_busy();
    set_busy();
    if(length >= UART_MAX_SEND_BUF)
        length = UART_MAX_SEND_BUF;
    for(int i = 0; i < length; i++)
        send_buf[i] = *str++;
    put_string(send_buf, length);
}

/**
 *@name     int Uart::put_char(uint16_t ch)
 *@brief    串口方式发送一个字节
 *@param    str：       要发送的字符串，数据缓冲区
            length：    缓冲区的长度
 *@retval   已发送的数据
*/
#include "printf.h"


void Uart::printf(const char *fmt, ...)
{
    uint16_t i = 0;
    uint16_t length = 0;

    wait_busy();
    set_busy();
    va_list va_params;
    va_start(va_params, fmt);
    //vsprintf(send_buf, fmt, va_params); //存在内存溢出的风险
    MyVsprintf((char *)send_buf, fmt, va_params);
    va_end(va_params);

    while(send_buf[i++] != '\0')
    {
        length++;
        if(length >= UART_MAX_SEND_BUF)
        {
            length = UART_MAX_SEND_BUF;
            break;
        }
    };
    put_string(send_buf, length);
}


/**
 *@name     int Uart::put_char(uint16_t ch)
 *@brief    串口方式发送一个字节
 *@param    str：       要发送的字符串，数据缓冲区
            length：    缓冲区的长度
 *@retval   已发送的数据
*/
void Uart::wait_busy()
{
	switch ((uint32_t)_USARTx)
	{
	case (uint32_t)USART1_BASE:
		while (busy[0] == 1);
		break;
#ifdef USART2
	case (uint32_t)USART2_BASE:
		while (busy[1] == 1);
		break;
#endif
	}
}

/**
 *@name     int Uart::put_char(uint16_t ch)
 *@brief    串口方式发送一个字节
 *@param    str：       要发送的字符串，数据缓冲区
            length：    缓冲区的长度
 *@retval   已发送的数据
*/
void Uart::set_busy()
{
	switch ((uint32_t)_USARTx)
	{
	case (uint32_t)USART1_BASE:
		busy[0] = 1;
		break;
#ifdef USART2
	case (uint32_t)USART2_BASE:
		busy[1] = 1;
		break;
#endif
	}
}

/**
 *@name     void Uart::attach(void (*fptr)(void), IrqType type)
 *@brief    附加中断
 *@param    void (*fptr)(void)：中断回调函数
            IrqType type：   中断类型
 *@retval   none
*/
void Uart::attach(void (*fptr)(void), IrqType type) {
    if (fptr) {
		// 打开相应中断
		switch (type)
		{
			case RxIrq:
				LL_USART_EnableIT_RXNE(_USARTx);
				break;
			case TcIrq:
				LL_USART_EnableIT_TC(_USARTx);
				break;
		default:
			break;
		}
        _irq[type].attach(fptr);
    }
}

void Uart::_irq_handler(uint32_t id, IrqType irq_type) {
    Uart *handler = (Uart*)id;
    handler->_irq[irq_type].call();
}


extern "C" {

	void USART1_IRQHandler(void)
	{
		if (LL_USART_IsActiveFlag_RXNE(USART1) == SET )
		{
			irq_handler(serial_irq_ids[NUM_UART1],RxIrq);
			// 如果回调函数中没有读取数据，则将当前数据抛弃，准备下一次接收
			if (LL_USART_IsActiveFlag_RXNE(USART1) == SET )
			{
				LL_USART_RequestRxDataFlush(USART1);
			}
		}
		if (LL_USART_IsActiveFlag_TC(USART1) == SET)
		{
			// 清除忙标志，调用发送结束回调函数
			busy[0] = 0;
			irq_handler(serial_irq_ids[NUM_UART1],TcIrq);
			// 清除发送结束中断标志
			LL_USART_ClearFlag_TC(USART1);
			LL_DMA_DisableChannel(DMA1, LL_DMA_CHANNEL_2);
		}
	}
#ifdef USART2
	void USART2_IRQHandler(void)
	{
		if (LL_USART_IsActiveFlag_RXNE(USART2) == SET )
		{
			irq_handler(serial_irq_ids[NUM_UART2],RxIrq);
			// 如果回调函数中没有读取数据，则将当前数据抛弃，准备下一次接收
			if (LL_USART_IsActiveFlag_RXNE(USART2) == SET )
			{
				LL_USART_RequestRxDataFlush(USART2);
			}
		}
		if (LL_USART_IsActiveFlag_TC(USART2) == SET)
		{
			// 清除忙标志，调用发送结束回调函数
			busy[1] = 0;
			irq_handler(serial_irq_ids[NUM_UART2],TcIrq);
			// 清除发送结束中断标志
			LL_USART_ClearFlag_TC(USART2);
			LL_DMA_DisableChannel(DMA1, LL_DMA_CHANNEL_4);
		}
	}
#endif

	void serial_irq_handler(uint8_t index, uart_irq_handler handler, uint32_t id)
	{
		irq_handler = handler;
		serial_irq_ids[index] = id;
	}
}




