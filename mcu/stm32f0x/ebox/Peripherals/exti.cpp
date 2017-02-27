/**
  ******************************************************************************
  * @file    exti.cpp
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
 * -LQM (2016/9/6)
 *      *��ֲ��STM32F0,����HAL��LL��
 */

/* Includes ------------------------------------------------------------------*/
#include "exti.h"

/** @defgroup exti 
  * @brief exti driver modules
  * @{
  */

/**
  * @}
  */
  
  /** @defgroup analog˽�к궨��
  * @{
  */  
  
/**
  * @}
  */
/** @defgroup extiȫ�ֱ���
  * @{
  */
static exti_irq_handler irq_handler;
static uint32_t exti_irq_ids[16];

int exti_irq_init(uint8_t index,exti_irq_handler handler,uint32_t id)
{
 exti_irq_ids[index] = id;
 irq_handler =  handler;
 return 0;
}  
/**
  * @}
  */



/**
 * @brief    Exti���캯����ʵ����һ������
 * @param    *pin: ָ�����ⲿ�ж�����PA0~PG15
 * @param    trigger: ���Ŵ����е�������������һ������ģʽ�е�һ�֣�
 *           - EXTI_Trigger_Rising: �����ش����ж�
 *           - EXTI_Trigger_Falling: �½��ش����ж�
 *           - EXTI_Trigger_Rising_Falling: �����غ��½��ؾ������ж�
 * @return   NONE
 */
Exti::Exti(Gpio *pin, uint8_t trigger)
{
    this->exti_pin = pin;
    this->trigger = trigger;
}

/**
 * @brief   ��ʼ���ⲿ�ж����ŵ�������Ϣ��
 * @param   NONE
 *          
 * @return  NONE
 * @note    ��ʼ����Ĭ�Ͽ����жϣ�����û�������жϣ�
 *          ���Ե���interrupt(DISABLE)�ر��жϡ�
 */ 
void Exti::begin()
{	
    init_info(exti_pin);
    //exti_irq_init(this->pin_source,(&Exti::_irq_handler),(uint32_t)this);
    exti_irq_init(exti_pin->id&0x0f,(&Exti::_irq_handler),(uint32_t)this);

    exti_pin->mode(INPUT);

    //RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
	 LL_APB1_GRP2_EnableClock(LL_APB1_GRP2_PERIPH_SYSCFG);

    // EXTI_InitTypeDef EXTI_InitStructure;
    // /* EXTI line(PB0) mode config */
    // GPIO_EXTILineConfig(port_source, pin_source);
    // EXTI_InitStructure.EXTI_Line = exti_line;
    // EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
    // EXTI_InitStructure.EXTI_Trigger = trigger; //�½����ж�
    // EXTI_InitStructure.EXTI_LineCmd = ENABLE;
    // EXTI_Init(&EXTI_InitStructure);
	LL_SYSCFG_SetEXTISource(port_source, pin_source);
	LL_EXTI_EnableIT_0_31(exti_line);

	switch (trigger)
	{
		case LL_EXTI_TRIGGER_FALLING:
			LL_EXTI_EnableFallingTrig_0_31(exti_line);
			break;
		case LL_EXTI_TRIGGER_RISING:
			LL_EXTI_EnableRisingTrig_0_31(exti_line);
			break;
		case LL_EXTI_TRIGGER_RISING_FALLING:
			LL_EXTI_EnableRisingTrig_0_31(exti_line);
			LL_EXTI_EnableFallingTrig_0_31(exti_line);
			break;
	default:
		break;
	}
	
    interrupt(ENABLE);

}

/**
 * @brief   �ⲿ�ж����ŵ��ж�������ֹ���ƺ���
 * @param   enable: ������߽�ֹ�ж�
 *          - ENABLE: ������ⲿ�ж�   
 *          - DISABLE: ��ֹ���ⲿ�ж�   
 *          
 * @return  NONE
 */ 
void Exti::interrupt(FunctionalState enable)
{
	  /*-2- Configure NVIC for EXTI4_15_IRQn */
  NVIC_EnableIRQ(irq);
  NVIC_SetPriority(irq,0);

}

/**
 * @brief   ��ʼ����������Ӧ���жϿ���������
 * @param   *pin: �ⲿ�ж�����Ӧ������PA0~PG15
 *          
 * @return  NONE
 */ 
