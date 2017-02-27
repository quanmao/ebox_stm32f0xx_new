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

/* Includes ------------------------------------------------------------------*/
#include "ebox_i2c.h"
#include "ebox_core.h"
#include "stm32f0xx_ll_rcc.h"


// ��Ƶ�����������֣��ߵ�ƽ���͵�ƽ
#define I2C_TIMING    __LL_I2C_CONVERT_TIMINGS(0xb, 0xc7, 0xc3, 0x02, 0x04)

/**
 *@name     I2c(I2C_TypeDef *I2Cx, Gpio *scl_pin, Gpio *sda_pin)
 *@brief      I2C���캯��
 *@param    I2Cx:  I2C1,I2C2
 *          scl_pin:  ʱ��Pin
 *          sda_pin:  ����Pin
 *@retval   None
*/
E_I2c::E_I2c(I2C_TypeDef *I2Cx, E_PinBase *scl, E_PinBase *sda)
{
    busy = 0;
    _i2cx = I2Cx;
		_index = getIndex(scl->_id,I2C_MAP);
    scl->mode(AF_PP_PU,I2C_MAP[getIndex(scl->_id,I2C_MAP)].pin_af);
    sda->mode(AF_PP_PU,I2C_MAP[getIndex(sda->_id,I2C_MAP)].pin_af);
}

