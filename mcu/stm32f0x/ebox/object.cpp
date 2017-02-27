/**
  ******************************************************************************
  * @file    object.cpp
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


/* Includes ------------------------------------------------------------------*/
#include "ebox.h"


Gpio PA0(GPIOA, LL_GPIO_PIN_0);
Gpio PA1(GPIOA, LL_GPIO_PIN_1);
Gpio PA2(GPIOA, LL_GPIO_PIN_2);
Gpio PA3(GPIOA, LL_GPIO_PIN_3);
Gpio PA4(GPIOA, LL_GPIO_PIN_4);
Gpio PA5(GPIOA, LL_GPIO_PIN_5);
Gpio PA6(GPIOA, LL_GPIO_PIN_6);
Gpio PA7(GPIOA, LL_GPIO_PIN_7);
Gpio PA8(GPIOA, LL_GPIO_PIN_8);
Gpio PA9(GPIOA, LL_GPIO_PIN_9);
Gpio PA10(GPIOA, LL_GPIO_PIN_10);
Gpio PA11(GPIOA, LL_GPIO_PIN_11);
Gpio PA12(GPIOA, LL_GPIO_PIN_12);
Gpio PA13(GPIOA, LL_GPIO_PIN_13);
Gpio PA14(GPIOA, LL_GPIO_PIN_14);
Gpio PA15(GPIOA, LL_GPIO_PIN_15);

Gpio PB0(GPIOB, LL_GPIO_PIN_0);
Gpio PB1(GPIOB, LL_GPIO_PIN_1);
Gpio PB2(GPIOB, LL_GPIO_PIN_2);
Gpio PB3(GPIOB, LL_GPIO_PIN_3);
Gpio PB4(GPIOB, LL_GPIO_PIN_4);
Gpio PB5(GPIOB, LL_GPIO_PIN_5);
Gpio PB6(GPIOB, LL_GPIO_PIN_6);
Gpio PB7(GPIOB, LL_GPIO_PIN_7);
Gpio PB8(GPIOB, LL_GPIO_PIN_8);
Gpio PB9(GPIOB, LL_GPIO_PIN_9);
Gpio PB10(GPIOB, LL_GPIO_PIN_10);
Gpio PB11(GPIOB, LL_GPIO_PIN_11);
Gpio PB12(GPIOB, LL_GPIO_PIN_12);
Gpio PB13(GPIOB, LL_GPIO_PIN_13);
Gpio PB14(GPIOB, LL_GPIO_PIN_14);
Gpio PB15(GPIOB, LL_GPIO_PIN_15);


//Gpio PC0(GPIOC, LL_GPIO_PIN_0);
//Gpio PC1(GPIOC, LL_GPIO_PIN_1);
//Gpio PC2(GPIOC, LL_GPIO_PIN_2);
//Gpio PC3(GPIOC, LL_GPIO_PIN_3);
//Gpio PC4(GPIOC, LL_GPIO_PIN_4);
//Gpio PC5(GPIOC, LL_GPIO_PIN_5);
//Gpio PC6(GPIOC, LL_GPIO_PIN_6);
//Gpio PC7(GPIOC, LL_GPIO_PIN_7);
//Gpio PC8(GPIOC, LL_GPIO_PIN_8);
//Gpio PC9(GPIOC, LL_GPIO_PIN_9);
//Gpio PC10(GPIOC, LL_GPIO_PIN_10);
//Gpio PC11(GPIOC, LL_GPIO_PIN_11);
//Gpio PC12(GPIOC, LL_GPIO_PIN_12);
//Gpio PC13(GPIOC, LL_GPIO_PIN_13);
//Gpio PC14(GPIOC, LL_GPIO_PIN_14);
//Gpio PC15(GPIOC, LL_GPIO_PIN_15);


//Gpio PD0(GPIOD, LL_GPIO_PIN_0);
//Gpio PD1(GPIOD, LL_GPIO_PIN_1);
//Gpio PD2(GPIOD, LL_GPIO_PIN_2);
//Gpio PD3(GPIOD, LL_GPIO_PIN_3);
//Gpio PD4(GPIOD, LL_GPIO_PIN_4);
//Gpio PD5(GPIOD, LL_GPIO_PIN_5);
//Gpio PD6(GPIOD, LL_GPIO_PIN_6);
//Gpio PD7(GPIOD, LL_GPIO_PIN_7);
//Gpio PD8(GPIOD, LL_GPIO_PIN_8);
//Gpio PD9(GPIOD, LL_GPIO_PIN_9);
//Gpio PD10(GPIOD, LL_GPIO_PIN_10);
//Gpio PD11(GPIOD, LL_GPIO_PIN_11);
//Gpio PD12(GPIOD, LL_GPIO_PIN_12);
//Gpio PD13(GPIOD, LL_GPIO_PIN_13);
//Gpio PD14(GPIOD, LL_GPIO_PIN_14);
//Gpio PD15(GPIOD, LL_GPIO_PIN_15);

