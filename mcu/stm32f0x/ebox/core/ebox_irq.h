/**
  ******************************************************************************
  * @file    ebox_IRQ.h
  * @author  cat_li
  * @version V2.0
  * @date    2016/10/26
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
#ifndef __EBOX_IRQ_H_
#define __EBOX_IRQ_H_

#include "ebox_core.h"
#include "ebox_gpio.h"
#include "ebox_Template.h"

class IRQ{
public:
    IRQ(E_PinBase *irqpin){irqpin->mode(INPUT,0);};
		IRQ(uint16_t i){};

    void IRQ_ENABLE(void); 

    // 绑定静态回调函数
    void attach(void (*fptr)(void)){
        pirq.attach(fptr);
    }
    // 绑定成员回调函数
    template<typename T>
    void attach(T* tptr, void (T::*mptr)(void)) {
        pirq.attach(tptr, mptr);
    }

    FunctionPointer pirq;
		
private:
	// 静态成员不依赖类的创建，即使不创建类，它也存在，且可以在外部通过类名访问: IRQ::irq_handler(0)
	// 所有实例共享静态成员，静态成员不能访问普通成员，需要通过对象名间接访问
	  static void irq_handler(uint32_t id);
};

#endif
