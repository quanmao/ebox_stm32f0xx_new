/**
  ******************************************************************************
  * @file    ebox_core.c
  * @author  cat_li
  * @version V2.0
  * @date    2016/10/21
  * @brief
  *     2017��3��4��  add Systick�жϻص��ӿ�
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
#include "common.h"
#include "stdio.h"
#define systick_no_interrupt()  SysTick->CTRL &=0xfffffffd
#define systick_interrupt()     SysTick->CTRL |=0x0002

// ��������жϵĴ���,��ebox_define�е���
__IO uint8_t irqCount = 0;
cpu_t cpu;

#ifdef __cplusplus
extern "C" {
#endif

    // systick �жϻص�����ָ�룬
    fun_noPara_t  callBackFun;
    uint16_t _multiple = 1;
    
    __IO uint64_t millis_seconds;//�ṩһ��mills()��Ч��ȫ�ֱ���������cpu���ÿ���
    static uint8_t count;

    void nullFun(void){}

    /**
     *@brief    ע���жϻص�������ÿ���жϾ����ã�Ĭ������Ϊ1ms
     *@param    SystickCallBack_T fun �ص��������޲Σ�
     *@retval   E_OK  ע��ɹ��� E_NG ע��ʧ��
    */
    uint8_t SystickCallBackRegister(fun_noPara_t fun){
        if (callBackFun == NULL || callBackFun == nullFun)
        {
            callBackFun = fun;
            return E_OK;
        }else{
            return E_NG;
        }
    }

    /**
     *@brief    ע���жϻص�����������ָ���������� = �ж����� * multiple
     *@param    SystickCallBack_T fun �ص��������޲Σ�, uint8_t multiple �����������趨��������
     *@retval   E_OK  ע��ɹ��� E_NG ע��ʧ��
    */
    uint8_t SystickCallBackRegister1(fun_noPara_t fun,uint16_t multiple){

        if (callBackFun == NULL || callBackFun == nullFun)
        {
            callBackFun = fun;
            _multiple = multiple == 0 ? 1 : multiple;
            return E_OK;
        }else{
            return E_NG;
        }
    }

    /**
     *@brief    ����ϵͳʱ�ӣ��������������û��ڱ�ʵ�֣��������޸�ʱ��
     *@param    none
     *@retval   none
    */
    __weak void SystemClock_Config(void)
    {
        /* Configuration will allow to reach a SYSCLK frequency set to 24MHz:
         Syst freq = ((HSI_VALUE * PLLMUL)/ PLLDIV)
        						 ((8MHz * 12)/ 4)                  = 24MHz             */
        LL_UTILS_PLLInitTypeDef sUTILS_PLLInitStruct = {LL_RCC_PLL_MUL_12 , LL_RCC_PREDIV_DIV_2};
        /* Variable to store AHB and APB buses clock configuration */
        /* Settings to have HCLK set to 12MHz and APB to 6 MHz */
        LL_UTILS_ClkInitTypeDef sUTILS_ClkInitStruct = {LL_RCC_SYSCLK_DIV_1, LL_RCC_APB1_DIV_1};

        /* Switch to PLL with HSI as clock source             */
        LL_PLL_ConfigSystemClock_HSI(&sUTILS_PLLInitStruct, &sUTILS_ClkInitStruct);
    }

    //extern uint16_t  AD_value[];




    void ebox_init(void)
    {
        SystemClock_Config();

        get_system_clock(&cpu.clock);
        get_chip_info();
        /* ����ʱ��Դ������£�systick��ʱ��Ƶ��ΪHCLK, 1ms�жϼ�����ֵӦΪHCLK/1000
         * ���HCLKΪ8M����systick��С����ʱ��Ϊ1/8us,
         */
        SysTick_Config(cpu.clock.hclk/1000);//  ÿ�� 1ms����һ���ж�
        count = (uint8_t)(cpu.clock.hclk/1000000);
        //LL_SYSTICK_SetClkSource(LL_SYSTICK_CLKSOURCE_HCLK);//systemticks clock��
        SystickCallBackRegister(nullFun);

//        //ͳ��cpu��������//////////////////
//        cpu.ability = 0;
//        millis_seconds = 0;
//        do
//        {
//            cpu.ability++;//ͳ��cpu��������
//        }
//        while (millis_seconds < 100);
//        cpu.ability = cpu.ability * 10;
        ////////////////////////////////
//        ADC1_init();

//        NVIC_PriorityGroupConfig(NVIC_GROUP_CONFIG);

        //��pb4Ĭ������ΪIO�ڣ�����jtag
//        RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
//        GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);
//       set_systick_user_event_per_sec(1000);
//        random_seed(AD_value[0]);//��ʼ�����������
        //get_system_clock(&cpu.clock);

    }

    uint64_t micros(void)
    {
        //�����ʱ�����������ж��ҷ�����systick�������Ҫ��millis_secend���в���
        if ((SysTick->CTRL & (1 << 16)) && (__get_PRIMASK()))
        {
            millis_seconds++; //ms����
        }
        /* millis_seconds * 1000 ��msת��Ϊus
         * cpu.clock.hclk/1000 Systick reloadֵ��Systick���¼���������cpu.clock.hclk/1000 - SysTick->VAL = ����ֵ
         * ����ֵ /(cpu.clock.hclk/1000000) = us
         * ����Ϊ millis_seconds *1000 + ((cpu.clock.hclk/1000 - SysTick->VAL)/(cpu.clock.hclk/1000000)) =
         * (millis_seconds * 1000 + (1000 - (SysTick->VAL / cpu.clock.hclk)*1000000))
         */
        return  (millis_seconds * 1000 + (1000 - (SysTick->VAL / SysTick->LOAD)*1000));
    }

    uint64_t millis( void )
    {
        return millis_seconds;
    }

    void delay_ms(uint64_t ms)
    {
        uint64_t end ;
        end = micros() + ms * 1000 - 3;
        while (micros() < end);
    }

