/**
  ******************************************************************************
  * @file    ebox_i2c.h
  * @author  cat_li
  * @version V2.0
  * @date    2016/11/15
  * @brief   仅工作在主模式
		1  2017/5/30  移除E_PinBase的构造函数，使用PIN_ID
					增加超时，防止程序死掉。
					读写函数增加返回状态
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

/* Includes ------------------------------------------------------------------*/
#include "ebox_i2c.h"
#include "ebox_core.h"
#include "stm32f0xx_ll_rcc.h"


// 分频，建立，保持，高电平，低电平
#define I2C_TIMING    __LL_I2C_CONVERT_TIMINGS(0xb, 0xc7, 0xc3, 0x02, 0x04)

/**
 *@name     I2c(I2C_TypeDef *I2Cx, Gpio *scl_pin, Gpio *sda_pin)
 *@brief      I2C构造函数
 *@param    I2Cx:  I2C1,I2C2
 *          scl_pin:  时钟Pin
 *          sda_pin:  数据Pin
 *@retval   None
*/
E_I2c::E_I2c(I2C_TypeDef *I2Cx,E_PinID scl,E_PinID sda,uint16_t timeout)
{
	E_PinBase _scl(scl),_sda(sda);

	_index = getIndex(scl,I2C_MAP);

	_scl.mode(I2C_MAP[_index]._pin_date,I2C_MAP[_index]._pin_af);
	_sda.mode(I2C_MAP[_index]._pin_date,I2C_MAP[_index]._pin_af);

	_index = getPeriphIndex((uint32_t)I2Cx,I2C_INFO);
	busy = 0;
	_i2cx = I2Cx;
	_timeout = timeout;
}

/**
 *@name     begin(uint16_t speed)
 *@brief    根据i2c时钟和设置速率speed计算timing。默认400k @8M
 *@param    speed:  速率 10,100,400 分别代表10k，100k，400k
 *@retval   None
*/
void  E_I2c::begin(uint16_t speed)
{
	switch (cpu.clock.pclk1/1000000){
	case 16:
		switch (speed){
		case 10:
			_timing = __LL_I2C_CONVERT_TIMINGS(0x3, 0xc7, 0xc3, 0x02, 0x04);	// 10k 	@16M
			break;
		case 100:
			_timing = __LL_I2C_CONVERT_TIMINGS(0x3, 0x13, 0xf, 0x02, 0x04);		// 100k @16M
			break;
		case 400:
		default:
			_timing = __LL_I2C_CONVERT_TIMINGS(0x1, 0x9, 0x3, 0x2, 0x3);			// 400k @16M
		}
		break;
	case 48:
		switch (speed){
		case 10:
			_timing = __LL_I2C_CONVERT_TIMINGS(0xb, 0xc7, 0xc3, 0x02, 0x04);	// 10k 	@48M
			break;
		case 100:
			_timing = __LL_I2C_CONVERT_TIMINGS(0xb, 0x13, 0xf, 0x02, 0x04);		// 100k @48M
			break;
		case 400:
		default:
			_timing = __LL_I2C_CONVERT_TIMINGS(0x5, 0x9, 0x3, 0x3, 0x3);			// 400k @48M
		}
		break;
	case 8:
	default:	// 默认系统时钟为8M
		switch (speed){
		case 10:
			_timing = __LL_I2C_CONVERT_TIMINGS(0x1, 0xc7, 0xc3, 0x02, 0x04);	// 10k 	@8M
			break;
		case 100:
			_timing = __LL_I2C_CONVERT_TIMINGS(0x1, 0x13, 0xf, 0x02, 0x04);		// 100k @8M
			break;
		case 400:
		default:
			_timing = __LL_I2C_CONVERT_TIMINGS(0x0, 0x9, 0x3, 0x1, 0x3);			// 400k @8M
		}
	}
	config();
}

void E_I2c::config()
{
	// 使能I2C时钟
	LL_APB1_GRP1_EnableClock(I2C_INFO[_index]._rcc);
	// I2C1 需要选择特定的时钟
	(_i2cx == I2C1)?(LL_RCC_SetI2CClockSource(LL_RCC_I2C1_CLKSOURCE_SYSCLK)):(void());

	LL_I2C_Disable(_i2cx);
	LL_I2C_SetTiming(_i2cx,_timing);
	LL_I2C_Enable(_i2cx);
}

uint32_t E_I2c::read_config()
{
	return _timing;
}

