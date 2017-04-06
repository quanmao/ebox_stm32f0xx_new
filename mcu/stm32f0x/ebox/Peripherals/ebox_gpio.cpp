/**
  ******************************************************************************
  * @file    ebox_gpio.cpp
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

#include "ebox_gpio.h"
#include "stm32f0xx_ll_bus.h"


/**
 *@brief    初始化端口信息,使能时钟
 *@param    NONE
 *@retval   NONE
*/
E_PinBase::E_PinBase(PIN_ID pin){
    _pin = GETPIN(pin);
	  _id = pin;
	
    //Port_Init((E_PORT)GETPORTINDEX(_id));
	  	switch (GETPORTINDEX(_id))
    {
    case E_PORTA:
        _port = GPIOA;
        LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOA);
        break;
    case E_PORTB:
        _port = GPIOB;
        LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOB);
        break;
#ifdef GPIOC
    case E_PORTC:
        _port = GPIOC;
        LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOC);
        break;
#endif
#ifdef GPIOD
    case E_PORTD:
        _port = GPIOD;
        LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOD);
        break;
#endif
#ifdef GPIOE
    case E_PORTE:
        _port = GPIOE;
        LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOE);
        break;
#endif
#ifdef GPIOF
    case E_PORTF:
        _port = GPIOF;
        LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOF);
        break;
#endif
    default:
        break;
    } 

}

/**
 *@brief    配置端口
 *@param    mode -- 输入输出模式; af -- 第二功能位
 *@retval   NONE
*/
void E_PinBase::mode(PIN_MODE mode,uint16_t af){


    //LL_AHB1_GRP1_EnableClock(E_PORT_MAP[GETPORT(id)].Periphs);

    LL_GPIO_SetPinMode(this->_port,this->_pin,E_PinMode_MAP[mode].mode);
    LL_GPIO_SetPinOutputType(this->_port,this->_pin,E_PinMode_MAP[mode].OutputType);
    LL_GPIO_SetPinPull(this->_port,this->_pin,E_PinMode_MAP[mode].Pull);
    LL_GPIO_SetPinSpeed(this->_port,this->_pin,LL_GPIO_SPEED_FREQ_HIGH);
    //af最大为7
    if (af <= 8){
        (_pin<LL_GPIO_PIN_8)?(LL_GPIO_SetAFPin_0_7(_port,_pin,af)):(LL_GPIO_SetAFPin_8_15(_port,_pin,af));
    }
}

/**
 *@brief    GPIO输出设置
 *@param    val：1：输出高电平；0：输出低电平
 *@retval   NONE
*/
void E_GPIO::write(uint8_t val){
		(val==0)?(_port->BRR = _pin):(_port->BSRR = _pin);
}

/**
 *@brief    返回指定Pin值
 *@param    NONE
 *@retval   0 or 1
*/
uint8_t E_GPIO::read(void)
{
	
	return ((_port->IDR & _pin) == _pin)?(1):(0);
}

/**
 *@brief    GPIO输出电平翻转
 *@param    NONE
 *@retval   NONE
*/
void E_GPIO::toggle(void)
{
	_port->ODR ^= _pin;
}

/**
 *@brief    GPIO输出低电平
 *@param    NONE
 *@retval   NONE
*/
void E_GPIO::reset(void)
{
    this->_port->BRR = this->_pin;
}

/**
 *@brief    GPIO输出高电平
 *@param    NONE
 *@retval   NONE
*/
void E_GPIO::set(void)
{
    this->_port->BSRR = this->_pin;
}

/**
 *@brief    操作符重载
 *@param    NONE
 *@retval   NONE
*/
uint8_t E_GPIO::operator =(E_GPIO&) 
{
    return read();
}

E_GPIO::operator int()
{
    return read();
}

E_GPIO E_GPIO::operator= (uint8_t value)
{
    write(value);
    return *this;
}

E_PORT::E_PORT(uint8_t port,uint32_t mask){
    _mask = mask;
    init_port(port);
}

E_PORT::E_PORT(uint8_t port, uint8_t pinnum, uint8_t pinoffset){
    _mask = (0xffffffff>>(32-pinnum))<<pinoffset;
    _offset = pinoffset;    
    init_port(port);
}

void E_PORT::init_port(uint8_t port){
        switch (port)
    {
    case E_PORTA:
        _port = GPIOA;
        LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOA);
        break;
    case E_PORTB:
        _port = GPIOB;
        LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOB);
        break;
#ifdef GPIOC
    case E_PORTC:
        _port = GPIOC;
        LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOC);
        break;
#endif
#ifdef GPIOD
    case E_PORTD:
        _port = GPIOD;
        LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOD);
        break;
#endif
#ifdef GPIOE
    case E_PORTE:
        _port = GPIOE;
        LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOE);
        break;
#endif
#ifdef GPIOF
    case E_PORTF:
        _port = GPIOF;
        LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOF);
        break;
#endif
    default:
        break;
    }
}