//    void delay_us(uint16_t us)
//    {
//        uint64_t end = micros() + us - 3;
//        while(micros() < end);
//    }

    void  delay_us(uint16_t us)
    {
        uint32_t ticks;
        uint32_t told,tnow,tcnt=0;

        ticks = (us-1) *count;             /* �������� */
        tcnt = 0;
        told = SysTick->VAL;               /* ���浱ǰ����ֵ */

        while (1)
        {
            tnow = SysTick->VAL;
            tcnt += (tnow < told)? (told-tnow):(SysTick->LOAD - tnow + told);
            if (tcnt >= ticks)break;
            told = tnow;
        }
    }

//    callback_fun_type systick_cb_table[1] = {0};
//    __IO uint16_t systick_user_event_per_sec;//��ʵ��ֵ
//    __IO uint16_t _systick_user_event_per_sec;//���ڱ�millis_secondȡ����

//    void set_systick_user_event_per_sec(uint16_t frq)
//    {
//        _systick_user_event_per_sec = 1000 / frq;
//        systick_user_event_per_sec = frq;
//    }

//    void attach_systick_user_event(void (*callback_fun)(void))
//    {
//        systick_cb_table[0] = callback_fun;
//    }


    void ebox_SysTick_Handler(void)//systick�ж�
    {
        if (millis_seconds++ % _multiple == 0)
        {
            callBackFun();
        }
    }
    /**
     *@brief    ��ȡϵͳʱ��
     *@param    *clock��  ʱ��ָ�룬����ϵͳʱ��
     *@retval   none
    */
    static void get_system_clock(cpu_clock_t *clock)
    {
        LL_RCC_ClocksTypeDef RCC_Clock;

        SystemCoreClockUpdate();
        LL_RCC_GetSystemClocksFreq(&RCC_Clock);

        clock->core = RCC_Clock.SYSCLK_Frequency;
        clock->hclk = RCC_Clock.HCLK_Frequency;
        clock->pclk1 = RCC_Clock.PCLK1_Frequency;
    }

    /**
     *@brief    ��ȡϵͳ��Ϣ��CPU_ID,flashsize
     *@param    *clock��  ʱ��ָ�룬����ϵͳʱ��
     *@retval   none
    */
    static void get_chip_info(void)
    {
//		cpu.chip_id[2] = LL_GetUID_Word0(); //���ֽ�
//		cpu.chip_id[1] = LL_GetUID_Word1(); //
//		cpu.chip_id[0] = LL_GetUID_Word2(); //���ֽ�
//		cpu.flash_size = LL_GetFlashSize();   //оƬflash����
    }

