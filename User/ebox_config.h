/**
  ******************************************************************************
  * @file    ebox_config.h
  * @author  cat_li
  * @version V2.0
  * @date    2016/10/21
  * @brief   ebox配置裁剪文件
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

#ifndef __EBOX_CONFIG_H
#define __EBOX_CONFIG_H

//是否使用DEBUG,需要在ebox_debug.h文件中定义输出设备 1 使用  0 不使用
#define EBOX_DEBUG  1

/*内存块大小，动态分配内存时增量,过小容易导致多次才能分配成功，过大浪费内存。最大不能
 *超过启动文件 starup_stm32fxxxx.s 中的 Heap_Size
 *printf模块中调用
 */
#define	BUF_BLOCK		16	//64

/* flash写入新数据时，是否擦除覆盖当前区块的其它数据; 
 * 0 写入新数据，并保留当前区块其他位置旧数据。 需要占用FLASH_PAGE_SIZE大小的内存作为缓冲区
 * 1 只保留新写入的数据。 节省内存
 */
#define FLASH_OVERRIDE	1

//是否使用超时,1 使用  0 不使用
#define USE_TIMEOUT	1
//是否使用内部参考电压 1 使用, 0 不使用
#define USE_INTERVREF	0


#endif

