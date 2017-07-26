/**
  ******************************************************************************
  * @file    ebox_analog.cpp
  * @author  cat_li
  * @version V2.0
  * @date    2016/10/21
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


	
int get_channel(E_PinID pin){
	
		switch ((uint32_t)pin)
		{
// PA0-7			
			case PA_0:
				return LL_ADC_CHANNEL_0 ;
			case PA_1:
				return LL_ADC_CHANNEL_1 ;
//			case PA2:
//				return LL_ADC_CHANNEL_2 ;
//			case PA3:
//				return LL_ADC_CHANNEL_3 ;
//			case PA4:
//				return LL_ADC_CHANNEL_4 ;
//			case PA5:
//				return LL_ADC_CHANNEL_5 ;
//			case PA6:
//				return LL_ADC_CHANNEL_6 ;
//			case PA7:
//				return LL_ADC_CHANNEL_7 ;
			case 0xffffffff:
				return LL_ADC_CHANNEL_7 ;
			case 0xfffffff1:
				return LL_ADC_CHANNEL_7 ;
// PB0,1				
			case PB_0:
				return LL_ADC_CHANNEL_8 ;
				//break;
			case PB_1:
				return LL_ADC_CHANNEL_9 ;
				//break;	
//// PC0-5				
//			case PC0:
//				return LL_ADC_CHANNEL_10 ;
//				break;
//			case PC1:
//				return LL_ADC_CHANNEL_11 ;
//				break;
//			case PC2:
//				return LL_ADC_CHANNEL_12 ;
//				break;
//			case PC3:
//				return LL_ADC_CHANNEL_13 ;
//				break;
//			case PC4:
//				return LL_ADC_CHANNEL_14 ;
//				break;
//			case PC5:
//				return LL_ADC_CHANNEL_15 ;
//				break;				
			default:
				return (uint16_t)NC;
		}
	}


	
	/**
 *@name     void ADC1_init(void)
 *@brief    ADC初始化
 *@param    NONE
 *@retval   NONE
*/
void ADC1_init(void)
{
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
    
    
  /*## Configuration of ADC hierarchical scope: multimode ####################*/
  
    /* Note: Feature not available on this STM32 serie */ 
    
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
    LL_ADC_REG_SetContinuousMode(ADC1, LL_ADC_REG_CONV_SINGLE);
    
    /* Set ADC group regular conversion data transfer */
    // LL_ADC_REG_SetDMATransfer(ADC1, LL_ADC_REG_DMA_TRANSFER_NONE);
    
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
    //LL_ADC_REG_SetSequencerChannels(ADC1, LL_ADC_CHANNEL_4);
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
  //LL_ADC_EnableIT_OVR(ADC1);
  
  /* Note: In this example, end of ADC conversions are awaited by polling     */
  /*       (not by interruption).                                             */
  
}

/**
 *@name     uint16_t analog_read(uint32_t *channel)
 *@brief    读取ADC1某个引脚上的模拟转换结果
 *@param    channel：ADC1 通道
 *@retval   如果引脚正确则返回该引脚的模拟电压值所对应的12bit的ADC转换结果
            如果引脚错误返回0；
*/
uint16_t analogin_read(uint16_t *channel)
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

	LL_ADC_REG_SetSequencerChannels(ADC1, *channel);

	LL_ADC_REG_StartConversion(ADC1);
	while (!LL_ADC_IsActiveFlag_EOC(ADC1));

	return LL_ADC_REG_ReadConversionData12(ADC1);
}

/**
 *@name     uint16_t analog_read_voltage(uint32_t *channel)
 *@brief    读取某个引脚上的模拟电压
 *@param    channel：ADC1引脚对应的通道
 *@retval   如果引脚正确则返回该引脚的模拟电压值所对应的模拟电压，默认参考电压为3.3V
*/
uint16_t analogin_read_voltage(uint16_t *channel){
	return __LL_ADC_CALC_DATA_TO_VOLTAGE(VDDA_APPLI,analogin_read(channel),LL_ADC_RESOLUTION_12B);
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

