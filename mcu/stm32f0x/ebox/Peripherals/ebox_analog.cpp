/**
  ******************************************************************************
  * @file    ebox_analog.cpp
  * @author  cat_li
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
	
#include "ebox_analog.h"
#include "ebox_debug.h"

static uint16_t _refVoltage;	// ADC参考电压
static uint32_t _interChannel = LL_ADC_PATH_INTERNAL_NONE;

/**
 *@name     void ADC1_init(void)
 *@brief    ADC初始化
 *@param    NONE
 *@retval   NONE
*/
void ADC1_init(void)
{
 
  /* Enable ADC clock (core clock) */
  LL_APB1_GRP2_EnableClock(LL_APB1_GRP2_PERIPH_ADC1);
    
  // ADC没有启动,配置ADC时钟
  if (LL_ADC_IsEnabled(ADC1) == 0)
  {
    /* Set ADC clock (conversion clock) */
    LL_ADC_SetClock(ADC1, LL_ADC_CLOCK_SYNC_PCLK_DIV2);
    
    /* Set ADC data resolution */
    LL_ADC_SetResolution(ADC1, LL_ADC_RESOLUTION_12B);
    
    /* Set ADC conversion data alignment */
    LL_ADC_SetResolution(ADC1, LL_ADC_DATA_ALIGN_RIGHT);
    
    /* Set ADC low power mode */
    LL_ADC_SetLowPowerMode(ADC1, LL_ADC_LP_MODE_NONE);
    
    /* Set ADC channels sampling time */
    /* Note: On this STM32 serie, sampling time is common to all channels     */
    /*       of the entire ADC instance.                                      */
    /*       Therefore, sampling time is configured here under ADC instance   */
    /*       scope (not under channel scope as on some other STM32 devices    */
    /*       on which sampling time is channel wise).                         */
    LL_ADC_SetSamplingTimeCommonChannels(ADC1, LL_ADC_SAMPLINGTIME_41CYCLES_5);
    
  }
  
  
  /*## Configuration of ADC hierarchical scope: ADC group regular ############*/
  // 启动ADC
  if ((LL_ADC_IsEnabled(ADC1) == 0)||(LL_ADC_REG_IsConversionOngoing(ADC1) == 0)   )
  {
    /* Set ADC group regular trigger source */
    LL_ADC_REG_SetTriggerSource(ADC1, LL_ADC_REG_TRIG_SOFTWARE);    
    /* Set ADC group regular trigger polarity */
    // LL_ADC_REG_SetTriggerEdge(ADC1, LL_ADC_REG_TRIG_EXT_RISING);    
    /* Set ADC group regular continuous mode */
    LL_ADC_REG_SetContinuousMode(ADC1, LL_ADC_REG_CONV_SINGLE);    
    /* Set ADC group regular conversion data transfer */
    // LL_ADC_REG_SetDMATransfer(ADC1, LL_ADC_REG_DMA_TRANSFER_NONE);    
    /* Set ADC group regular overrun behavior */
    LL_ADC_REG_SetOverrun(ADC1, LL_ADC_REG_OVR_DATA_OVERWRITTEN);    
    /* Set ADC group regular sequencer */
  }
  
  /*## Configuration of ADC hierarchical scope: channels #####################*/
  
  /* Note: Hardware constraint (refer to description of the functions         */
  /*       below):                                                            */
  /*       On this STM32 serie, setting of these features is conditioned to   */
  /*       ADC state:                                                         */
  /*       ADC must be disabled or enabled without conversion on going        */
  /*       on either groups regular or injected.                              */
  if ((LL_ADC_IsEnabled(ADC1) == 0)||(LL_ADC_REG_IsConversionOngoing(ADC1) == 0)   )
  {
    /* Set ADC channels sampling time */
    /* Note: On this STM32 serie, sampling time is common to all channels     */
    /*       of the entire ADC instance.                                      */
    /*       See sampling time configured above, at ADC instance scope.       */
    
  }

}

