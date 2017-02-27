/**
  ******************************************************************************
  * @file    analog.cpp
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
 * -LQM (2016/9/5)
 *      *移植到STM32F0,基于HAL库LL层
 */

/* Includes ------------------------------------------------------------------*/
#include "common.h"
#include "Analog.h"


/** @defgroup analog 
  * @brief analog driver modules默认开启16通道 采用DMA+ADC连续转换模式。提供AD采集服务，只需将IO设置为AIN
  * 模式即可读取引脚相应的电压
  * @{
  */

/** @defgroup analog私有类型定义
  * @{
  */

/**
  * @}
  */

/** @defgroup analog私有宏定义
  * @{
  */  
  
//#define CH 16/* ADC JEXTTRIG mask */

/**
  * @}
  */
  
  
/** @defgroup analog私有变量
  * @{
  */
  
//u16  AD_value[CH];///< 用来存放ADC转换结果，也是DMA的目标地址
  
/**
  * @}
  */


/** @defgroup analog函数
  * @{
  */


/**
 *@name     void DMA_configuration(void)
 *@brief    ADC的DMA配置，将ADC1配置为DMA自动传输模式。转换结果自动的传输到内存
 *@param    NONE
 *@retval   NONE
*/
void DMA_configuration(void)
{
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
}

/**
 *@name     void ADC1_init(void)
 *@brief    ADC初始化
 *@param    NONE
 *@retval   NONE
*/
void ADC1_init(void)
{
	/*## Configuration of ADC hierarchical scope: ADC instance #################*/
	/*       ADC must be disabled.                                              */
	if (LL_ADC_IsEnabled(ADC1) == 0)
	{
		/* Enable ADC clock (core clock) */
		LL_APB1_GRP2_EnableClock(LL_APB1_GRP2_PERIPH_ADC1);
		// ADC Initial
		/* Set ADC clock (conversion clock) */
		LL_ADC_SetClock(ADC1, LL_ADC_CLOCK_SYNC_PCLK_DIV2);
		/* Set ADC data resolution */
		LL_ADC_SetResolution(ADC1, LL_ADC_RESOLUTION_12B);
		/* Set ADC conversion data alignment */
		LL_ADC_SetResolution(ADC1, LL_ADC_DATA_ALIGN_RIGHT);
		/* Set ADC low power mode */
		LL_ADC_SetLowPowerMode(ADC1, LL_ADC_LP_MODE_NONE);

		/* Set ADC group regular trigger source */
		LL_ADC_REG_SetTriggerSource(ADC1, LL_ADC_REG_TRIG_SOFTWARE);
		/* Set ADC group regular trigger polarity 外部触法极性 */
		// LL_ADC_REG_SetTriggerEdge(ADC1, LL_ADC_REG_TRIG_EXT_RISING);
		/* Set ADC group regular conversion data transfer */
		// LL_ADC_REG_SetDMATransfer(ADC1, LL_ADC_REG_DMA_TRANSFER_NONE);
		/* Set ADC group regular overrun behavior */
		LL_ADC_REG_SetOverrun(ADC1, LL_ADC_REG_OVR_DATA_OVERWRITTEN);

		/* Set ADC group regular sequencer */
		/* Set ADC group regular continuous mode 一次触发转换序列中的所有通道*/
//    	LL_ADC_REG_SetContinuousMode(ADC1, LL_ADC_REG_CONV_SINGLE);
		LL_ADC_REG_SetContinuousMode(ADC1, LL_ADC_REG_CONV_CONTINUOUS);
		LL_ADC_REG_SetSequencerDiscont(ADC1, LL_ADC_REG_SEQ_DISCONT_DISABLE);

		/* Set ADC group regular continuous mode 一次触发按顺序转换序列中的一个通道*/
//    	LL_ADC_REG_SetContinuousMode(ADC1, LL_ADC_REG_CONV_SINGLE);
//    	LL_ADC_REG_SetSequencerDiscont(ADC1, LL_ADC_REG_SEQ_DISCONT_1RANK );

		/* Set ADC channels sampling time */
		/* Note: On this STM32 serie, sampling time is common to all channels     */
		/*       of the entire ADC instance.                                      */
		/*       See sampling time configured above, at ADC instance scope.       */
		LL_ADC_SetSamplingTimeCommonChannels(ADC1, LL_ADC_SAMPLINGTIME_239CYCLES_5);
	}

	if (__LL_ADC_IS_ENABLED_ALL_COMMON_INSTANCE() == 0)
	{
		/* Set ADC measurement path to internal channels */
		LL_ADC_SetCommonPathInternalCh(__LL_ADC_COMMON_INSTANCE(ADC1), LL_ADC_PATH_INTERNAL_TEMPSENSOR|LL_ADC_PATH_INTERNAL_VREFINT);
	}

	if (LL_ADC_IsEnabled(ADC1) == 0)
	{
		/* Run ADC self calibration */
		LL_ADC_StartCalibration(ADC1);
		while (LL_ADC_IsCalibrationOnGoing(ADC1) != 0)
		{
		}
		/* Enable ADC */
		LL_ADC_Enable(ADC1);
		/* Poll for ADC ready to convert */
		while (LL_ADC_IsActiveFlag_ADRDY(ADC1) == 0)
		{
		}
	}
}

