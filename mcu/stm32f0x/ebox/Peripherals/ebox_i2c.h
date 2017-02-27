/**
  ******************************************************************************
  * @file    ebox_i2c.h
  * @author  cat_li
  * @version V2.0
  * @date    2016/11/15
  * @brief   stm32f0
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
/* Define to prevent recursive inclusion -------------------------------------*/

#ifndef __EBOX_I2C_H_
#define __EBOX_I2C_H_

#include "stm32_define.h"
#include "ebox_gpio.h"
#include "stm32f0xx_ll_i2c.h"

class E_I2c
{

public:
    E_I2c(I2C_TypeDef *I2Cx, E_PinBase *scl, E_PinBase *sda);
    void		begin(uint16_t speed);
    void    config();
    uint32_t    read_config();

    int8_t	writeChar(uint8_t slave_address, uint8_t data);
    int8_t  writeBuf(uint8_t slave_address,uint8_t *data,uint16_t num_to_write);
    int8_t	writeBuf(uint8_t slave_address,uint8_t reg_address,uint8_t *data, uint16_t num_to_write);

    int8_t	readBuf(uint8_t slave_address,uint8_t *data, uint16_t num_to_read);
    int8_t	readBuf(uint8_t slave_address, uint8_t reg_address, uint8_t *data,uint16_t num_to_read);
    int8_t	wait_dev_busy(uint8_t slave_address);
public:
    int8_t take_i2c_right(uint32_t timing);
    int8_t release_i2c_right(void);

private:
    I2C_TypeDef *_i2cx;
		uint8_t 		_index;
    uint32_t    _timing;
    uint8_t     busy;
};


/*
	1.֧���κ�IO���ţ�
	2.�����ӿں�Ӳ��I2C��ȫһ�����Ի������
	ע�⣺
				1.�����speed����delay_us��ʱ�������ơ����в�׼
				2.speed����ֻ��Ϊ100000��200000,300k,400k��������Ǵ�ֵ����Ὣspeed��ֱֵ�Ӵ��ݸ�delay_us.��delay_us(speed);
				3.���ڵ���I2C�豸����ʹ��100k�����ߴ���10��ֵ
*/
//class SoftI2c
//{
//public:
//    SoftI2c(Gpio *scl, Gpio *sda);
//    void 		begin(uint32_t speed);
//    int8_t 		config(uint32_t speed);
//    uint32_t	read_config();
//    int8_t		write_byte(uint8_t slave_address, uint8_t reg_address, uint8_t data);
//    int8_t 		write_byte(uint8_t slave_address, uint8_t reg_address, uint8_t *data, uint16_t num_to_write);
//    int8_t 		read_byte (uint8_t slave_address, uint8_t reg_address, uint8_t *data);
//    int8_t 		read_byte (uint8_t slave_address, uint8_t reg_address, uint8_t *data, uint16_t num_to_read);
//    int8_t		wait_dev_busy (uint8_t slave_address);
//public:
//    int8_t      take_i2c_right(uint32_t speed);
//    int8_t      release_i2c_right(void);

//private:
//    void 	start();
//    void 	stop();
//    int8_t 	send_ack();
//    int8_t 	send_no_ack();

//    int8_t 	send_byte(uint8_t Byte);
//    int8_t	send_7bits_address(uint8_t slave_address);
//    uint8_t receive_byte();

//    int8_t	wait_ack();

//private:
//    Gpio        *sda_pin;
//    Gpio        *scl_pin;
//    uint32_t	speed;
//    uint16_t	delay_times;
//    uint8_t 	busy;
//};

#endif