/**
 *@name     uint16_t analog_read(uint32_t *channel)
 *@brief    读取ADC1某个引脚上的模拟转换结果
 *@param    channel：ADC1 通道
 *@retval   如果引脚正确则返回该引脚的模拟电压值所对应的12bit的ADC转换结果
            如果引脚错误返回0；
*/
uint16_t E_ADC::read(void)
{
//	//LL_DMA_DisableChannel(DMA1,LL_DMA_CHANNEL_1);
//	LL_ADC_REG_SetDMATransfer(ADC1,LL_ADC_REG_DMA_TRANSFER_NONE);
//	LL_ADC_REG_SetContinuousMode(ADC1, LL_ADC_REG_CONV_SINGLE);
//	LL_ADC_REG_SetSequencerChannels(ADC1, *channel);

	if (LL_ADC_IsEnabled(ADC1) == 0)
	{
		/* Run ADC self calibration */
		LL_ADC_StartCalibration(ADC1);

		/* Poll for ADC effectively calibrated */
//    #if (USE_TIMEOUT == 1)
//    Timeout = ADC_CALIBRATION_TIMEOUT_MS;
//    #endif /* USE_TIMEOUT */

		while (LL_ADC_IsCalibrationOnGoing(ADC1) != 0)
		{
//    #if (USE_TIMEOUT == 1)
//      /* Check Systick counter flag to decrement the time-out value */
//      if (LL_SYSTICK_IsActiveCounterFlag())
//      {
//        if(Timeout-- == 0)
//        {
//        /* Time-out occurred. Set LED to blinking mode */
//        LED_Blinking(LED_BLINK_ERROR);
//        }
//      }
//    #endif /* USE_TIMEOUT */
		}



		/* Enable ADC */
		LL_ADC_Enable(ADC1);

		/* Poll for ADC ready to convert */
//    #if (USE_TIMEOUT == 1)
//    Timeout = ADC_ENABLE_TIMEOUT_MS;
//    #endif /* USE_TIMEOUT */

		while (LL_ADC_IsActiveFlag_ADRDY(ADC1) == 0)
		{
//    #if (USE_TIMEOUT == 1)
//      /* Check Systick counter flag to decrement the time-out value */
//      if (LL_SYSTICK_IsActiveCounterFlag())
//      {
//        if(Timeout-- == 0)
//        {
//        /* Time-out occurred. Set LED to blinking mode */
//        LED_Blinking(LED_BLINK_ERROR);
//        }
//      }
//    #endif /* USE_TIMEOUT */
		}}

	LL_ADC_REG_SetSequencerChannels(ADC1, _channel);
	LL_ADC_REG_StartConversion(ADC1);
	
	while (!LL_ADC_IsActiveFlag_EOC(ADC1));
	return LL_ADC_REG_ReadConversionData12(ADC1);
}

/**
 *@brief    读取当前通道的模拟电压
 *@param    none
 *@retval   如果引脚正确则返回该引脚的模拟电压值所对应的模拟电压,单位mv
*/
uint16_t E_ADC::getVoltage(){
#if USE_INTERVREF
	uint16_t iRef = 0;
	uint32_t ch = _channel;	//保存当前通道
	// 读取参考电压
	_channel = LL_ADC_CHANNEL_VREFINT;
	iRef = __LL_ADC_CALC_VREFANALOG_VOLTAGE(read(),LL_ADC_RESOLUTION_12B);
	DBG("VrefInt is : %d  | ",iRef);
	// 读取当前通道电压
	_channel = ch;
	return __LL_ADC_CALC_DATA_TO_VOLTAGE(iRef,read(),LL_ADC_RESOLUTION_12B);
#else
	return __LL_ADC_CALC_DATA_TO_VOLTAGE(_refVoltage,read(),LL_ADC_RESOLUTION_12B);
#endif
}

