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

//是否使用DEBUG  1 使用  0 不使用
#define EBOX_DEBUG  1
//根据不同的输出设备调用不同的输出函数，并且要保证已经初始化
#if EBOX_DEBUG
#define DBG(...) usart.printf(__VA_ARGS__)
#else
#define DBG(...)
#endif

/*内存块大小，动态分配内存时增量,过小容易导致多次才能分配成功，过大浪费内存。最大不能
 *超过启动文件 starup_stm32fxxxx.s 中的 Heap_Size
 */
#define	BUF_BLOCK		64


#endif

