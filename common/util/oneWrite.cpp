/**
  ******************************************************************************
  * @file    oneWrite.cpp
  * @author  cat_li
  * @version V1.0
  * @date    2016/11/21
  * @brief   �������������� 
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
#include "oneWrite.h"
#include "ebox_core.h"

/**
 *@brief    ����������ʼ������(��λ��Ӧ��)
 *@param    NONE
 *@retval   0 ��Ӧ��;  1 �豸ok
*/
uint8_t oneWrite::reset(void){
    uint8_t retry = 125,r;
    //_bus->mode(INPUT_PU);
    _bus->mode(OUTPUT_PP_PU);
    _bus->set();

    // �ȴ������ͷ�
    do{
        if (retry -- == 0) return 0;
        delay_us(2);
    }while (_bus->read() == 0);

//		_bus->mode(OUTPUT_PP_PU);
    // ��λ���壬��������420us
    _bus->reset();
    delay_us(550);
    _bus->set();
//		// ��������480us�����еȴ��ӻ�Ӧ��15-60us��Ӧ������60-240us
//		_bus->mode(INPUT_PU);
    delay_us(30);
    r = _bus->read();
    delay_us(400);
    _bus->set();
    return r;
}

/**
 *@brief    ��ӻ�д1���ֽ�
 *@param    uint8_t bit  0 = 0 
 *											!0 = 1;
 *@retval   none
*/
void oneWrite::writeBit(uint8_t bit){
    //_bus->mode(OUTPUT_PP_PU);

    if (bit & 1){
        no_interrupts();
        _bus->reset();
        //		delay_us(1);
        _bus->set();
        interrupts();
        delay_us(70);
        _bus->reset();
    }else{
        no_interrupts();
        _bus->reset();
        delay_us(70);
        _bus->set();
        interrupts();
//				delay_us(2);
    }
}

/**
 *@brief    ��ȡһ���ֽ�
 *@param    None
 *@retval   0 or 1
*/
uint8_t oneWrite::readBit(void){
    uint8_t r;
    _bus->mode(OUTPUT_PP_PU);
    no_interrupts();
    _bus->reset();
    //delay_us(1);
    _bus->set();
    delay_us(15);
    //_bus->mode(INPUT_PU);
    r = _bus->read();
    interrupts();
    delay_us(80);
    return r;
}

void oneWrite::writeChar(uint8_t ch){
    uint8_t bitMask;
    for (bitMask = 0x01; bitMask; bitMask <<=1){
        writeBit((bitMask & ch)?1:0);
    }
}

uint8_t oneWrite::readChar(void){
    uint8_t bitMask,r = 0;

    for (bitMask = 0x01; bitMask; bitMask <<=1){
        if (readBit()) r|=bitMask;
    }
    return r;
}