float E_ADC::getTemperature(){
#if USE_INTERVREF
	uint32_t iRef = 0;
	uint32_t ch = _channel;
	_channel = LL_ADC_CHANNEL_VREFINT;
	iRef = __LL_ADC_CALC_VREFANALOG_VOLTAGE(read(),LL_ADC_RESOLUTION_12B);
	DBG("temp refV is: %d  | ",iRef);
	_channel = ch;
	return __LL_ADC_CALC_TEMPERATURE(iRef,read(),LL_ADC_RESOLUTION_12B);
#else
	return __LL_ADC_CALC_TEMPERATURE(_refVoltage,read(),LL_ADC_RESOLUTION_12B);
#endif
}

/**
 *@brief    构造函数
 *@param    E_PinID id, Pin id
 *		   uint16_t refVoltage 参考电压，默认3300
 *@retval   如果引脚正确则返回该引脚的模拟电压值所对应的模拟电压，默认参考电压为3.3V
*/
E_ADC::E_ADC(E_PinID id,uint16_t refVoltage){
	uint8_t index;
	index = getIndex(id,ADC_MAP);
	_channel = ADC_MAP[index]._periph_OR_ch;
	_refVoltage = refVoltage;

	_setInterChannel();

	ADC1_init();
}

E_ADC::E_ADC(ADC_CH ch){
	switch ((uint8_t)ch){
	case 0:
		_channel = LL_ADC_CHANNEL_TEMPSENSOR;
		_interChannel |= LL_ADC_PATH_INTERNAL_TEMPSENSOR;
		break;
	case 1:
		_channel = LL_ADC_CHANNEL_VBAT;
		_interChannel |= LL_ADC_PATH_INTERNAL_VBAT;
	 	break;
	}
	_setInterChannel();
	ADC1_init();
}

void E_ADC::setRefVoltage(uint16_t refVoltage){
	_refVoltage = refVoltage;
}

void E_ADC::_setInterChannel(void){
	  if(__LL_ADC_IS_ENABLED_ALL_COMMON_INSTANCE() == 0)
  {
    /* Note: Call of the functions below are commented because they are       */
    /*       useless in this example:                                         */
    /*       setting corresponding to default configuration from reset state. */
    
    /* Set ADC clock (conversion clock) common to several ADC instances */
    /* Note: On this STM32 serie, ADC common clock asynchonous prescaler      */
    /*       is applied to each ADC instance if ADC instance clock is         */
    /*       set to clock source asynchronous                                 */
    /*       (refer to function "LL_ADC_SetClock()" below).                   */
    // LL_ADC_SetCommonClock(__LL_ADC_COMMON_INSTANCE(ADC1), LL_ADC_CLOCK_ASYNC_DIV1);
    
    /* Set ADC measurement path to internal channels */
#if USE_INTERVREF
		_interChannel |= LL_ADC_PATH_INTERNAL_VREFINT;
	//LL_ADC_SetCommonPathInternalCh(__LL_ADC_COMMON_INSTANCE(ADC1), LL_ADC_PATH_INTERNAL_VREFINT);
#endif
	 LL_ADC_SetCommonPathInternalCh(__LL_ADC_COMMON_INSTANCE(ADC1),_interChannel);     
    
  /*## Configuration of ADC hierarchical scope: multimode ####################*/
  
    /* Note: Feature not available on this STM32 serie */ 
    
  }
}


uint8_t E_AnalogDMA::channelNum = 0;

