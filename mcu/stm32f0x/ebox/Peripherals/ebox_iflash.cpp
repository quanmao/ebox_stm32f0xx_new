/**
  ******************************************************************************
  * @file    iflash.cpp
  * @author  shentq
  * @version V1.2
  * @date    2016/08/14
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
/**
 * Modification History:
 * -LQM (2016/8/30)
 *      *��ֲ��STM32F0,����HAL��LL��
 */

/* Includes ------------------------------------------------------------------*/
#include "ebox_iflash.h"

#define FLASH_BSY             FLASH_SR_BSY            /*!< FLASH Busy flag                           */ 
#define FLASH_PGERR           FLASH_SR_PGERR          /*!< FLASH Programming error flag    */
#define FLASH_WRPERR          FLASH_SR_WRPERR         /*!< FLASH Write protected error flag          */
#define FLASH_EOP             FLASH_SR_EOP            /*!< FLASH End of Operation flag               */

#define FLASH_GET_FLAG(__FLAG__)   (((FLASH->SR) & (__FLAG__)) == (__FLAG__))
#define FLASH_CLEAR_FLAG(__FLAG__)   ((FLASH->SR) = (__FLAG__))

void ebox_FLASH_Unlock(void)
{
    if (((FLASH->CR) & (FLASH_CR_LOCK)) != RESET)
    {
        /* Authorize the FLASH Registers access */
        WRITE_REG(FLASH->KEYR, FLASH_KEY1);
        WRITE_REG(FLASH->KEYR, FLASH_KEY2);
    }
}

void ebox_FLASH_Lock(void)
{
	/* Set the LOCK Bit to lock the FLASH Registers access */
	SET_BIT(FLASH->CR, FLASH_CR_LOCK);
}

void ebox_Program_HalfWord(uint32_t Address, uint16_t Data)
{
	/* Check FLASH End of Operation flag  */
	//while (!FLASH_GET_FLAG(FLASH_EOP)){}
	while (FLASH_GET_FLAG(FLASH_BSY)){}
    /* Proceed to program the new data */
    SET_BIT(FLASH->CR, FLASH_CR_PG);

  /* Write data in the address */
  *(__IO uint16_t*)Address = Data;
}

void ebox_FLASH_PageErase(uint32_t PageAddress)
{
    /* Proceed to erase the page */
    SET_BIT(FLASH->CR, FLASH_CR_PER);
    WRITE_REG(FLASH->AR, PageAddress);
    SET_BIT(FLASH->CR, FLASH_CR_STRT);
}


uint16_t E_Flash::write_without_check(uint32_t iAddress, uint8_t *buf, uint16_t iNumByteToWrite)
{
    uint16_t i;
//    volatile FLASH_Status FLASHStatus = FLASH_COMPLETE;
    i = 0;

    ebox_FLASH_Unlock();

    while ((i < iNumByteToWrite)/* && (FLASH_GET_FLAG(FLASH_EOP))*/)
    {
        ebox_Program_HalfWord(iAddress, *(uint16_t *)buf);
        i = i + 2;
        iAddress = iAddress + 2;
        buf = buf + 2;
    }

    ebox_FLASH_Lock();

    return iNumByteToWrite;
}
/**
  * @brief  Programs a half word at a specified Option Byte Data address.
  * @note   This function can be used for all STM32F10x devices.
  * @param  Address: specifies the address to be programmed.
  * @param  buf: specifies the data to be programmed.
  * @param  iNbrToWrite: the number to write into flash
  * @retval if success return the number to write, -1 if error
  *
  */
#define FLASH_PAGE_S FLASH_PAGE_SIZE
int E_Flash::write(uint32_t iAddress, uint8_t *buf, uint32_t iNbrToWrite)
{
    /* Unlock the Flash Bank1 Program Erase controller */
    uint32_t secpos;
    uint32_t iNumByteToWrite = iNbrToWrite;
    uint16_t secoff;
    uint16_t secremain;
    uint16_t i = 0;
//    static uint8_t tmp[FLASH_PAGE_SIZE];
	  static uint8_t tmp[FLASH_PAGE_S];
//    volatile FLASH_Status FLASHStatus = FLASH_COMPLETE;

    ebox_FLASH_Unlock();
    secpos = iAddress & (~(FLASH_PAGE_SIZE - 1 )) ; //������ַ(ȡ12-31bitλ)
    secoff = iAddress & (FLASH_PAGE_SIZE - 1);  	//�������ڵ�ƫ��(0-11bitλ)
    secremain = FLASH_PAGE_SIZE - secoff;       	//����ʣ��ռ��С
	// ��ǰ����ʣ��ռ����д������
    if (iNumByteToWrite <= secremain) secremain = iNumByteToWrite; //������4096���ֽ�

    while ( 1 )
    {
        read(secpos, tmp, FLASH_PAGE_S);   //������������
		// �ж���Ҫд��Ĳ����Ƿ�������
        for (i = 0; i < secremain; i++)  	//У������
        {
            if (tmp[secoff + i] != 0XFF)break;  //��Ҫ����
        }
        if (i < secremain) //��Ҫ����
        {
            ebox_FLASH_PageErase(secpos); //�����������
			// ����Ҫд�������д�뻺����
            for (i = 0; i < secremain; i++) //����
            {
                tmp[i + secoff] = buf[i];
            }
			// ������������д������
            write_without_check(secpos , tmp , FLASH_PAGE_S); //д����������
        }
        else
        {
            write_without_check(iAddress, buf, secremain); //д�Ѿ������˵�,ֱ��д������ʣ������.
        }
		// ��ǰ����ʣ��ռ乻д��
        if (iNumByteToWrite == secremain) //д�������
            break;
        else	//����д�룬�Ƶ���һ������
        {
            secpos += FLASH_PAGE_SIZE;
            secoff = 0;//ƫ��λ��Ϊ0
            buf += secremain;  //ָ��ƫ��
            iAddress += secremain;//д��ַƫ��
            iNumByteToWrite -= secremain;  //�ֽ����ݼ�
            if (iNumByteToWrite > FLASH_PAGE_SIZE){
				secremain = FLASH_PAGE_SIZE; //��һ����������д����
			}else{
				secremain = iNumByteToWrite;  //��һ����������д����
			}
        }
    }
    ebox_FLASH_Lock();
    return iNbrToWrite;
}


/**
  * @brief  Programs a half word at a specified Option Byte Data address.
  * @note   This function can be used for all STM32F10x devices.
  * @param  Address: specifies the address to be programmed.
  * @param  buf: specifies the data to be programmed.
  * @param  iNbrToWrite: the number to read from flash
  * @retval if success return the number to write, without error
  *
  */
int E_Flash::read(uint32_t iAddress, uint8_t *buf, int32_t iNbrToRead)
{
    int i = 0;
    while(i < iNbrToRead )
    {
        *(buf + i) = *(__IO uint8_t *) iAddress++;				
        i++;
    }
    return i;
}