/**
 *@name     begin(uint16_t speed)
 *@brief    ����i2cʱ�Ӻ���������speed����timing��Ĭ��400k @8M
 *@param    speed:  ���� 10,100,400 �ֱ����10k��100k��400k
 *@retval   None
*/
void  E_I2c::begin(uint16_t speed)
{
		switch(cpu.clock.pclk1/1000000){
			case 16:
					switch(speed){
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
					switch(speed){
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
			default:	// Ĭ��ϵͳʱ��Ϊ8M
				switch(speed){
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
		// ʹ��I2Cʱ��
		LL_APB1_GRP1_EnableClock(I2C_MAP[_index].Periph);
		// I2C1 ��Ҫѡ���ض���ʱ��
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
 *@brief    I2Cд��һ���ֽ�. start->data->stop
 *@param    I2Cx:  I2C1,I2C2
 *          uint8_t data:  Ҫд�������
 *@retval   None
*/
int8_t E_I2c::writeChar(uint8_t slave_address,uint8_t data)
{
    // д���ַ�Ĵ���������
    LL_I2C_HandleTransfer(_i2cx,slave_address,LL_I2C_ADDRESSING_MODE_7BIT,1,LL_I2C_MODE_AUTOEND,LL_I2C_GENERATE_START_WRITE);

    while (!LL_I2C_IsActiveFlag_STOP(_i2cx))
    {
        if (LL_I2C_IsActiveFlag_TXIS(_i2cx))
        {
            LL_I2C_TransmitData8(_i2cx,data);
        }
    }
    LL_I2C_ClearFlag_STOP(_i2cx);

    return 0;
}

/**
 *@name     writeBuf(uint8_t slave_address, uint8_t *data, uint16_t num_to_write)
 *@brief    I2C����д start->data....->stop
 *@param    I2Cx:  I2C1,I2C2
 *          uint8_t *data:  Ҫд�������
 *          uint16_t num_to_write  Ҫд������ݳ���
 *@retval   None
*/
int8_t E_I2c::writeBuf(uint8_t slave_address, uint8_t *data, uint16_t num_to_write)
{
    if (num_to_write >255){
        // num_to_writ>255: RELOAD,NBYTE=0xFF,START
        LL_I2C_HandleTransfer(_i2cx,slave_address,LL_I2C_ADDRESSING_MODE_7BIT,0xFF,LL_I2C_MODE_RELOAD,LL_I2C_GENERATE_START_WRITE);

        while (!LL_I2C_IsActiveFlag_TCR(_i2cx))
        {
            if (LL_I2C_IsActiveFlag_TXIS(_i2cx))
            {
                LL_I2C_TransmitData8(_i2cx,*data++);
            }
        }
        num_to_write = num_to_write - 255;
        // num_to_writ>255: RELOAD,NBYTE=0xFF,NOSTART
        while (num_to_write > 255){
            // ���͵�ַ�Ĵ���
            LL_I2C_HandleTransfer(_i2cx,slave_address,LL_I2C_ADDRESSING_MODE_7BIT,0xFF,LL_I2C_MODE_RELOAD,LL_I2C_GENERATE_NOSTARTSTOP);

            while (!LL_I2C_IsActiveFlag_TCR(_i2cx))
            {
                if (LL_I2C_IsActiveFlag_TXIS(_i2cx))
                {
                    LL_I2C_TransmitData8(_i2cx,*data++);
                }
            }
            num_to_write = num_to_write - 255;
        }
        // num_to_writ<=255: AUTOEND,NBYTE=num_to_writ,NOSTART
        LL_I2C_HandleTransfer(_i2cx,slave_address,LL_I2C_ADDRESSING_MODE_7BIT,num_to_write,LL_I2C_MODE_AUTOEND,LL_I2C_GENERATE_NOSTARTSTOP);

        while (!LL_I2C_IsActiveFlag_STOP(_i2cx))
        {
            if (LL_I2C_IsActiveFlag_TXIS(_i2cx))
            {
                LL_I2C_TransmitData8(_i2cx,*data++);
            }
        }
        LL_I2C_ClearFlag_STOP(_i2cx);
    }else{
        // д���ַ�Ĵ���������
        LL_I2C_HandleTransfer(_i2cx,slave_address,LL_I2C_ADDRESSING_MODE_7BIT,num_to_write,LL_I2C_MODE_AUTOEND,LL_I2C_GENERATE_START_WRITE);

        while (!LL_I2C_IsActiveFlag_STOP(_i2cx))
        {
            if (LL_I2C_IsActiveFlag_TXIS(_i2cx))
            {
                LL_I2C_TransmitData8(_i2cx,*data++);
            }
        }
        LL_I2C_ClearFlag_STOP(_i2cx);
    }

    return 0;
}

/**
 *@name     writeBuf(uint8_t slave_address, uint8_t reg_address, uint8_t *data, uint16_t num_to_write)
 *@brief    ��ָ��λ������д������ start->reg_address->data....->stop
 *@param    I2Cx:  I2C1,I2C2
 *          uint8_t reg_address:�Ĵ�����ַ
 *          uint8_t *data:  Ҫд�������
 *          uint16_t num_to_write  Ҫд������ݳ���
 *@retval   None
*/
int8_t E_I2c::writeBuf(uint8_t slave_address, uint8_t reg_address, uint8_t *data, uint16_t num_to_write)
{
    // ���͵�ַ�Ĵ���
    LL_I2C_HandleTransfer(_i2cx,slave_address,LL_I2C_ADDRESSING_MODE_7BIT,1,LL_I2C_MODE_SOFTEND,LL_I2C_GENERATE_START_WRITE);
    while (!LL_I2C_IsActiveFlag_TC(_i2cx))
    {
        if (LL_I2C_IsActiveFlag_TXIS(_i2cx))
        {
            LL_I2C_TransmitData8(_i2cx,reg_address);
        }
    }
    // ��������
    if (num_to_write >255){
        // num_to_writ>255: RELOAD,NBYTE=0xFF,NOSTART
        while (num_to_write > 255){
            // ���͵�ַ�Ĵ���
            LL_I2C_HandleTransfer(_i2cx,slave_address,LL_I2C_ADDRESSING_MODE_7BIT,0xFF,LL_I2C_MODE_RELOAD,LL_I2C_GENERATE_NOSTARTSTOP);

            while (!LL_I2C_IsActiveFlag_TCR(_i2cx))
            {
                if (LL_I2C_IsActiveFlag_TXIS(_i2cx))
                {
                    LL_I2C_TransmitData8(_i2cx,*data++);
                }
            }
            num_to_write = num_to_write - 255;
        }
        // num_to_writ<=255: AUTOEND,NBYTE=num_to_writ,NOSTART
        LL_I2C_HandleTransfer(_i2cx,slave_address,LL_I2C_ADDRESSING_MODE_7BIT,num_to_write,LL_I2C_MODE_AUTOEND,LL_I2C_GENERATE_NOSTARTSTOP);

        while (!LL_I2C_IsActiveFlag_STOP(_i2cx))
        {
            if (LL_I2C_IsActiveFlag_TXIS(_i2cx))
            {
                LL_I2C_TransmitData8(_i2cx,*data++);
            }
        }
        LL_I2C_ClearFlag_STOP(_i2cx);
    }else{
        // д���ַ�Ĵ���������
        LL_I2C_HandleTransfer(_i2cx,slave_address,LL_I2C_ADDRESSING_MODE_7BIT,num_to_write,LL_I2C_MODE_AUTOEND,LL_I2C_GENERATE_NOSTARTSTOP);

        while (!LL_I2C_IsActiveFlag_STOP(_i2cx))
        {
            if (LL_I2C_IsActiveFlag_TXIS(_i2cx))
            {
                LL_I2C_TransmitData8(_i2cx,*data++);
            }
        }
        LL_I2C_ClearFlag_STOP(_i2cx);
    }

    return 0;
}

/**
 *@name     readBuf(uint8_t slave_address, uint8_t reg_address, uint8_t *data,uint16_t num_to_read)
 *@brief    I2C��ָ��λ��������ȡ start->write(reg_address)->read(data....)->stop
 *@param    I2Cx:  I2C1,I2C2
 *					uint8_t slave_address�����豸��ַ
 *					uint8_t reg_address��Ҫ��ȡ��λ��
 *          uint8_t *data:  ���ݻ��壬�������������
 *          uint16_t num_to_write  Ҫ��ȡ�����ݳ���
 *@retval   None
*/
int8_t E_I2c::readBuf(uint8_t slave_address, uint8_t reg_address, uint8_t *data,uint16_t num_to_read)
{
    // ���͵�ַ�Ĵ���
    LL_I2C_HandleTransfer(_i2cx,slave_address,LL_I2C_ADDRESSING_MODE_7BIT,1,LL_I2C_MODE_SOFTEND,LL_I2C_GENERATE_START_WRITE);
    while (!LL_I2C_IsActiveFlag_TC(_i2cx))
    {
        if (LL_I2C_IsActiveFlag_TXIS(_i2cx))
        {
            LL_I2C_TransmitData8(_i2cx,reg_address);
        }
    }
    // ���Ͷ�ָ��ӵ�ǰ��ַ��ʼ��ȡ����
    LL_I2C_HandleTransfer(_i2cx,slave_address,LL_I2C_ADDRESSING_MODE_7BIT,num_to_read,LL_I2C_MODE_AUTOEND,LL_I2C_GENERATE_START_READ);
    while (!LL_I2C_IsActiveFlag_STOP(_i2cx))
    {
        if (LL_I2C_IsActiveFlag_RXNE(_i2cx))
        {
            *data++ = LL_I2C_ReceiveData8(_i2cx);
        }
    }
    LL_I2C_ClearFlag_STOP(_i2cx);
    return 0;
}

/**
 *@name     readBuf(uint8_t slave_address,uint8_t *data, uint16_t num_to_read)
 *@brief    I2C������
 *@param    I2Cx:  I2C1,I2C2
 *          uint8_t *data:  ���ݻ��壬�������������
 *          uint16_t num_to_write  Ҫ��ȡ�����ݳ���
 *@retval   None
*/
int8_t E_I2c::readBuf(uint8_t slave_address,uint8_t *data, uint16_t num_to_read)
{
    // ���Ͷ�ָ��ӵ�ǰ��ַ��ʼ��ȡ����
    LL_I2C_HandleTransfer(_i2cx,slave_address,LL_I2C_ADDRESSING_MODE_7BIT,num_to_read,LL_I2C_MODE_AUTOEND,LL_I2C_GENERATE_START_READ);
    while (!LL_I2C_IsActiveFlag_STOP(_i2cx))
    {
        if (LL_I2C_IsActiveFlag_RXNE(_i2cx))
        {
            *data++ = LL_I2C_ReceiveData8(_i2cx);
        }
    }
    LL_I2C_ClearFlag_STOP(_i2cx);
    return 0;
}

/**
 *@name     wait_dev_busy(uint8_t slave_address)
 *@brief    �ȴ��豸���С���ָ���豸����startָ�����豸æ���򷵻�NACK,���򷵻�ACK,���豸����stopָ��
 *@param    slave_address:  �豸��ַ
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
    }while (LL_I2C_IsActiveFlag_NACK(_i2cx) == 1); //�������Ӧ��������ȴ�

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
