/**
  ******************************************************************************
  * @file    pwm.cpp
  * @author  shentq
  * @version V1.2
  * @date    2016/08/14
  * @brief   ebox application example .
  *	 	2017/7/15	cat_li
		1	移植到ebox stm32f0平台
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


#include "ebox.h"
#include "math.h"

// 串口，led
E_UART usart(USART1,PA_9,PA_10);
E_GPIO led(PA_5);

//E_PWM pwm1(TIM3,PA_6);
E_PWM pwm2(TIM2,PA_3);

void setup()
{
    ebox_init();
    usart.begin(115200);
    usart.printf("core:%d\r\n",cpu.clock.core);
    usart.printf("hclk:%d\r\n",cpu.clock.hclk);
    usart.printf("pclk1:%d\r\n",cpu.clock.pclk1);

//		pwm1.begin(40000,100);
		pwm2.begin(4000,500);
	//pwm1.SetPorlicy(0);
//    usart.printf("max frq = %dKhz\r\n",pwm1.GetMaxFrequency()/1000);
	usart.printf("max frq = %dKhz\r\n",pwm2.GetMaxFrequency()/1000);
    //usart.printf("max frq = %f\r\n",pwm1.get_accuracy());
}

float x;
uint16_t y;

int main(void)
{
    setup();

    while(1)
    {
//        x = x + PI * 0.01;
//        if(x >= PI)x = 0;
//        y = 2000 - (sin(x) + 1) * 1000;

//       // pwm1.SetDutyCycle(y);
//        delay_ms(10);
    }

}




