/**
  ******************************************************************************
  * @file    oneWrite.h
  * @author  cat_li
  * @version V1.0
  * @date    2016/11/21
  * @brief   单总线驱动程序 
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

#ifndef __ONEWRITE_H_
#define __ONEWRITE_H_

#include "stm32_define.h"
#include "ebox_define.h"
#include "ebox_gpio.h"

class oneWrite{
	public:
		oneWrite(E_GPIO *bus){
			_bus = bus;
		};
	
		uint8_t reset(void);
		void 		writeChar(uint8_t ch);
		uint8_t	readChar(void);

	  
	private:
		void 		writeBit(uint8_t bit);
		uint8_t     readBit(void);
	
		E_GPIO *_bus;
};


#endif
