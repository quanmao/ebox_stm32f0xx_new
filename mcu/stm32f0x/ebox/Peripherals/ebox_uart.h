/**
  ******************************************************************************
  * @file    ebox_uart.h
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

#ifndef __EBOX_UART_H_
#define __EBOX_UART_H_

#include "stm32_define.h"
#include "ebox_define.h"
#include "ebox_gpio.h"



class E_UART{
	public:
		E_UART(USART_TypeDef *UARTx,E_PinBase *PinTx,E_PinBase *PinRx);
		void begin(uint32_t speed);
		
	private:
	  USART_TypeDef *UARTx;
	  E_PinBase *Rx;
		E_PinBase *Tx;
		
		uint8_t index;
};

#endif
