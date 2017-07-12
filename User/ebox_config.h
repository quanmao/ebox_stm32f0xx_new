/**
  ******************************************************************************
  * @file    ebox_config.h
  * @author  cat_li
  * @version V2.0
  * @date    2016/10/21
  * @brief   ebox���òü��ļ�
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

//�Ƿ�ʹ��DEBUG  1 ʹ��  0 ��ʹ��
#define EBOX_DEBUG  1
//���ݲ�ͬ������豸���ò�ͬ���������������Ҫ��֤�Ѿ���ʼ��
#if EBOX_DEBUG
#define DBG(...) usart.printf(__VA_ARGS__)
#else
#define DBG(...)
#endif

/*�ڴ���С����̬�����ڴ�ʱ����,��С���׵��¶�β��ܷ���ɹ��������˷��ڴ档�����
 *���������ļ� starup_stm32fxxxx.s �е� Heap_Size
 */
#define	BUF_BLOCK		64


#endif