void E_AnalogDMA::dmaConfig()
{  
  /*## Configuration of NVIC #################################################*/
  /* Configure NVIC to enable ADC1 interruptions */
//  NVIC_SetPriority(ADC1_COMP_IRQn, 0); /* ADC IRQ greater priority than DMA IRQ */
//  NVIC_EnableIRQ(ADC1_COMP_IRQn);
  
  /*## Configuration of ADC ##################################################*/
  
  /*## Configuration of ADC hierarchical scope: common to several ADC ########*/
  
  /* Enable ADC clock (core clock) */
  LL_APB1_GRP2_EnableClock(LL_APB1_GRP2_PERIPH_ADC1);
  
  /* Note: Hardware constraint (refer to description of the functions         */
  /*       below):                                                            */
  /*       On this STM32 serie, setting of these features is conditioned to   */
  /*       ADC state:                                                         */
  /*       All ADC instances of the ADC common group must be disabled.        */
  /* Note: In this example, all these checks are not necessary but are        */
  /*       implemented anyway to show the best practice usages                */
  /*       corresponding to reference manual procedure.                       */
  /*       Software can be optimized by removing some of these checks, if     */
  /*       they are not relevant considering previous settings and actions    */
  /*       in user application.                                               */
  if(__LL_ADC_IS_ENABLED_ALL_COMMON_INSTANCE() == 0)
  {
    /* Note: Call of the functions below are commented because they are       */
    /*       useless in this example:                                         */
    /*       setting corresponding to default configuration from reset state. */
    
    /* Set ADC clock (conversion clock) common to several ADC instances */
    /* Note: On this STM32 serie, ADC common clock asynchonous prescaler      */
    /*       is applied to each ADC instance if ADC instance clock is         */
    /*       set to clock source asynchronous                                 */
    /*       (refer to function "LL_ADC_SetClock()" below).                   */
    // LL_ADC_SetCommonClock(__LL_ADC_COMMON_INSTANCE(ADC1), LL_ADC_CLOCK_ASYNC_DIV1);
    
    /* Set ADC measurement path to internal channels */
    // LL_ADC_SetCommonPathInternalCh(__LL_ADC_COMMON_INSTANCE(ADC1), LL_ADC_PATH_INTERNAL_NONE);    
  }
  
  
  /*## Configuration of ADC hierarchical scope: ADC instance #################*/
  
  /* Note: Hardware constraint (refer to description of the functions         */
  /*       below):                                                            */
  /*       On this STM32 serie, setting of these features is conditioned to   */
  /*       ADC state:                                                         */
  /*       ADC must be disabled.                                              */
  if (LL_ADC_IsEnabled(ADC1) == 0)
  {
    /* Note: Call of the functions below are commented because they are       */
    /*       useless in this example:                                         */
    /*       setting corresponding to default configuration from reset state. */
    
    /* Set ADC clock (conversion clock) */
    LL_ADC_SetClock(ADC1, LL_ADC_CLOCK_SYNC_PCLK_DIV2);
    
    /* Set ADC data resolution */
    // LL_ADC_SetResolution(ADC1, LL_ADC_RESOLUTION_12B);
    
    /* Set ADC conversion data alignment */
    // LL_ADC_SetResolution(ADC1, LL_ADC_DATA_ALIGN_RIGHT);
    
    /* Set ADC low power mode */
    // LL_ADC_SetLowPowerMode(ADC1, LL_ADC_LP_MODE_NONE);
    
    /* Set ADC channels sampling time */
    /* Note: On this STM32 serie, sampling time is common to all channels     */
    /*       of the entire ADC instance.                                      */
    /*       Therefore, sampling time is configured here under ADC instance   */
    /*       scope (not under channel scope as on some other STM32 devices    */
    /*       on which sampling time is channel wise).                         */
    /* Note: Considering interruption occurring after each number of          */
    /*       "ADC_CONVERTED_DATA_BUFFER_SIZE" ADC conversions                 */
    /*       (IT from DMA transfer complete),                                 */
    /*       select sampling time and ADC clock with sufficient               */
    /*       duration to not create an overhead situation in IRQHandler.      */
    LL_ADC_SetSamplingTimeCommonChannels(ADC1, LL_ADC_SAMPLINGTIME_41CYCLES_5);    
  }
  
  
  /*## Configuration of ADC hierarchical scope: ADC group regular ############*/
  
  /* Note: Hardware constraint (refer to description of the functions         */
  /*       below):                                                            */
  /*       On this STM32 serie, setting of these features is conditioned to   */
  /*       ADC state:                                                         */
  /*       ADC must be disabled or enabled without conversion on going        */
  /*       on group regular.                                                  */
  if ((LL_ADC_IsEnabled(ADC1) == 0)               ||
      (LL_ADC_REG_IsConversionOngoing(ADC1) == 0)   )
  {
    /* Set ADC group regular trigger source */
    LL_ADC_REG_SetTriggerSource(ADC1, LL_ADC_REG_TRIG_SOFTWARE);
    
    /* Set ADC group regular trigger polarity */
    // LL_ADC_REG_SetTriggerEdge(ADC1, LL_ADC_REG_TRIG_EXT_RISING);
    /* Set ADC group regular continuous mode */
    LL_ADC_REG_SetContinuousMode(ADC1, LL_ADC_REG_CONV_CONTINUOUS);    
    /* Set ADC group regular conversion data transfer */
//    LL_ADC_REG_SetDMATransfer(ADC1, LL_ADC_REG_DMA_TRANSFER_UNLIMITED);


    
    /* Set ADC group regular overrun behavior */
    LL_ADC_REG_SetOverrun(ADC1, LL_ADC_REG_OVR_DATA_OVERWRITTEN);
    
    /* Set ADC group regular sequencer */
    /* Note: On this STM32 serie, ADC group regular sequencer is              */
    /*       not fully configurable: sequencer length and each rank           */
    /*       affectation to a channel are fixed by channel HW number.         */
    /*       Refer to description of function                                 */
    /*       "LL_ADC_REG_SetSequencerChannels()".                             */
    
    /* Set ADC group regular sequencer discontinuous mode */
    // LL_ADC_REG_SetSequencerDiscont(ADC1, LL_ADC_REG_SEQ_DISCONT_DISABLE);
    
    /* Set ADC group regular sequence: channel on rank corresponding to       */
    /* channel number.                                                        */
//    LL_ADC_REG_SetSequencerChannels(ADC1, LL_ADC_CHANNEL_4);
		LL_ADC_REG_SetSequencerChannels(ADC1, channel);
  }
  
  
  /*## Configuration of ADC hierarchical scope: ADC group injected ###########*/
  
  /* Note: Feature not available on this STM32 serie */ 
  
  
  /*## Configuration of ADC hierarchical scope: channels #####################*/
  
  /* Note: Hardware constraint (refer to description of the functions         */
  /*       below):                                                            */
  /*       On this STM32 serie, setting of these features is conditioned to   */
  /*       ADC state:                                                         */
  /*       ADC must be disabled or enabled without conversion on going        */
  /*       on either groups regular or injected.                              */
  if ((LL_ADC_IsEnabled(ADC1) == 0)               ||
      (LL_ADC_REG_IsConversionOngoing(ADC1) == 0)   )
  {
    /* Set ADC channels sampling time */
    /* Note: On this STM32 serie, sampling time is common to all channels     */
    /*       of the entire ADC instance.                                      */
    /*       See sampling time configured above, at ADC instance scope.       */
    
  }
  
  
  /*## Configuration of ADC transversal scope: analog watchdog ###############*/
  
  /* Note: On this STM32 serie, there is only 1 analog watchdog available.    */
  
  /* Set ADC analog watchdog: channels to be monitored */
  // LL_ADC_SetAnalogWDMonitChannels(ADC1, LL_ADC_AWD_DISABLE);
  
  /* Set ADC analog watchdog: thresholds */
  // LL_ADC_ConfigAnalogWDThresholds(ADC1, __LL_ADC_DIGITAL_SCALE(LL_ADC_RESOLUTION_12B), 0x000);
  
  
  /*## Configuration of ADC transversal scope: oversampling ##################*/
  
  /* Set ADC oversampling scope */
  // LL_ADC_SetOverSamplingScope(ADC1, LL_ADC_OVS_DISABLE);  
  /* Set ADC oversampling parameters */
  // LL_ADC_ConfigOverSamplingRatioShift(ADC1, LL_ADC_OVS_RATIO_2, LL_ADC_OVS_SHIFT_NONE);
  
  
  /*## Configuration of ADC interruptions ####################################*/
  /* Enable interruption ADC group regular overrun */
//  LL_ADC_EnableIT_OVR(ADC1);
  

  /*## Configuration of NVIC #################################################*/
  /* Configure NVIC to enable DMA interruptions */
//  NVIC_SetPriority(DMA1_Channel1_IRQn, 1); /* DMA IRQ lower priority than ADC IRQ */
//  NVIC_EnableIRQ(DMA1_Channel1_IRQn);
  
  /*## Configuration of DMA ##################################################*/
  /* Enable the peripheral clock of DMA */
  LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_DMA1);
  
  /* Configure the DMA transfer */
  /*  - DMA transfer in circular mode to match with ADC configuration:        */
  /*    DMA unlimited requests.                                               */
  /*  - DMA transfer from ADC without address increment.                      */
  /*  - DMA transfer to memory with address increment.                        */
  /*  - DMA transfer from ADC by half-word to match with ADC configuration:   */
  /*    ADC resolution 12 bits.                                               */
  /*  - DMA transfer to memory by half-word to match with ADC conversion data */
  /*    buffer variable type: half-word.                                      */
  LL_DMA_ConfigTransfer(DMA1,
                        LL_DMA_CHANNEL_1,
                        LL_DMA_DIRECTION_PERIPH_TO_MEMORY |
                        LL_DMA_MODE_CIRCULAR              |
                        LL_DMA_PERIPH_NOINCREMENT         |
                        LL_DMA_MEMORY_INCREMENT           |
                        LL_DMA_PDATAALIGN_HALFWORD        |
                        LL_DMA_MDATAALIGN_HALFWORD        |
                        LL_DMA_PRIORITY_HIGH               );
  
  /* Set DMA transfer addresses of source and destination */
  LL_DMA_ConfigAddresses(DMA1,
                         LL_DMA_CHANNEL_1,
                         LL_ADC_DMA_GetRegAddr(ADC1, LL_ADC_DMA_REG_REGULAR_DATA),
                         (uint32_t)&buffer,
                         LL_DMA_DIRECTION_PERIPH_TO_MEMORY);
  if(_deep == 0){
  /* Set DMA transfer size */
  LL_DMA_SetDataLength(DMA1,
                       LL_DMA_CHANNEL_1,
                       channelNum);
		LL_ADC_REG_SetDMATransfer(ADC1, LL_ADC_REG_DMA_TRANSFER_UNLIMITED);
	}else{
		  LL_DMA_SetDataLength(DMA1,
                       LL_DMA_CHANNEL_1,
                       channelNum*_deep);
		  LL_ADC_REG_SetDMATransfer(ADC1, LL_ADC_REG_DMA_TRANSFER_LIMITED);
	}
  