/**
 *@name     writeChar(uint8_t slave_address,uint8_t data)
 *@brief    I2C写入一个字节. start->data->stop
 *@param    I2Cx:  I2C1,I2C2
 *          uint8_t data:  要写入的数据
 *@retval   状态 E_OK 成功； E_TIMEOUT 超时
*/
int8_t E_I2c::writeChar(uint8_t slave_address,uint8_t data)
{
//	uint16_t time = _timeout;
	// 写入地址寄存器和数据
	LL_I2C_HandleTransfer(_i2cx,slave_address,LL_I2C_ADDRESSING_MODE_7BIT,1,LL_I2C_MODE_AUTOEND,LL_I2C_GENERATE_START_WRITE);
	
	while (!LL_I2C_IsActiveFlag_STOP(_i2cx))
	{
		if (LL_I2C_IsActiveFlag_TXIS(_i2cx))
		{
			LL_I2C_TransmitData8(_i2cx,data);
		}
//		if ((time--) == 0)
//		{
//			return E_TIMEOUT;
//		}
	}
	LL_I2C_ClearFlag_STOP(_i2cx);

	return E_OK;
}

/**
 *@name     writeBuf(uint8_t slave_address, uint8_t *data, uint16_t num_to_write)
 *@brief    I2C连续写 start->data....->stop
 *@param    I2Cx:  I2C1,I2C2
 *          uint8_t *data:  要写入的数据
 *          uint16_t num_to_write  要写入的数据长度
 *@retval   状态 E_OK 成功； E_TIMEOUT 超时
*/
int8_t E_I2c::writeBuf(uint8_t slave_address, uint8_t *data, uint16_t num_to_write)
{
//	uint16_t time = _timeout;
	if (num_to_write >255){
		// num_to_writ>255: RELOAD,NBYTE=0xFF,START
		LL_I2C_HandleTransfer(_i2cx,slave_address,LL_I2C_ADDRESSING_MODE_7BIT,0xFF,LL_I2C_MODE_RELOAD,LL_I2C_GENERATE_START_WRITE);

		while (!LL_I2C_IsActiveFlag_TCR(_i2cx))
		{
			if (LL_I2C_IsActiveFlag_TXIS(_i2cx))
			{
				LL_I2C_TransmitData8(_i2cx,*data++);
			}
//			if ((time--) == 0)
//			{
//				return E_TIMEOUT;
//			}
		}
		num_to_write = num_to_write - 255;
		// num_to_writ>255: RELOAD,NBYTE=0xFF,NOSTART

		while (num_to_write > 255){
			// 发送地址寄存器
			LL_I2C_HandleTransfer(_i2cx,slave_address,LL_I2C_ADDRESSING_MODE_7BIT,0xFF,LL_I2C_MODE_RELOAD,LL_I2C_GENERATE_NOSTARTSTOP);
//			time = _timeout;
			while (!LL_I2C_IsActiveFlag_TCR(_i2cx))
			{
				if (LL_I2C_IsActiveFlag_TXIS(_i2cx))
				{
					LL_I2C_TransmitData8(_i2cx,*data++);
				}
//				if ((time--) == 0)
//				{
//					return E_TIMEOUT;
//				}
			}
			num_to_write = num_to_write - 255;
		}
		// num_to_writ<=255: AUTOEND,NBYTE=num_to_writ,NOSTART
		LL_I2C_HandleTransfer(_i2cx,slave_address,LL_I2C_ADDRESSING_MODE_7BIT,num_to_write,LL_I2C_MODE_AUTOEND,LL_I2C_GENERATE_NOSTARTSTOP);
//		time = _timeout;
		while (!LL_I2C_IsActiveFlag_STOP(_i2cx))
		{
			if (LL_I2C_IsActiveFlag_TXIS(_i2cx))
			{
				LL_I2C_TransmitData8(_i2cx,*data++);
			}
//			if ((time--) == 0)
//			{
//				return E_TIMEOUT;
//			}
		}
		LL_I2C_ClearFlag_STOP(_i2cx);
	}else{
		// 写入地址寄存器和数据
		LL_I2C_HandleTransfer(_i2cx,slave_address,LL_I2C_ADDRESSING_MODE_7BIT,num_to_write,LL_I2C_MODE_AUTOEND,LL_I2C_GENERATE_START_WRITE);
//		time = _timeout;
		while (!LL_I2C_IsActiveFlag_STOP(_i2cx))
		{
			if (LL_I2C_IsActiveFlag_TXIS(_i2cx))
			{
				LL_I2C_TransmitData8(_i2cx,*data++);
			}
//			if ((time--) == 0)
//			{
//				return E_TIMEOUT;
//			}
		}
		LL_I2C_ClearFlag_STOP(_i2cx);
	}

	return E_OK;
}

