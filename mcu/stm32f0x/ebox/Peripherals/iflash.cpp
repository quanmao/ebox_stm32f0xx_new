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
 *      *移植到STM32F0,基于HAL库LL层
 */

/* Includes ------------------------------------------------------------------*/
#include "iflash.h"

void ebox_FLASH_Unlock(void)
{
//	if (HAL_IS_BIT_SET(FLASH->CR, FLASH_CR_LOCK))
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


uint16_t Flash::write_without_check(uint32_t iAddress, uint8_t *buf, uint16_t iNumByteToWrite)
{
    uint16_t i;
//    volatile FLASH_Status FLASHStatus = FLASH_COMPLETE;
    i = 0;

    ebox_FLASH_Unlock();

    while ((i < iNumByteToWrite)/* && (FLASHStatus == FLASH_COMPLETE)*/)
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

int Flash::write(uint32_t iAddress, uint8_t *buf, uint32_t iNbrToWrite)
{
    /* Unlock the Flash Bank1 Program Erase controller */
    uint32_t secpos;
    uint32_t iNumByteToWrite = iNbrToWrite;
    uint16_t secoff;
    uint16_t secremain;
    uint16_t i = 0;
//    static uint8_t tmp[FLASH_PAGE_SIZE];
	uint8_t tmp[FLASH_PAGE_SIZE];
//    volatile FLASH_Status FLASHStatus = FLASH_COMPLETE;

//    FLASH_UnlockBank1();
    ebox_FLASH_Unlock();
    secpos = iAddress & (~(FLASH_PAGE_SIZE - 1 )) ; //扇区地址
    secoff = iAddress & (FLASH_PAGE_SIZE - 1);  //在扇区内的偏移
    secremain = FLASH_PAGE_SIZE - secoff;       //扇区剩余空间大小

    if (iNumByteToWrite <= secremain) secremain = iNumByteToWrite; //不大于4096个字节

    while ( 1 )
    {
        read(secpos, tmp, FLASH_PAGE_SIZE);   //读出整个扇区
        for (i = 0; i < secremain; i++)  //校验数据
        {
            if (tmp[secoff + i] != 0XFF)break;  //需要擦除
        }
        if (i < secremain) //需要擦除
        {
            ebox_FLASH_PageErase(secpos);
//            FLASHStatus = FLASH_ErasePage(secpos); //擦除这个扇区
//            if(FLASHStatus != FLASH_COMPLETE)
//                return -1;
            for (i = 0; i < secremain; i++) //复制
            {
                tmp[i + secoff] = buf[i];
            }
            write_without_check(secpos , tmp , FLASH_PAGE_SIZE); //写入整个扇区
        }
        else
        {
            write_without_check(iAddress, buf, secremain); //写已经擦除了的,直接写入扇区剩余区间.
        }

        if (iNumByteToWrite == secremain) //写入结束了
            break;
        else
        {
            secpos += FLASH_PAGE_SIZE;
            secoff = 0;//偏移位置为0
            buf += secremain;  //指针偏移
            iAddress += secremain;//写地址偏移
            iNumByteToWrite -= secremain;  //字节数递减
            if (iNumByteToWrite > FLASH_PAGE_SIZE) secremain = FLASH_PAGE_SIZE; //下一个扇区还是写不完
            else secremain = iNumByteToWrite;  //下一个扇区可以写完了
        }

    }
    ebox_FLASH_Lock();
//    FLASH_LockBank1();
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
int Flash::read(uint32_t iAddress, uint8_t *buf, int32_t iNbrToRead)
{
    int i = 0;
    while(i < iNbrToRead )
    {
        *(buf + i) = *(__IO uint8_t *) iAddress++;
        i++;
    }
    return i;
}

