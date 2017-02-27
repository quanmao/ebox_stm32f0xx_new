/**
  ******************************************************************************
  * @file    pwm.h
  * @author  shentq
  * @version V1.2
  * @date    2016/08/14
  * @brief   
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
 * Modification History:
 * -shentq                  -version 1.2(2016/08/17)
 *      *修复了set_duty时导致波形输出畸形
 *      *修复了芯片主频变动导致定时器PWM频率变化的bug，支持任意主频下正常工作
 *      *增加了get_timer_clock(),get_max_frq(),get_accuracy()三个函数接口
 * -LQM (2016/9/1)
 *      *移植到STM32F0,基于HAL库LL层  最大输出频率为 TIM时钟频率/100
 *		*屏蔽TIM2,占用TIM3 1-4,TIM14CH1，使用时需要注意
 */
/* Define to prevent recursive inclusion -------------------------------------*/

#ifndef __PWM_H
#define __PWM_H

#include "common.h"
/*
	1.支持TIM2,3,4下的CH1-4通道。
	2.可以随时调用pwm的参数修改函数。调用setfrq函数不会改变占空比,但是会影响同定时器下其他通道的工作。
    3.pwm所占用的定时器的时钟源频率可以通过get_timer_clock()获得；
    4.PWM最大频率输出可以通过get_max_frq()获得；
    5.PWM当前输出的精度可以通过get_accuracy()获得；
	注意：1.暂时没有完美支持remap引脚。
					如果使用remap引脚需要自己在执行完初始化函数后再执行相应的remap函数。
				2.使用了相应的timx为pwm服务后，该tim就不能再初始化成另外的功能
				3.初始化pwm后，只有调用setDuty函数后才会输出pwm
				4.如果duty大于等于1000则输出持续高电平
*/

//PWMPin:有定时器oc通道的io
//period：定时器的周期。溢出值
//frq = 1-720k;在72k以内可以保证占空比精度为0.1%。超过72k后占空比精度为1%。
//pwm的占空比=duty/1000;（duty：0-1000）
class Pwm
{
public:
    Pwm(Gpio *pwm_pin);
    void begin(uint32_t frq, uint16_t duty);
    void set_frq(uint32_t frq);
    void set_duty(uint16_t duty);//保证以最快的速度分配完成
    void set_oc_polarity(uint8_t flag);

    uint32_t get_timer_source_clock();
    uint32_t get_max_frq();
    float   get_accuracy();

private:
    Gpio *pwm_pin;
    uint8_t af_configration; // 保存信息，使用AF0,1,2,3,4....

    TIM_TypeDef *TIMx;
    uint32_t    rcc;
    uint8_t     ch;
    uint32_t	TIM_Channel;
    uint16_t    period;//保存溢出值，用于计算占空比
    uint16_t    duty;//保存占空比值
    uint16_t    oc_polarity;
    uint8_t     accuracy;

    void init_info(Gpio *pwm_pin);
    void base_init(uint16_t Period, uint16_t Prescaler);
//  void _set_duty(uint16_t duty);

};
void analog_write(Gpio *pwm_pin, uint16_t duty);

#endif
