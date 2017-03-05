/**
  ******************************************************************************
  * @file   : *.cpp
  * @author : cat_li
  * @version: V2.0
  * @date   : 2016/11/10

  * @brief   ebox application example .
  *
  * Copyright 2016 shentq. All Rights Reserved.
  ******************************************************************************
 */


#include "ebox.h"
#include <stdio.h>
#include "LiquidCrystal_I2C.h"
#include "driver_led.h"


E_GPIO led(PA_5);
E_GPIO led2(PA_5);

/**
 *@brief    回调函数，调用周期取决于systick中断回调注册函数。如果systick中断只处理一个任务，使用
 *@param    NONE
 *@retval   NONE
*/
void SystickCallBack(void)
{
    led.toggle();
}

void led2On(){
   // led2.set();
    led.toggle();
}

void led2Off(){
    //led2.reset();
}

// 可以在systick里调用多个函数。需要调用的函数保存在数组里，每次只调用一个或2个
// 函数指针数组，保存需要调用的函数指针
fun_noPara_t funTable[10] = {
    led2Off,led2On,nullFun,nullFun,nullFun,
    nullFun,nullFun,nullFun,nullFun,nullFun
};

/**
 *@brief    回调函数
 *@param    NONE
 *@retval   NONE
*/
void SystickCallBack1(void){
    static uint8_t i;
    i = (i>249)?0:i+1;
    // 每10倍周期调用一个函数（预先保存在数组里）如果SystickCallBack1每1ms被调用一次，
    // 则下面的函数每10ms被调用一次，修改10可以修改倍数。好处在于每10ms间隔的时候只
    // 调用一个函数，减少每次中断中执行的代码数量
    switch (i%10)
    {
    case 0:
        funTable[0]();
        break;
    case 1:
        funTable[1]();
        break;
    case 2:
        funTable[2]();
        break;
    case 3:
        funTable[3]();
        break;
    default:
        break;
    }
    // 类似于上面，每100被周期调用一次
    switch (i%100){
    default:
        break;
    }
}

void setup()
{
    ebox_init();
    led.mode(OUTPUT_PP);
    led2.mode(OUTPUT_PP);

    // 默认周期回调。1ms
    //SystickCallBackRegister(SystickCallBack);
    // 指定回调周期，回调周期是系统默认周期的N倍
    //SystickCallBackRegister1(SystickCallBack,1000);

    // 默认周期回调。1ms
    //SystickCallBackRegister(SystickCallBack1);
    // 指定回调周期，回调周期是系统默认周期的N倍
    SystickCallBackRegister1(SystickCallBack1,50);
}

int main(void){
    setup();
    while (1)
    {
    }
}