#ifdef __cplusplus
}


void eBox::sleep(void){
    LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_PWR);
    /* Set STOP_LPREGU mode when CPU enters deepsleep */
    LL_PWR_SetPowerMode(LL_PWR_MODE_STOP_MAINREGU);
    /* Set SLEEPDEEP bit of Cortex System Control Register */
    //LL_LPM_EnableDeepSleep();
    LL_LPM_EnableSleepOnExit();
    /* Request Wait For Interrupt */
    __WFI();
}

void eBox::stop(){
//	  LL_GPIO_InitTypeDef gpio_initstruct = {LL_GPIO_PIN_ALL, LL_GPIO_MODE_ANALOG,
//                                         LL_GPIO_SPEED_FREQ_HIGH, LL_GPIO_OUTPUT_PUSHPULL,
//                                         LL_GPIO_PULL_NO, LL_GPIO_AF_0};

//  /* Set all GPIO in analog state to reduce power consumption,                */
//  /* Note: Debug using ST-Link is not possible during the execution of this   */
//  /*       example because communication between ST-link and the device       */
//  /*       under test is done through UART. All GPIO pins are disabled (set   */
//  /*       to analog input mode) including  UART I/O pins.                    */
//  LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOA |
//                           LL_AHB1_GRP1_PERIPH_GPIOB |
//                           LL_AHB1_GRP1_PERIPH_GPIOC |
//                           LL_AHB1_GRP1_PERIPH_GPIOD |
//                           LL_AHB1_GRP1_PERIPH_GPIOE |
//                           LL_AHB1_GRP1_PERIPH_GPIOF);

//  LL_GPIO_Init(GPIOA, &gpio_initstruct);
//  LL_GPIO_Init(GPIOB, &gpio_initstruct);
//  LL_GPIO_Init(GPIOC, &gpio_initstruct);
//  LL_GPIO_Init(GPIOD, &gpio_initstruct);
//  LL_GPIO_Init(GPIOE, &gpio_initstruct);
//  LL_GPIO_Init(GPIOF, &gpio_initstruct);

//  LL_AHB1_GRP1_DisableClock(LL_AHB1_GRP1_PERIPH_GPIOA |
//                           LL_AHB1_GRP1_PERIPH_GPIOB |
//                           LL_AHB1_GRP1_PERIPH_GPIOC |
//                           LL_AHB1_GRP1_PERIPH_GPIOD |
//                           LL_AHB1_GRP1_PERIPH_GPIOE |
//                           LL_AHB1_GRP1_PERIPH_GPIOF);

    /** Request to enter STOP_LPREGU mode
      * Following procedure describe in STM32F0xx Reference Manual
      * See PWR part, section Low-power modes, STOP_LPREGU mode
      */
    /* Set STOP_LPREGU mode when CPU enters deepsleep */
    LL_PWR_SetPowerMode(LL_PWR_MODE_STOP_LPREGU);
    /* Set SLEEPDEEP bit of Cortex System Control Register */
    //LL_LPM_EnableDeepSleep();
    LL_LPM_EnableSleepOnExit();

    /* Request Wait For Interrupt */
    __WFI();
}

void eBox::standby(void){
    /* Disable all used wakeup sources */
    LL_PWR_DisableWakeUpPin(LL_PWR_WAKEUP_PIN1);

    /* Clear all wake up Flag */
    LL_PWR_ClearFlag_WU();

    /* Enable wakeup pin */
    LL_PWR_EnableWakeUpPin(LL_PWR_WAKEUP_PIN1);
    /* Set STANDBY mode when CPU enters deepsleep */
    LL_PWR_SetPowerMode(LL_PWR_MODE_STANDBY);
    /* Set SLEEPDEEP bit of Cortex System Control Register */
    //LL_LPM_EnableDeepSleep();
    LL_LPM_EnableSleepOnExit();
    /* Request Wait For Interrupt */
    __WFI();
}


#endif