void Exti::init_info(Gpio *pin)
{
    // switch((uint32_t)pin->id>>8)
    // {
    // case (uint32_t)0:
        // port_source = LL_SYSCFG_EXTI_PORTA;
        // break;
    // case (uint32_t)1:
        // port_source = GPIO_PortSourceGPIOB;
        // break;
    // case (uint32_t)2:
        // port_source = GPIO_PortSourceGPIOC;
        // break;
    // case (uint32_t)3:
        // port_source = GPIO_PortSourceGPIOD;
        // break;
    // case (uint32_t)4:
        // port_source = GPIO_PortSourceGPIOE;
        // break;
    // case (uint32_t)5:
        // port_source = GPIO_PortSourceGPIOF;
        // break;
    // }
	
	port_source = (uint32_t)pin->id>>8;
	
    switch(pin->id&0x0f)
    {
    case 0:
        pin_source = LL_SYSCFG_EXTI_LINE0;
        exti_line = LL_EXTI_LINE_0;
        irq = EXTI0_1_IRQn;
        break;

    case 1:
        pin_source = LL_SYSCFG_EXTI_LINE1;
        exti_line = LL_EXTI_LINE_1;
        irq = EXTI0_1_IRQn;
        break;

    case 2:
        pin_source = LL_SYSCFG_EXTI_LINE2;
        exti_line = LL_EXTI_LINE_2;
        irq = EXTI2_3_IRQn;
        break;

    case 3:
        pin_source = LL_SYSCFG_EXTI_LINE3;
        exti_line = LL_EXTI_LINE_3;
        irq = EXTI2_3_IRQn;
        break;

    case 4:
        pin_source = LL_SYSCFG_EXTI_LINE4;
        exti_line = LL_EXTI_LINE_4;
        irq = EXTI4_15_IRQn;
        break;

    case 5:
        pin_source = LL_SYSCFG_EXTI_LINE5;
        exti_line = LL_EXTI_LINE_5;
        irq = EXTI4_15_IRQn;
        break;

    case 6:
        pin_source = LL_SYSCFG_EXTI_LINE6;
        exti_line = LL_EXTI_LINE_6;
        irq = EXTI4_15_IRQn;
        break;

    case 7:
        pin_source = LL_SYSCFG_EXTI_LINE7;
        exti_line = LL_EXTI_LINE_7;
        irq = EXTI4_15_IRQn;
        break;

    case 8:
        pin_source = LL_SYSCFG_EXTI_LINE8;
        exti_line = LL_EXTI_LINE_8;
        irq = EXTI4_15_IRQn;
        break;

    case 9:
        pin_source = LL_SYSCFG_EXTI_LINE9;
        exti_line = LL_EXTI_LINE_9;
        irq = EXTI4_15_IRQn;
        break;

    case 10:
        pin_source = LL_SYSCFG_EXTI_LINE10;
        exti_line = LL_EXTI_LINE_10;
        irq = EXTI4_15_IRQn;
        break;

    case 11:
        pin_source = LL_SYSCFG_EXTI_LINE11;
        exti_line = LL_EXTI_LINE_11;
        irq = EXTI4_15_IRQn;
        break;

    case 12:
        pin_source = LL_SYSCFG_EXTI_LINE12;
        exti_line = LL_EXTI_LINE_12;
        irq = EXTI4_15_IRQn;
        break;

    case 13:
        pin_source = LL_SYSCFG_EXTI_LINE13;
        exti_line = LL_EXTI_LINE_13;
        irq = EXTI4_15_IRQn;
        break;

    case 14:
        pin_source = LL_SYSCFG_EXTI_LINE14;
        exti_line = LL_EXTI_LINE_14;
        irq = EXTI4_15_IRQn;
        break;

    case 15:
        pin_source = LL_SYSCFG_EXTI_LINE15;
        exti_line = LL_EXTI_LINE_15;
        irq = EXTI4_15_IRQn;
        break;

    }

}

void Exti::_irq_handler( uint32_t id)
{ 
    Exti *handler = (Exti*)id;
    handler->_irq.call();
}
/**
 * @brief   ���жϵĻص������������жϺ����ִ��callback_fun()
 * @param   void (*fptr)(void)���ͺ�����ָ�롣
 *          
 * @return  NONE
 */ 
void Exti::attach(void (*fptr)(void)) 
{
    if (fptr)
    {
        _irq.attach(fptr);
    }
}


