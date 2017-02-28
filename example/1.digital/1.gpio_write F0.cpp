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
 * 1  ����GPIO��������E_GPIO, ����ΪPIN_ID; ������PA_5��ʽ
 * 2	����GPIOģʽ����Ϊͨ��GPIOʹ��ʱ��������af���ܡ�����������stm32_define.h�У��ο�ö�ٱ��� PIN_MODE
 * 3  ��д����
*/

// ���嵥���˿�
E_GPIO PA5(PA_5);
// ����ͬһ��port�����ɲ������˿�, PA0,PA3��������ʹ�����ַ�ʽ
E_PORT P_PORT(E_PORTA,0x09);
//����ͬһ��port�����������˿�, PA1-PA2������ʹ�����ַ�ʽ
E_PORT P_PORTS(E_PORTA,2,1);
// ����ͬport���ɶ˿ڴ������Ϊһ��������в���;�ʺϲ�ͬport��IO��ͬһport��IO���ʹ��
// ����������� IO˳��
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
    // ��һ��������һ��
    P_PORT.resetAll();
    P_PORTS.resetAll();

    /** write��ʱ����������������ǲ�����λ��ʽ���������pin����дʱ����Ҫ����
    	* δ���Pin��Ӱ�졣�������mask����ģ���Ҫ����δ���Pin��Ӱ��
    	*/
    P_PORT.write(0x01);  //PA0 = 1
    P_PORTS.write(0x01); //PA1 = 1
    // ������һ����
    P_PORT.write(0x03);  //PA0=1
    P_PORTS.write(0x03); //PA1 = 1, PA2 = 1

    /** read��ʱ��ͬ������������ǲ�����λ��ʽ���������pin����дʱ����Ҫ����
      	* δ���Pin��Ӱ�졣�������mask����ģ���Ҫ����δ���Pin��Ӱ��
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




