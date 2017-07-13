/**
  ******************************************************************************
  * @file    ebox_exti.h
  * @author  cat_li
  * @version V2.0
  * @date    2016/10/26
  * @brief
		2017/7/13	cat_li	
		1	�޸��ļ���Ϊebox_exti	
		2 	�����½��أ������أ��ֱ𴥷������󶨲�ͬ�Ļص�����
			������������½��ػص�����
		
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

// ��������
enum TrigType {
	FALLING = 0,	// �½��ش���
	RISING,			// �����ش���
	FALL_RISING		// �������½���
};
	
enum ExtiType{
	IT = 0,			// �ж�
	EVENT,			// �¼�
	IT_EVENT		// �ж�&�¼�
};

class E_EXTI{
public:

	E_EXTI(E_PinID id,ExtiType type = IT);
	E_EXTI(E_GPIO *pin,ExtiType type = IT);
	E_EXTI(uint16_t i){};

	void ENABLE(ExtiType type);
	void ENABLE(TrigType type);

	// �󶨾�̬�ص�����
	void attach(void (*fptr)(void),TrigType type){
		if (type == FALL_RISING){
			_pirq[FALLING].attach(fptr);
			_pirq[RISING].attach(fptr);
		}else{
			_pirq[type].attach(fptr);
		}
	}
	// �󶨳�Ա�ص�����
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
	// ��̬��Ա��������Ĵ�������ʹ�������࣬��Ҳ���ڣ��ҿ������ⲿͨ����������: IRQ::irq_handler(0)
	// ����ʵ������̬��Ա����̬��Ա���ܷ�����ͨ��Ա����Ҫͨ����������ӷ���
	static void _irq_handler(uint32_t id);
	// �ص�����ָ�����飬�ֱ���½��ػص��������ػص�����
	FunctionPointer _pirq[2];
	E_GPIO	*_pin;
};

#endif
