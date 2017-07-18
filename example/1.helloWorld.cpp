 /**
  ******************************************************************************
  * @file    main.cpp
  * @author  shentq
  * @version V1.2
  * @date    2017/07/07
  * @brief   uart例程，包含中断绑定 .
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

#include "ebox.h"

E_GPIO PA5(PA_5);
E_UART usart(USART1,PA_9,PA_10);

// 接收中断回调函数
void rxirq(void){
	PA5 = 1;
}

void setup()
{
	ebox_init();
	PA5.mode(OUTPUT_PP);
	usart.begin(115200);
	// 绑定中断回调函数，可以绑定对象成员，也可以绑定静态函数
	usart.attach(&PA5,&E_GPIO::reset,TxIrq);
	usart.attach(&rxirq,RxIrq);
	// 开中断
	usart.enable_irq(TxIrq);
	usart.enable_irq(RxIrq);
}
char buf[] = "hello world !\r\n";
int main(void)
{
    setup();
    while(1)
    {
        usart.print("hello World !\r\n");
        usart.print("stm32f0 usart print %d \r\n");
        delay_ms(1000);
    }
}