/**
 *@brief    设置端口指定PIN(mask)的模式
 *@param    mode 参考PIN_MODE
 *@retval   NONE
*/
void E_PORT::mode(PIN_MODE mode){  
		uint16_t i;		
		for(i=0;i<16;i++){
				if(_mask & (1<<i)){
					 LL_GPIO_SetPinMode(_port,(1<<i),E_PinMode_MAP[mode].mode);
					 LL_GPIO_SetPinOutputType(_port,i,E_PinMode_MAP[mode].OutputType);
					 LL_GPIO_SetPinPull(_port,i,E_PinMode_MAP[mode].Pull);
					 LL_GPIO_SetPinSpeed(_port,i,LL_GPIO_SPEED_FREQ_HIGH);
				}
		}    
}

/**
 *@brief    设置端口指定PIN(mask)为1
 *@param    NONE
 *@retval   NONE
*/
void E_PORT::setAll(void){
    //LL_GPIO_SetOutputPin(_port,_mask);
		_port->BSRR = _mask;
		//_port->ODR = (_port->ODR & ~_mask) | (0xffff & _mask);
}

/**
 *@brief    设置端口指定PIN(mask)为0
 *@param    NONE
 *@retval   NONE
*/
void E_PORT::resetAll(void){
    //LL_GPIO_ResetOutputPin(_port,_mask);
		//_port->ODR = (_port->ODR & ~_mask) | (0x00 & _mask);
		_port->BRR = _mask;
}

/**
 *@brief    向端口写数据
 *@param    uint16_t val
 *@retval   NONE
*/
void E_PORT::write(uint16_t val){
		//LL_GPIO_WriteOutputPort(_port,_mask&val);
	_offset == 0 ? _port->ODR = (_port->ODR & ~_mask) | (val & _mask):_port->ODR = (_port->ODR & ~_mask) | ((val<<_offset) & _mask);		
}

/**
 *@brief    从端口读取数据
 *@param    uint16_t val
 *@retval   NONE
*/
uint16_t E_PORT::read(void){
	return _offset== 0 ? _port->IDR & _mask:((_port->IDR & _mask) >> _offset);
}

/**
 *@brief    操作符重载
 *@param    NONE
 *@retval   NONE
*/
//uint16_t E_PORT::operator =(E_PORT&) 
//{
//    return read();
//}

E_PORT::operator uint16_t()
{
    return read();
}

void E_PORT::operator =(uint16_t value)
{
    write(value);
    //return *this;
}

// E_BUS 实现

E_BUS::E_BUS(PIN_ID p0,PIN_ID p1,PIN_ID p2,PIN_ID p3,PIN_ID p4,PIN_ID p5,
             PIN_ID p6,PIN_ID p7,PIN_ID p8,PIN_ID p9,PIN_ID p10,PIN_ID p11,
             PIN_ID p12,PIN_ID p13,PIN_ID p14,PIN_ID p15){

    PIN_ID pins[] = {p0,p1,p2,p3,p4,p5,p6,p7,p8,p9,p10,p11,p12,p13,p14,p15} ;
    uint8_t i;
    //_mask = 0;
    
    for (i=0;i<16;i++)
    {
        _pins[i] = (pins[i] !=P_NC)?(new E_GPIO(pins[i])):(0);
//        if(pins[i] !=P_NC){
//            _mask |= (1<<i);
//        }
    }
}

E_BUS::E_BUS(PIN_ID Pins[16]){
    uint8_t i;
//    _mask = 0;
    for (i=0;i<16;i++)
    {
        _pins[i] = (Pins[i] !=P_NC)?(new E_GPIO(Pins[i])):(0);
//        if (Pins[i] !=P_NC){
//            _mask |= (1<<i);
//        }
    }
};

void E_BUS::mode(PIN_MODE mode){
    uint16_t i;
    for (i=0;i<16;i++){
        if (_pins[i] != 0){
            _pins[i]->mode(mode);
        }
    }
}

void E_BUS::write(uint16_t val){
    uint16_t i;
    for (i=0;i<16;i++){
        if (_pins[i] !=0 ){
            _pins[i]->write((val>>i)&1);
        }
    }
}

uint16_t E_BUS::read(void){
    uint16_t r=0,i;
    for (i=0;i<16;i++){
        if (_pins[i] !=0 ){
            r |= _pins[i]->read()<<i;
        }
    }
    return r;
}

void E_BUS::setAll(void){
    write(0xffff);
}

void E_BUS::resetAll(void){
    write(0);
}

/**
 *@brief    操作符重载
 *@param    NONE
 *@retval   NONE
*/
#include "stm32_assert_template.h"
E_GPIO & E_BUS::operator[] (uint16_t index){
    assert_param(index >= 0 && index <= 16);
    assert_param(_pins[index]);  
    return *_pins[index];
}

void E_BUS::operator= (uint16_t value)
{
    write(value);
    //return *this;
}

//E_BUS& E_BUS::operator= (E_BUS& rhs){
//    write(rhs.read());
//    return *this;
//}

E_BUS::operator uint16_t(){
    return read();
}