/**
 *@name     writeBuf(uint8_t slave_address, uint8_t reg_address, uint8_t *data, uint16_t num_to_write)
 *@brief    在指定位置连续写入数据 start->reg_address->data....->stop
 *@param    I2Cx:  I2C1,I2C2
 *          uint8_t reg_address:寄存器地址
 *          uint8_t *data:  要写入的数据
 *          uint16_t num_to_write  要写入的数据长度
 *@retval   状态 E_OK 成功； E_TIMEOUT 超时
*/
int8_t E_I2c::writeBuf(uint8_t slave_address, uint8_t reg_address, uint8_t *data, uint16_t num_to_write)
{
//	uint16_t time = _timeout;
	// 发送地址寄存器
	LL_I2C_HandleTransfer(_i2cx,slave_address,LL_I2C_ADDRESSING_MODE_7BIT,1,LL_I2C_MODE_SOFTEND,LL_I2C_GENERATE_START_WRITE);

	while (!LL_I2C_IsActiveFlag_TC(_i2cx))
	{
		if (LL_I2C_IsActiveFlag_TXIS(_i2cx))
		{
			LL_I2C_TransmitData8(_i2cx,reg_address);
		}
//		if ((time--) == 0)
//		{
//			return E_TIMEOUT;
//		}
	}
	// 发送数据
	if (num_to_write >255){
		// num_to_writ>255: RELOAD,NBYTE=0xFF,NOSTART
		while (num_to_write > 255){
			// 发送地址寄存器
			LL_I2C_HandleTransfer(_i2cx,slave_address,LL_I2C_ADDRESSING_MODE_7BIT,0xFF,LL_I2C_MODE_RELOAD,LL_I2C_GENERATE_NOSTARTSTOP);
//			time = _timeout;
			while (!LL_I2C_IsActiveFlag_TCR(_i2cx))
			{
				if (LL_I2C_IsActiveFlag_TXIS(_i2cx))
				{
					LL_I2C_TransmitData8(_i2cx,*data++);
				}
//				if ((time--) == 0)
//				{
//					return E_TIMEOUT;
//				}
			}
			num_to_write = num_to_write - 255;
		}
		// num_to_writ<=255: AUTOEND,NBYTE=num_to_writ,NOSTART
		LL_I2C_HandleTransfer(_i2cx,slave_address,LL_I2C_ADDRESSING_MODE_7BIT,num_to_write,LL_I2C_MODE_AUTOEND,LL_I2C_GENERATE_NOSTARTSTOP);
//		time = _timeout;
		while (!LL_I2C_IsActiveFlag_STOP(_i2cx))
		{
			if (LL_I2C_IsActiveFlag_TXIS(_i2cx))
			{
				LL_I2C_TransmitData8(_i2cx,*data++);
			}
//			if ((time--) == 0)
//			{
//				return E_TIMEOUT;
//			}
		}
		LL_I2C_ClearFlag_STOP(_i2cx);
	}else{
		// 写入地址寄存器和数据
		LL_I2C_HandleTransfer(_i2cx,slave_address,LL_I2C_ADDRESSING_MODE_7BIT,num_to_write,LL_I2C_MODE_AUTOEND,LL_I2C_GENERATE_NOSTARTSTOP);
//		time = _timeout;
		while (!LL_I2C_IsActiveFlag_STOP(_i2cx))
		{
			if (LL_I2C_IsActiveFlag_TXIS(_i2cx))
			{
				LL_I2C_TransmitData8(_i2cx,*data++);
			}
//			if ((time--) == 0)
//			{
//				return E_TIMEOUT;
//			}
		}
		LL_I2C_ClearFlag_STOP(_i2cx);
	}

	return E_OK;
}

