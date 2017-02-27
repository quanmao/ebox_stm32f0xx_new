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
 *     ע�⣺�˲��ֺ�F1ϵ�в�ͨ�ã��ṩ��2��ADCģʽ��һ���ǵ�ͨ���βɼ���������DMA
 *  һ�������ɼ���ָ��������������ͨ�����ͨ���� 
 *  ��ͨ�����βɼ�,�ο���ѹ������analog.h�� VDDA_APPLI 
 *    1  ��������   Analog  adc1(&PA0)
 *    2  �ṩread()��read_Voltage()����
 *  �����ɼ�
 *    1  ��������(��Ҫͬʱָ��ÿͨ�������Ĵ���)  AnalogDMA adcs(2)
 *    2  ��Ӳɼ�ͨ��  Add(&PA1);Add(&PA2);......
 *    3  ȡֵǰ��������  update()
 *    4  ȡֵ  adcs.buffer[]
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





