/**
  ******************************************************************************
  * @file    ebox_uart.h
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

#ifndef __EBOX_UART_H_
#define __EBOX_UART_H_

#include "stm32_define.h"
#include "ebox_define.h"
#include "ebox_gpio.h"
#include "print.h"
#include "ebox_Template.h"

#define UART_NUM (2)

enum IrqType {
	RxIrq = 0,
	TcIrq
};
	
typedef void (*uart_irq_handler)(uint32_t id, IrqType type);

class E_UART: public Print{
public:
//	E_UART(USART_TypeDef *UARTx,E_PinBase *PinTx,E_PinBase *PinRx);

	E_UART(USART_TypeDef *UARTx,PIN_ID PinTx,PIN_ID PinRx);

	// initial uart
	void begin(uint32_t speed);
	void enable_irq(IrqType type);
	void disable_irq(IrqType type);

	//write method
	virtual size_t  write(uint8_t ch);
//	virtual size_t  write(const uint8_t *buffer, size_t size);
	using   Print::write;

	//read method
	uint16_t read();
	void    wait_busy();
	/** Attach a function to call whenever a serial interrupt is generated
	 *
	 *  @param fptr A pointer to a void function, or 0 to set as none
	 *  @param type Which serial interrupt to attach the member function to (Seriall::RxIrq for receive, TxIrq for transmit buffer empty)
	 */
	//attach user event
	// 绑定静态回调函数
	void attach(void (*fptr)(void), IrqType type);


	/** Attach a member function to call whenever a serial interrupt is generated
	 *
	 *  @param tptr pointer to the object to call the member function on
	 *  @param mptr pointer to the member function to be called
	 *  @param type Which serial interrupt to attach the member function to (Seriall::RxIrq for receive, TxIrq for transmit buffer empty)
	 */
	template<typename T>
	void attach(T* tptr, void (T::*mptr)(void), IrqType type) {
		if ((mptr != NULL) && (tptr != NULL)) {
			_irq[type].attach(tptr, mptr);
		}
	}



private:
	USART_TypeDef *UARTx;
	E_PinBase *Rx;
	E_PinBase *Tx;

	uint8_t index;

	static void _irq_handler(uint32_t id, IrqType irq_type);

protected:
	FunctionPointer _irq[2];
};

#endif
