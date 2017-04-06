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
 *@brief    ebox通用gpio接口，继承自E_PinBase类。适合对单个pin进行操作
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
		
			//操作符重载
	operator int();
	uint8_t operator =(E_GPIO&);
	E_GPIO operator = (uint8_t value);
};

/**
 *@brief    ebox通用gpio接口，适合对同一Port的多个Pin同时操作
*/
class E_PORT{
public:
    /**
       *@brief    操作同一个Port的多个pin，可以连续，可以不连续
       *@param    port 端口； mask 要操作的pin，
       *    例1： 要控制 PA2,PA3,PA4; 则 port = E_PORTA, mask = 0x1c;即（0001 1100）
       *    例2： 要控制 PA0,PA4,PA5; 则 port = E_PORTA, mask = 0x31;即（0011 0001）       
       *@retval   NONE
       */
    E_PORT(uint8_t port, uint32_t mask = 0xffffffff);
    /**
       *@brief    操作同一个Port的多个pin，只能是连续Pin
       *@param    port 端口； pinnum 要操作的Pin数量，pinoffset 相对与pin0的偏移
       *    例： 要控制 PA2,PA3,PA4; 则 port = E_PORTA, pinnum = 3, pinoffset = 2
       *@retval   NONE
       */
    E_PORT(uint8_t port, uint8_t pinnum, uint8_t pinoffset);
    void mode(PIN_MODE mode);

    void 	write(uint16_t val);
    void 	setAll(void);
    void 	resetAll(void);

    uint16_t read(void);

    //操作符重载
    // 相当于E_PORT.read()
    operator uint16_t();
    // 相当于E_PORT.write()
    void operator = (uint16_t value);

private:
    uint32_t _mask;
    uint8_t _offset;
    GPIO_TypeDef *_port;
    
    void init_port(uint8_t port);
};

/**
 *@brief    ebox通用gpio接口，继承自E_PinBase类。将不同Port的多个Pin组合进行操作
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

    //操作符重载

     void operator = (uint16_t value);
//    E_BUS& operator= (E_BUS& rhs);

		/**
		 *@brief 重载下标运算符，返回类型E_GPIO, index为数组下标。
		 */
    E_GPIO& operator[] (uint16_t index);
		// 相当于write
    operator uint16_t();

private:
    E_GPIO *_pins[16];
//    uint32_t _mask;

};
#endif
