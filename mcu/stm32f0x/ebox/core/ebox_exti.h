/**
  ******************************************************************************
  * @file    ebox_exti.h
  * @author  cat_li
  * @version V2.0
  * @date    2016/10/26
  * @brief
		2017/7/13	cat_li	
		1	修改文件名为ebox_exti	
		2 	允许下降沿，上升沿，分别触发，并绑定不同的回调函数
			允许绑定上升沿下降沿回调函数
		
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
#ifndef __EBOX_EXTI_H_
#define __EBOX_EXTI_H_

#include "ebox_core.h"
#include "ebox_gpio.h"
#include "ebox_Template.h"

// 触发类型
enum TrigType {
	FALLING = 0,	// 下降沿触发
	RISING,			// 上升沿触发
	FALL_RISING		// 上升沿下降沿
};
	
enum ExtiType{
	IT = 0,			// 中断
	EVENT,			// 事件
	IT_EVENT		// 中断&事件
};

class E_EXTI{
public:

	E_EXTI(E_PinID id,ExtiType type = IT);
	E_EXTI(E_GPIO *pin,ExtiType type = IT);
	E_EXTI(uint16_t i){};

	void ENABLE(ExtiType type);
	void ENABLE(TrigType type);

	// 绑定静态回调函数
	void attach(void (*fptr)(void),TrigType type){
		if (type == FALL_RISING){
			_pirq[FALLING].attach(fptr);
			_pirq[RISING].attach(fptr);
		}else{
			_pirq[type].attach(fptr);
		}
	}
	// 绑定成员回调函数
	template<typename T>
	void attach(T* tptr, void (T::*mptr)(void),TrigType type) {
		if (type == FALL_RISING){
			_pirq[FALLING].attach(tptr, mptr);
			_pirq[RISING].attach(tptr, mptr);
		}else{
			_pirq[type].attach(tptr, mptr);
		}
	}

private:
	// 静态成员不依赖类的创建，即使不创建类，它也存在，且可以在外部通过类名访问: IRQ::irq_handler(0)
	// 所有实例共享静态成员，静态成员不能访问普通成员，需要通过对象名间接访问
	static void _irq_handler(uint32_t id);
	// 回调函数指针数组，分别绑定下降沿回调和上升沿回调函数
	FunctionPointer _pirq[2];
	E_GPIO	*_pin;
};

#endif