//Gpio PE0(GPIOE, LL_GPIO_PIN_0);
//Gpio PE1(GPIOE, LL_GPIO_PIN_1);
//Gpio PE2(GPIOE, LL_GPIO_PIN_2);
//Gpio PE3(GPIOE, LL_GPIO_PIN_3);
//Gpio PE4(GPIOE, LL_GPIO_PIN_4);
//Gpio PE5(GPIOE, LL_GPIO_PIN_5);
//Gpio PE6(GPIOE, LL_GPIO_PIN_6);
//Gpio PE7(GPIOE, LL_GPIO_PIN_7);
//Gpio PE8(GPIOE, LL_GPIO_PIN_8);
//Gpio PE9(GPIOE, LL_GPIO_PIN_9);
//Gpio PE10(GPIOE, LL_GPIO_PIN_10);
//Gpio PE11(GPIOE, LL_GPIO_PIN_11);
//Gpio PE12(GPIOE, LL_GPIO_PIN_12);
//Gpio PE13(GPIOE, LL_GPIO_PIN_13);
//Gpio PE14(GPIOE, LL_GPIO_PIN_14);
//Gpio PE15(GPIOE, LL_GPIO_PIN_15);

//Gpio PF0(GPIOF, LL_GPIO_PIN_0);
//Gpio PF1(GPIOF, LL_GPIO_PIN_1);
//Gpio PF2(GPIOF, LL_GPIO_PIN_2);
//Gpio PF3(GPIOF, LL_GPIO_PIN_3);
//Gpio PF4(GPIOF, LL_GPIO_PIN_4);
//Gpio PF5(GPIOF, LL_GPIO_PIN_5);
//Gpio PF6(GPIOF, LL_GPIO_PIN_6);
//Gpio PF7(GPIOF, LL_GPIO_PIN_7);
//Gpio PF8(GPIOF, LL_GPIO_PIN_8);
//Gpio PF9(GPIOF, LL_GPIO_PIN_9);
//Gpio PF10(GPIOF, LL_GPIO_PIN_10);
//Gpio PF11(GPIOF, LL_GPIO_PIN_11);
//Gpio PF12(GPIOF, LL_GPIO_PIN_12);
//Gpio PF13(GPIOF, LL_GPIO_PIN_13);
//Gpio PF14(GPIOF, LL_GPIO_PIN_14);
//Gpio PF15(GPIOF, LL_GPIO_PIN_15);

//Gpio PG0(GPIOG, LL_GPIO_PIN_0);
//Gpio PG1(GPIOG, LL_GPIO_PIN_1);
//Gpio PG2(GPIOG, LL_GPIO_PIN_2);
//Gpio PG3(GPIOG, LL_GPIO_PIN_3);
//Gpio PG4(GPIOG, LL_GPIO_PIN_4);
//Gpio PG5(GPIOG, LL_GPIO_PIN_5);
//Gpio PG6(GPIOG, LL_GPIO_PIN_6);
//Gpio PG7(GPIOG, LL_GPIO_PIN_7);
//Gpio PG8(GPIOG, LL_GPIO_PIN_8);
//Gpio PG9(GPIOG, LL_GPIO_PIN_9);
//Gpio PG10(GPIOG, LL_GPIO_PIN_10);
//Gpio PG11(GPIOG, LL_GPIO_PIN_11);
//Gpio PG12(GPIOG, LL_GPIO_PIN_12);
//Gpio PG13(GPIOG, LL_GPIO_PIN_13);
//Gpio PG14(GPIOG, LL_GPIO_PIN_14);
//Gpio PG15(GPIOG, LL_GPIO_PIN_15);

//´®¿Ú
#ifdef STM32F072xB
//Uart uart1(USART2, &PA2, &PA3);
Uart uart1(USART1, &PA9, &PA10);
#else
Uart uart1(USART1, &PA9, &PA10);
#endif
//Uart uart2(USART2, &PA2, &PA3);
//Uart uart3(USART3, &PB10, &PB11);
//Uart uart4(UART4, &PC10, &PC11);
//Uart uart5(UART5, &PC12, &PD2);

////spi
Spi     spi1(SPI1, &PA5, &PA6, &PA7);
//Spi     spi2(SPI2, &PB13, &PB14, &PB15);
//Spi     spi3(SPI3, &PB3, &PB4, &PB5);
SoftSpi	sspi1(&PA5, &PA6, &PA7);
//SoftSpi	sspi1(&PA4, &PA3, &PA2);

////i2c
//#ifdef STM32F030x6
////I2c     i2c1(I2C1, &PA9, &PA10);
//#else
////I2c     i2c1(I2C1, &PB6, &PB7);
//#endif
//I2c     i2c2(I2C2, &PB10, &PB11);
//I2c 	es2c(I2C2,&PB13,&PB14);
//SoftI2c si2c(&PA4, &PA5);
//SoftI2c si2c1(&PB6, &PB7);
//SoftI2c si2c2(&PB10, &PB11);

//can
//CAN can1(CAN1, &PA11, &PA12);
