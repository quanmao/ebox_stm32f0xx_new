/**
  ******************************************************************************
  * @file    ebox_gpio.h
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

#ifndef __EBOX_GPIO_H_
#define __EBOX_GPIO_H_

#include "stm32_define.h"

#define GETPIN(A) 	 			(uint16_t)(1<<(A&0x0f))
#define GETPORTINDEX(A)   (A)&0xf0

class E_PinBase{
protected:
    GPIO_TypeDef *_port;
    uint16_t _pin;
public:
    PIN_ID _id;

    E_PinBase(PIN_ID pin);
    void mode(PIN_MODE mode,uint16_t af);
};

/**
 *@brief    eboxͨ��gpio�ӿڣ��̳���E_PinBase�ࡣ�ʺ϶Ե���pin���в���
*/
class E_GPIO: public E_PinBase{
public:
    E_GPIO(PIN_ID pin): E_PinBase(pin){}

    void mode(PIN_MODE mode){
        E_PinBase::mode(mode,0);
    }

    void 		write(uint8_t val);
    void 		toggle(void);
    void 		set(void);
    void 		reset(void);

    uint8_t read(void);
		
			//����������
	operator int();
	uint8_t operator =(E_GPIO&);
	E_GPIO operator = (uint8_t value);
};

/**
 *@brief    eboxͨ��gpio�ӿڣ��ʺ϶�ͬһPort�Ķ��Pinͬʱ����
*/
class E_PORT{
public:
    /**
       *@brief    ����ͬһ��Port�Ķ��pin���������������Բ�����
       *@param    port �˿ڣ� mask Ҫ������pin��
       *    ��1�� Ҫ���� PA2,PA3,PA4; �� port = E_PORTA, mask = 0x1c;����0001 1100��
       *    ��2�� Ҫ���� PA0,PA4,PA5; �� port = E_PORTA, mask = 0x31;����0011 0001��       
       *@retval   NONE
       */
    E_PORT(uint8_t port, uint32_t mask = 0xffffffff);
    /**
       *@brief    ����ͬһ��Port�Ķ��pin��ֻ��������Pin
       *@param    port �˿ڣ� pinnum Ҫ������Pin������pinoffset �����pin0��ƫ��
       *    ���� Ҫ���� PA2,PA3,PA4; �� port = E_PORTA, pinnum = 3, pinoffset = 2
       *@retval   NONE
       */
    E_PORT(uint8_t port, uint8_t pinnum, uint8_t pinoffset);
    void mode(PIN_MODE mode);

    void 	write(uint16_t val);
    void 	setAll(void);
    void 	resetAll(void);

    uint16_t read(void);

    //����������
    // �൱��E_PORT.read()
    operator uint16_t();
    // �൱��E_PORT.write()
    void operator = (uint16_t value);

private:
    uint32_t _mask;
    uint8_t _offset;
    GPIO_TypeDef *_port;
    
    void init_port(uint8_t port);
};

/**
 *@brief    eboxͨ��gpio�ӿڣ��̳���E_PinBase�ࡣ����ͬPort�Ķ��Pin��Ͻ��в���
 */
class E_BUS{
public:
    E_BUS(PIN_ID p0,PIN_ID p1 = P_NC,PIN_ID p2 = P_NC,PIN_ID p3 = P_NC,PIN_ID p4 = P_NC,PIN_ID p5 = P_NC,
          PIN_ID p6 = P_NC,PIN_ID p7 = P_NC,PIN_ID p8 = P_NC,PIN_ID p9 = P_NC,PIN_ID p10 = P_NC,PIN_ID p11 = P_NC,
          PIN_ID p12 = P_NC,PIN_ID p13 = P_NC,PIN_ID p14 = P_NC,PIN_ID p15 = P_NC);

    E_BUS(PIN_ID Pins[16]);
    void mode(PIN_MODE mode);

    void 		write(uint16_t val);
    void 		setAll(void);
    void 		resetAll(void);

    uint16_t read(void);

    //����������

     void operator = (uint16_t value);
//    E_BUS& operator= (E_BUS& rhs);

		/**
		 *@brief �����±����������������E_GPIO, indexΪ�����±ꡣ
		 */
    E_GPIO& operator[] (uint16_t index);
		// �൱��write
    operator uint16_t();

private:
    E_GPIO *_pins[16];
//    uint32_t _mask;

};
#endif