//  /* Enable DMA transfer interruption: transfer complete */
//  LL_DMA_EnableIT_TC(DMA1,
//                     LL_DMA_CHANNEL_1);
//  
//  /* Enable DMA transfer interruption: half transfer */
//  LL_DMA_EnableIT_HT(DMA1,
//                     LL_DMA_CHANNEL_1);
//  
//  /* Enable DMA transfer interruption: transfer error */
//  LL_DMA_EnableIT_TE(DMA1,
//                     LL_DMA_CHANNEL_1);
  
  /*## Activation of DMA #####################################################*/
  /* Enable the DMA transfer */
  LL_DMA_EnableChannel(DMA1,
                       LL_DMA_CHANNEL_1);
}

void E_AnalogDMA::dmaStart()
{
  __IO uint32_t wait_loop_index = 0;
  __IO uint32_t backup_setting_adc_dma_transfer = 0;
//  #if (USE_TIMEOUT == 1)
//  uint32_t Timeout = 0; /* Variable used for timeout management */
//  #endif /* USE_TIMEOUT */
  
  /*## Operation on ADC hierarchical scope: ADC instance #####################*/
  
  /* Note: Hardware constraint (refer to description of the functions         */
  /*       below):                                                            */
  /*       On this STM32 serie, setting of these features is conditioned to   */
  /*       ADC state:                                                         */
  /*       ADC must be disabled.                                              */
  /* Note: In this example, all these checks are not necessary but are        */
  /*       implemented anyway to show the best practice usages                */
  /*       corresponding to reference manual procedure.                       */
  /*       Software can be optimized by removing some of these checks, if     */
  /*       they are not relevant considering previous settings and actions    */
  /*       in user application.                                               */
  if (LL_ADC_IsEnabled(ADC1) == 0)
  {
    /* Disable ADC DMA transfer request during calibration */
    /* Note: Specificity of this STM32 serie: Calibration factor is           */
    /*       available in data register and also transfered by DMA.           */
    /*       To not insert ADC calibration factor among ADC conversion data   */
    /*       in array variable, DMA transfer must be disabled during          */
    /*       calibration.                                                     */
    backup_setting_adc_dma_transfer = LL_ADC_REG_GetDMATransfer(ADC1);
    LL_ADC_REG_SetDMATransfer(ADC1, LL_ADC_REG_DMA_TRANSFER_NONE);
    
    /* Run ADC self calibration */
    LL_ADC_StartCalibration(ADC1);
    
    /* Poll for ADC effectively calibrated */
//    #if (USE_TIMEOUT == 1)
//    Timeout = ADC_CALIBRATION_TIMEOUT_MS;
//    #endif /* USE_TIMEOUT */
    
    while (LL_ADC_IsCalibrationOnGoing(ADC1) != 0)
    {
//    #if (USE_TIMEOUT == 1)
//      /* Check Systick counter flag to decrement the time-out value */
//      if (LL_SYSTICK_IsActiveCounterFlag())
//      {
//        if(Timeout-- == 0)
//        {
//        /* Time-out occurred. Set LED to blinking mode */
//        LED_Blinking(LED_BLINK_ERROR);
//        }
//      }
//    #endif /* USE_TIMEOUT */
    }
    
    /* Delay between ADC end of calibration and ADC enable.                   */
    /* Note: Variable divided by 2 to compensate partially                    */
    /*       CPU processing cycles (depends on compilation optimization).     */
//    wait_loop_index = (ADC_DELAY_CALIB_ENABLE_CPU_CYCLES >> 1);
//    while(wait_loop_index != 0)
//    {
//      wait_loop_index--;
//    }
    
    /* Restore ADC DMA transfer request after calibration */
    LL_ADC_REG_SetDMATransfer(ADC1, backup_setting_adc_dma_transfer);
    
    /* Enable ADC */
    LL_ADC_Enable(ADC1);
    
    /* Poll for ADC ready to convert */
//    #if (USE_TIMEOUT == 1)
//    Timeout = ADC_ENABLE_TIMEOUT_MS;
//    #endif /* USE_TIMEOUT */
    
    while (LL_ADC_IsActiveFlag_ADRDY(ADC1) == 0)
    {
//    #if (USE_TIMEOUT == 1)
//      /* Check Systick counter flag to decrement the time-out value */
//      if (LL_SYSTICK_IsActiveCounterFlag())
//      {
//        if(Timeout-- == 0)
//        {
//        /* Time-out occurred. Set LED to blinking mode */
//        LED_Blinking(LED_BLINK_ERROR);
//        }
//      }
//    #endif /* USE_TIMEOUT */
    }
    
    /* Note: ADC flag ADRDY is not cleared here to be able to check ADC       */
    /*       status afterwards.                                               */
    /*       This flag should be cleared at ADC Deactivation, before a new    */
    /*       ADC activation, using function "LL_ADC_ClearFlag_ADRDY()".       */
  }
  
  /*## Operation on ADC hierarchical scope: ADC group regular ################*/
  /* Note: No operation on ADC group regular performed here.                  */
  /*       ADC group regular conversions to be performed after this function  */
  /*       using function:                                                    */
  /*       "LL_ADC_REG_StartConversion();"                                    */
  
  /*## Operation on ADC hierarchical scope: ADC group injected ###############*/
  /* Note: Feature not available on this STM32 serie */ 
    if ((LL_ADC_IsEnabled(ADC1) == 1)               &&
      (LL_ADC_IsDisableOngoing(ADC1) == 0)        &&
      (LL_ADC_REG_IsConversionOngoing(ADC1) == 0)   )
  {
    LL_ADC_REG_StartConversion(ADC1);
  }
}