/**
 *@name     uint16_t analog_read(uint32_t *channel)
 *@brief    读取ADC1某个引脚上的模拟转换结果
 *@param    channel：ADC1 通道
 *@retval   如果引脚正确则返回该引脚的模拟电压值所对应的12bit的ADC转换结果
            如果引脚错误返回0；
*/
uint16_t analogin_read(uint32_t *channel)
{
	//LL_DMA_DisableChannel(DMA1,LL_DMA_CHANNEL_1);
	LL_ADC_REG_SetDMATransfer(ADC1,LL_ADC_REG_DMA_TRANSFER_NONE);
	LL_ADC_REG_SetContinuousMode(ADC1, LL_ADC_REG_CONV_SINGLE);
	LL_ADC_REG_SetSequencerChannels(ADC1, *channel);

	LL_ADC_REG_StartConversion(ADC1);
	while (!LL_ADC_IsActiveFlag_EOC(ADC1));

	return LL_ADC_REG_ReadConversionData12(ADC1);
}

/**
 *@name     void analog_read(uint32_t *channel,uint16_t *buffer, uint16_t size)
 *@brief    DMA方式读取一组指定AD通道转换值
 *@param    channel：ADC1 通道
 *@param    buffer: DMA缓冲区
 *@param 	size: DMA缓冲区大小
 *@retval   void
*/
void analogin_read(uint32_t *channel,uint16_t *buffer, uint16_t size)
{
	//LL_ADC_REG_StopConversion(ADC1);
	LL_ADC_REG_SetContinuousMode(ADC1, LL_ADC_REG_CONV_CONTINUOUS);
	LL_ADC_REG_SetDMATransfer(ADC1,LL_ADC_REG_DMA_TRANSFER_LIMITED);

	/* Set DMA transfer addresses of source and destination */
	LL_DMA_ConfigAddresses(DMA1,LL_DMA_CHANNEL_1,LL_ADC_DMA_GetRegAddr(ADC1, LL_ADC_DMA_REG_REGULAR_DATA),
	                       (uint32_t)buffer,
	                       LL_DMA_DIRECTION_PERIPH_TO_MEMORY);
	/* Set DMA transfer size */
	LL_DMA_SetDataLength(DMA1,LL_DMA_CHANNEL_1,size);

	/* Enable the DMA transfer */
	LL_DMA_EnableChannel(DMA1,LL_DMA_CHANNEL_1);
	// 清除标志量,设置通道
	LL_DMA_ClearFlag_TC1(DMA1);
	LL_ADC_REG_SetSequencerChannels(ADC1, *channel);
	// 启动ADC,并等待结束
	LL_ADC_REG_StartConversion(ADC1);
	while (!LL_DMA_IsActiveFlag_TC1(DMA1));
}

/**
 *@name     uint16_t analog_read_voltage(uint32_t *channel)
 *@brief    读取某个引脚上的模拟电压
 *@param    channel：ADC1引脚对应的通道
 *@retval   如果引脚正确则返回该引脚的模拟电压值所对应的模拟电压，默认参考电压为3.3V
*/
uint16_t analogin_read_voltage(uint32_t *channel){
	return __LL_ADC_CALC_DATA_TO_VOLTAGE(VDDA_APPLI,analogin_read(channel),LL_ADC_RESOLUTION_12B);
}

/**
 *@name     uint16_t analog_read_temperature(void)
 *@brief    读取内部温度传感器
 *@param    void
 *@retval   温度值
*/
uint16_t analog_read_temperature(void){
	uint32_t  channel;
	channel = LL_ADC_CHANNEL_TEMPSENSOR;
	// 防止没有启动ADC
	if (LL_ADC_IsEnabled(ADC1) == 0){
		ADC1_init();
	}
	return __LL_ADC_CALC_TEMPERATURE(VDDA_APPLI,analogin_read(&channel),LL_ADC_RESOLUTION_12B);
}


/**
  * @}
  */
