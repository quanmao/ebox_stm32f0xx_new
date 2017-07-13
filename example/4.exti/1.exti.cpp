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

// 分别创建串口，exti，gpio对象
E_GPIO	PA5(PC_13);
E_UART usart(USART1,PA_9,PA_10);
E_EXTI userbtn(PC_13);
E_GPIO led(PA_5);

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
//    ex.begin();
//    userbtn.attach(fall,FALLING);
//		userbtn.attach(rise,RISING);
		userbtn.attach(fallrise,FALL_RISING);
	
//    userbtn.attach(&test,&Test::event);
    userbtn.ENABLE(FALL_RISING);
}


int main(void)
{
    setup();
    while(1)
    {
        ;
    }


}




