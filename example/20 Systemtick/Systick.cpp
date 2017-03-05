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
 *@brief    �ص���������������ȡ����systick�жϻص�ע�ắ�������systick�ж�ֻ����һ������ʹ��
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

// ������systick����ö����������Ҫ���õĺ��������������ÿ��ֻ����һ����2��
// ����ָ�����飬������Ҫ���õĺ���ָ��
fun_noPara_t funTable[10] = {
    led2Off,led2On,nullFun,nullFun,nullFun,
    nullFun,nullFun,nullFun,nullFun,nullFun
};

/**
 *@brief    �ص�����
 *@param    NONE
 *@retval   NONE
*/
void SystickCallBack1(void){
    static uint8_t i;
    i = (i>249)?0:i+1;
    // ÿ10�����ڵ���һ��������Ԥ�ȱ�������������SystickCallBack1ÿ1ms������һ�Σ�
    // ������ĺ���ÿ10ms������һ�Σ��޸�10�����޸ı������ô�����ÿ10ms�����ʱ��ֻ
    // ����һ������������ÿ���ж���ִ�еĴ�������
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
    // ���������棬ÿ100�����ڵ���һ��
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

    // Ĭ�����ڻص���1ms
    //SystickCallBackRegister(SystickCallBack);
    // ָ���ص����ڣ��ص�������ϵͳĬ�����ڵ�N��
    //SystickCallBackRegister1(SystickCallBack,1000);

    // Ĭ�����ڻص���1ms
    //SystickCallBackRegister(SystickCallBack1);
    // ָ���ص����ڣ��ص�������ϵͳĬ�����ڵ�N��
    SystickCallBackRegister1(SystickCallBack1,50);
}

int main(void){
    setup();
    while (1)
    {
    }
}