/**
 *@name     readBuf(uint8_t slave_address, uint8_t reg_address, uint8_t *data,uint16_t num_to_read)
 *@brief    I2C从指定位置连续读取 start->write(reg_address)->read(data....)->stop
 *@param    I2Cx:  I2C1,I2C2
 *					uint8_t slave_address：从设备地址
 *					uint8_t reg_address：要读取的位置
 *          uint8_t *data:  数据缓冲，保存读到的数据
 *          uint16_t num_to_write  要读取的数据长度
 *@retval   状态 E_OK 成功； E_TIMEOUT 超时
*/
int8_t E_I2c::readBuf(uint8_t slave_address, uint8_t reg_address, uint8_t *data,uint16_t num_to_read)
{
//	uint16_t time = _timeout;
	// 发送地址寄存器
	LL_I2C_HandleTransfer(_i2cx,slave_address,LL_I2C_ADDRESSING_MODE_7BIT,1,LL_I2C_MODE_SOFTEND,LL_I2C_GENERATE_START_WRITE);
	while (!LL_I2C_IsActiveFlag_TC(_i2cx))
	{
		if (LL_I2C_IsActiveFlag_TXIS(_i2cx))
		{
			LL_I2C_TransmitData8(_i2cx,reg_address);
		}
//		if ((time--) == 0)
//		{
//			return E_TIMEOUT;
//		}
	}
	// 发送读指令，从当前地址开始读取数据
	LL_I2C_HandleTransfer(_i2cx,slave_address,LL_I2C_ADDRESSING_MODE_7BIT,num_to_read,LL_I2C_MODE_AUTOEND,LL_I2C_GENERATE_START_READ);
//	time = _timeout;
	while (!LL_I2C_IsActiveFlag_STOP(_i2cx))
	{
		if (LL_I2C_IsActiveFlag_RXNE(_i2cx))
		{
			*data++ = LL_I2C_ReceiveData8(_i2cx);
		}
//		if ((time--) == 0)
//		{
//			return E_TIMEOUT;
//		}
	}
	LL_I2C_ClearFlag_STOP(_i2cx);
	return E_OK;
}

/**
 *@name     readBuf(uint8_t slave_address,uint8_t *data, uint16_t num_to_read)
 *@brief    I2C连续读
 *@param    I2Cx:  I2C1,I2C2
 *          uint8_t *data:  数据缓冲，保存读到的数据
 *          uint16_t num_to_write  要读取的数据长度
 *@retval   状态 E_OK 成功； E_TIMEOUT 超时
*/
int8_t E_I2c::readBuf(uint8_t slave_address,uint8_t *data, uint16_t num_to_read)
{
//	uint16_t time = _timeout;
	// 发送读指令，从当前地址开始读取数据
	LL_I2C_HandleTransfer(_i2cx,slave_address,LL_I2C_ADDRESSING_MODE_7BIT,num_to_read,LL_I2C_MODE_AUTOEND,LL_I2C_GENERATE_START_READ);
	while (!LL_I2C_IsActiveFlag_STOP(_i2cx))
	{
		if (LL_I2C_IsActiveFlag_RXNE(_i2cx))
		{
			*data++ = LL_I2C_ReceiveData8(_i2cx);
		}
//		if ((time--) == 0)
//		{
//			return E_TIMEOUT;
//		}
	}
	LL_I2C_ClearFlag_STOP(_i2cx);
	return E_OK;
}

/**
 *@name     wait_dev_busy(uint8_t slave_address)
 *@brief    等待设备空闲。向指定设备发送start指令，如果设备忙，则返回NACK,否则返回ACK,主设备发送stop指令
 *@param    slave_address:  设备地址
 *@retval   None
*/
int8_t E_I2c::wait_dev_busy(uint8_t slave_address)
{
	uint8_t tryg = 0;
	uint8_t i = 255;

	do
	{
		// clear STOP & NACK flag
		SET_BIT(_i2cx->ICR,LL_I2C_ICR_NACKCF | LL_I2C_ICR_STOPCF);

		LL_I2C_HandleTransfer(_i2cx,slave_address,LL_I2C_ADDRESSING_MODE_7BIT,0,LL_I2C_MODE_AUTOEND,LL_I2C_GENERATE_START_WRITE);

		while (i--);

		if (tryg++ == 254)
		{
			return 1;
		}
	}while (LL_I2C_IsActiveFlag_NACK(_i2cx) == 1); //如果无响应，则继续等待

	LL_I2C_ClearFlag_STOP(_i2cx);

	return 0;
}

int8_t E_I2c::take_i2c_right(uint32_t timing)
{
	while (busy == 1)
	{
		delay_ms(1);
	}
	_timing = timing;
	config();
	busy = 1;
	return 0;
}

int8_t E_I2c::release_i2c_right(void)
{
	busy = 0;
	return 0;
}
