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
 * -LQM (2017/7/12)
 *     ע�⣺�˲��ֺ�F1ϵ�в�ͨ�ã��ṩ��2��ADCģʽ��һ���ǵ�ͨ���βɼ���������DMA
 *  һ�������ɼ���ָ��������������ͨ�����ͨ���� 
 *  ��ͨ�����βɼ�,�ο���ѹ������ebox_analog.h�� VDDA_APPLI 
 *    1  ��������   E_Analog adc1(new E_PinBase(PA_0));
 *    2  �ṩread()��getVoltage()����
 */

#include "ebox.h"

E_UART usart(USART1,PA_9,PA_10);

E_Analog adc1(new E_PinBase(PA_0));
E_AnalogDMA adcs(new E_PinBase(PA_0));

void setup()
{
    ebox_init();
    usart.begin(115200);
	  usart.printf("****ADC TEST ****** \r\n");
	  
//	  adcs.Add(&PA1);
//	  adcs.Add(&PA2);
}

int main(void)
{
    setup();
    while(1)
    {
			  usart.printf("\r\n****ADC TEST ONE ****** \r\n");
        usart.printf("PA0 = %d \r\n",adc1.read());
			  usart.printf("PA0 Voltage = %d mv \r\n",adc1.getVoltage());
        delay_ms(2000);
			  usart.printf("****ADC TEST CONTINUOUS****** \r\n");
//			  adcs.update();
//			  usart.printf("PA1_1 = %d  PA1_2 = %d  PA2_1 = %d  PA2_2 = %d \r\n",adcs.Buffer[0],adcs.Buffer[2],adcs.Buffer[1],adcs.Buffer[3]);
        delay_ms(2000);
		}
}