extern "C" {
	
	void EXTI0_1_IRQHandler(void)
	{
		if (LL_EXTI_IsActiveFlag_0_31(LL_EXTI_LINE_0) != RESET)
		{
			LL_EXTI_ClearFlag_0_31(LL_EXTI_LINE_0);
			/* Manage code in main.c.*/
			irq_handler(exti_irq_ids[0]);
		}
		if (LL_EXTI_IsActiveFlag_0_31(LL_EXTI_LINE_1) != RESET)
		{
			LL_EXTI_ClearFlag_0_31(LL_EXTI_LINE_1);
			/* Manage code in main.c.*/
			irq_handler(exti_irq_ids[1]);
		}
	}
	
	void EXTI2_3_IRQHandler(void)
	{
		if (LL_EXTI_IsActiveFlag_0_31(LL_EXTI_LINE_2) != RESET)
		{
			LL_EXTI_ClearFlag_0_31(LL_EXTI_LINE_2);
			/* Manage code in main.c.*/
			irq_handler(exti_irq_ids[2]);
		}
		if (LL_EXTI_IsActiveFlag_0_31(LL_EXTI_LINE_3) != RESET)
		{
			LL_EXTI_ClearFlag_0_31(LL_EXTI_LINE_3);
			/* Manage code in main.c.*/
			irq_handler(exti_irq_ids[3]);
		}	
	}
	
	void EXTI4_15_IRQHandler(void)
	{
		if (LL_EXTI_IsActiveFlag_0_31(LL_EXTI_LINE_4) != RESET)
		{
			LL_EXTI_ClearFlag_0_31(LL_EXTI_LINE_4);
			/* Manage code in main.c.*/
			irq_handler(exti_irq_ids[4]);
		}
		if (LL_EXTI_IsActiveFlag_0_31(LL_EXTI_LINE_5) != RESET)
		{
			LL_EXTI_ClearFlag_0_31(LL_EXTI_LINE_5);
			/* Manage code in main.c.*/
			irq_handler(exti_irq_ids[5]);
		}
		if (LL_EXTI_IsActiveFlag_0_31(LL_EXTI_LINE_6) != RESET)
		{
			LL_EXTI_ClearFlag_0_31(LL_EXTI_LINE_6);
			/* Manage code in main.c.*/
			irq_handler(exti_irq_ids[6]);
		}
		if (LL_EXTI_IsActiveFlag_0_31(LL_EXTI_LINE_7) != RESET)
		{
			LL_EXTI_ClearFlag_0_31(LL_EXTI_LINE_7);
			/* Manage code in main.c.*/
			irq_handler(exti_irq_ids[7]);
		}
		if (LL_EXTI_IsActiveFlag_0_31(LL_EXTI_LINE_8) != RESET)
		{
			LL_EXTI_ClearFlag_0_31(LL_EXTI_LINE_8);
			/* Manage code in main.c.*/
			irq_handler(exti_irq_ids[8]);
		}
		if (LL_EXTI_IsActiveFlag_0_31(LL_EXTI_LINE_9) != RESET)
		{
			LL_EXTI_ClearFlag_0_31(LL_EXTI_LINE_9);
			/* Manage code in main.c.*/
			irq_handler(exti_irq_ids[9]);
		}	
		if (LL_EXTI_IsActiveFlag_0_31(LL_EXTI_LINE_10) != RESET)
		{
			LL_EXTI_ClearFlag_0_31(LL_EXTI_LINE_10);
			/* Manage code in main.c.*/
			irq_handler(exti_irq_ids[10]);
		}
		if (LL_EXTI_IsActiveFlag_0_31(LL_EXTI_LINE_11) != RESET)
		{
			LL_EXTI_ClearFlag_0_31(LL_EXTI_LINE_11);
			/* Manage code in main.c.*/
			irq_handler(exti_irq_ids[11]);
		}
		if (LL_EXTI_IsActiveFlag_0_31(LL_EXTI_LINE_12) != RESET)
		{
			LL_EXTI_ClearFlag_0_31(LL_EXTI_LINE_12);
			/* Manage code in main.c.*/
			irq_handler(exti_irq_ids[12]);
		}
		if (LL_EXTI_IsActiveFlag_0_31(LL_EXTI_LINE_13) != RESET)
		{
			LL_EXTI_ClearFlag_0_31(LL_EXTI_LINE_13);
			/* Manage code in main.c.*/
			irq_handler(exti_irq_ids[13]);
		}
		if (LL_EXTI_IsActiveFlag_0_31(LL_EXTI_LINE_14) != RESET)
		{
			LL_EXTI_ClearFlag_0_31(LL_EXTI_LINE_14);
			/* Manage code in main.c.*/
			irq_handler(exti_irq_ids[14]);
		}
		if (LL_EXTI_IsActiveFlag_0_31(LL_EXTI_LINE_15) != RESET)
		{
			LL_EXTI_ClearFlag_0_31(LL_EXTI_LINE_15);
			/* Manage code in main.c.*/
			irq_handler(exti_irq_ids[15]);
		}
	}
}
