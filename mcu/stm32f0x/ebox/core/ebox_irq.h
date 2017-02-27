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

    // �󶨾�̬�ص�����
    void attach(void (*fptr)(void)){
        pirq.attach(fptr);
    }
    // �󶨳�Ա�ص�����
    template<typename T>
    void attach(T* tptr, void (T::*mptr)(void)) {
        pirq.attach(tptr, mptr);
    }

    FunctionPointer pirq;
		
private:
	// ��̬��Ա��������Ĵ�������ʹ�������࣬��Ҳ���ڣ��ҿ������ⲿͨ����������: IRQ::irq_handler(0)
	// ����ʵ������̬��Ա����̬��Ա���ܷ�����ͨ��Ա����Ҫͨ����������ӷ���
	  static void irq_handler(uint32_t id);
};

#endif
