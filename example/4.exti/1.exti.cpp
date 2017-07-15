 /**
  ******************************************************************************
  * @file    main.cpp
  * @author  cat_li
  * @version V1.0
  * @date    2017/07/13
  * @brief   ebox exti example, 基于stm32nucleo(072)平台验证	
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

uint32_t xx;

// 串口，led
E_UART usart(USART1,PA_9,PA_10);
E_GPIO led(PA_5);
// 两种方式创建exti对象
E_GPIO	PA5(PC_13);
E_EXTI userbtn(&PA5);
E_EXTI userbt1(PA_0,INPUT_PU);

/**
 *@brief    静态回调函数
 *@param    none
 *@retval   none
*/
void fall()
{
    xx--;
    usart.printf("\r\n falling,xx = %d", xx);
}

void rise()
{
    xx++;
    usart.printf("\r\n rising, xx = %d", xx);
}

void fallrise(){
		xx++;
		usart.printf("\r\n fallrise, xx = %d", xx);
}

/**
 *@brief    测试类，用来测试类成员绑定
 *@param    none
 *@retval   none
*/
class Test 
{
    public:
    void event() 
    {
        led.toggle();
    }
};
Test test;

void setup()
{
    ebox_init();
    usart.begin(115200);
		usart.printf("----------------EXTI TEST----------------\r\n");
    led.mode(OUTPUT_PP);
		// 上升沿，下降沿均触发
		userbtn.attach(fallrise,FALL_RISING);
		userbtn.ENABLE(FALL_RISING);
		// 上升沿，下降沿调用不同的回调函数
    userbt1.attach(rise,RISING);
		userbt1.attach(&test,&Test::event,FALLING);
		userbt1.ENABLE(FALL_RISING);	

}


int main(void)
{
    setup();
    while(1)
    {
        ;
    }


}




