 /**
  ******************************************************************************
  * @file    main.cpp
  * @author  shentq
  * @version V1.2
  * @date    2016/08/14
  * @brief   ADC test (STM32F0)
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
 * -LQM (2016/9/19)
 *     注意：此部分和F1系列不通用，提供了2中ADC模式，一种是单通单次采集，不适用DMA
 *  一种连续采集（指定采样次数，单通道或多通道） 
 *  单通道单次采集,参考电压定义在analog.h中 VDDA_APPLI 
 *    1  声明对象   Analog  adc1(&PA0)
 *    2  提供read()和read_Voltage()方法
 *  连续采集
 *    1  声明对象(需要同时指定每通道采样的次数)  AnalogDMA adcs(2)
 *    2  添加采集通道  Add(&PA1);Add(&PA2);......
 *    3  取值前更新数据  update()
 *    4  取值  adcs.buffer[]
 */

#include "ebox.h"

Analog adc1(&PA0);
AnalogDMA adcs(2);

void setup()
{
    ebox_init();
    uart1.begin(115200);
	  uart1.printf("****ADC TEST ****** \r\n");
	  
	  adcs.Add(&PA1);
	  adcs.Add(&PA2);
}

int main(void)
{
    setup();
    while(1)
    {
			  uart1.printf("\r\n****ADC TEST ONE ****** \r\n");
        uart1.printf("PA0 = %d \r\n",adc1.read());
			  uart1.printf("PA0 Voltage = %d mv \r\n",adc1.read_voltage());
        delay_ms(2000);
			  uart1.printf("****ADC TEST CONTINUOUS****** \r\n");
			  adcs.update();
			  uart1.printf("PA1_1 = %d  PA1_2 = %d  PA2_1 = %d  PA2_2 = %d \r\n",adcs.Buffer[0],adcs.Buffer[2],adcs.Buffer[1],adcs.Buffer[3]);
        delay_ms(2000);
		}
}





