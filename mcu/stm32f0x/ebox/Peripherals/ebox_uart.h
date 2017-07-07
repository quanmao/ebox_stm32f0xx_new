/**
  ******************************************************************************
  * @file    ebox_uart.h
  * @author  cat_li
  * @version V2.0
  * @date    2016/10/21
  * @brief
	1  2017/5/25  �Ƴ�E_PinBase�Ĺ��캯����ʹ��PIN_ID
	2  2017/5/30	����uart�ӿ�
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
#include "stm32f0xx_ll_usart.h"
#include "ebox_define.h"
#include "ebox_gpio.h"
#include "print.h"
#include "ebox_Template.h"

#define UART_NUM (2)

// �ж�����
enum IrqType {
	RxIrq = 0,	// ������ɺ󴥷�
	TxIrq		// ������ɺ󴥷�
};

typedef void (*uart_irq_handler)(uint32_t id, IrqType type);


class E_UART:public Print{
public:

	E_UART(USART_TypeDef *UARTx,E_PinID PinTx,E_PinID PinRx);

	// initial uart
	void begin(uint32_t speed,uint8_t data_bit, uint8_t parity, float stop_bit,uint8_t _use_dma);
	void begin(uint32_t speed,uint32_t data_bit=LL_USART_DATAWIDTH_8B, uint32_t parity =LL_USART_PARITY_NONE, uint32_t stop_bit =LL_USART_STOPBITS_1 ,uint8_t use_dma =1);

	void enable_irq(IrqType type);
	void disable_irq(IrqType type);

	//write method
	virtual size_t  write(uint8_t c);
//	virtual size_t  write(const uint8_t *buffer, size_t size);
	using   Print::write;

	//read method
	uint16_t read();
	void   wait_busy();
	void   set_busy();
	/**
	 *@brief    �󶨾�̬�ص����� attach user event
	 *@param    IrqType type �ж����ͣ�RxIrq(���յ�����),TxIrq�����ݷ�����ɣ�;
	 *@param    void (*fptr)(void) �ص�����ָ��
	 *@retval   NONE
	*/
	void attach(void (*fptr)(void), IrqType type);

	/**
	 *@brief    �󶨳�Ա�ص�����
	 *@param    IrqType type �ж����ͣ�RxIrq(���յ�����),TxIrq�����ݷ�����ɣ�;
	 *@param    void (T::*mptr)(void) ���Ա����ָ��
	 *@param    T* tptr ���ʵ������
	 *@retval   NONE
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

	static void _irq_handler(uint32_t id, IrqType irq_type);
protected:
	FunctionPointer _irq[2];
};

#endif
