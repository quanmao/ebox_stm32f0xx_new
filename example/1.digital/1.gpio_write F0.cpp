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

/**
 * 1  创建GPIO对象，类名E_GPIO, 参数为PIN_ID; 类似于PA_5形式
 * 2	设置GPIO模式，作为通用GPIO使用时不能配置af功能。可用类型在stm32_define.h中，参考枚举变量 PIN_MODE
 * 3  读写操作
*/

// 定义单个端口
E_GPIO PA5(PA_5);
// 定义同一个port的若干不连续端口, PA0,PA3；不建议使用这种方式
E_PORT P_PORT(E_PORTA,0x09);
//定义同一个port的若干连续端口, PA1-PA2；建议使用这种方式
E_PORT P_PORTS(E_PORTA,2,1);
// 将不同port若干端口打包，作为一个整体进行操作;适合不同port的IO或同一port的IO组合使用
// 可以任意调整 IO顺序
E_BUS P_BUS(PB_0,PB_3,PA_6,PA_7);


void setup()
{
    ebox_init();
    PA5.mode(OUTPUT_PP);
    P_PORT.mode(OUTPUT_PP);
    P_PORTS.mode(OUTPUT_PP);
    P_BUS.mode(OUTPUT_PP);

}

void operationPort(void){
    uint16_t r,s;
    // 这一步两个都一样
    P_PORT.resetAll();
    P_PORTS.resetAll();

    /** write的时候两个有区别。如果是采用移位方式定义的连续pin，读写时不需要考虑
    	* 未标记Pin的影响。如果是用mask定义的，需要考虑未标记Pin的影响
    	*/
    P_PORT.write(0x01);  //PA0 = 1
    P_PORTS.write(0x01); //PA1 = 1
    // 两个不一样了
    P_PORT.write(0x03);  //PA0=1
    P_PORTS.write(0x03); //PA1 = 1, PA2 = 1

    /** read的时候同样有区别。如果是采用移位方式定义的连续pin，读写时不需要考虑
      	* 未标记Pin的影响。如果是用mask定义的，需要考虑未标记Pin的影响
      	*/
    P_PORT.setAll();
    P_PORTS.setAll();
    r = P_PORT.read();  // r = 0x09
    s = P_PORTS.read(); // s = 0x03
    r = s|r;
    P_PORT = 0;//PA3 = 0, PA0 = 0
}

void operationBus(){
    uint16_t r;
    r = P_BUS.read();
    P_BUS[0].set(); // PB0 = 1
    r = P_BUS.read();
    P_BUS.resetAll(); // PB_0,PB_3,PA_6,PA_7 = 0
    P_BUS = 0x02; // pb3 =1
    r = P_BUS;
    P_BUS.write(0x0f); //    PB_0,PB_3,PA_6,PA_7 = 1
    P_BUS = r;
}

int main(void)
{
    setup();

    while (1)
    {
        operationBus();
        operationPort();
        delay_ms(500);
        PA5.toggle();
    }
}




