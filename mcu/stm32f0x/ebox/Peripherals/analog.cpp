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
 *      *��ֲ��STM32F0,����HAL��LL��
 */

/* Includes ------------------------------------------------------------------*/
#include "common.h"
#include "Analog.h"


/** @defgroup analog 
  * @brief analog driver modulesĬ�Ͽ���16ͨ�� ����DMA+ADC����ת��ģʽ���ṩAD�ɼ�����ֻ�轫IO����ΪAIN
  * ģʽ���ɶ�ȡ������Ӧ�ĵ�ѹ
  * @{
  */

/** @defgroup analog˽�����Ͷ���
  * @{
  */

/**
  * @}
  */

/** @defgroup analog˽�к궨��
  * @{
  */  
  
//#define CH 16/* ADC JEXTTRIG mask */

/**
  * @}
  */
  
  
/** @defgroup analog˽�б���
  * @{
  */
  
//u16  AD_value[CH];///< �������ADCת�������Ҳ��DMA��Ŀ���ַ
  
/**
  * @}
  */


/** @defgroup analog����
  * @{
  */


/**
 *@name     void DMA_configuration(void)
 *@brief    ADC��DMA���ã���ADC1����ΪDMA�Զ�����ģʽ��ת������Զ��Ĵ��䵽�ڴ�
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
 *@brief    ADC��ʼ��
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
		/* Set ADC group regular trigger polarity �ⲿ�������� */
		// LL_ADC_REG_SetTriggerEdge(ADC1, LL_ADC_REG_TRIG_EXT_RISING);
		/* Set ADC group regular conversion data transfer */
		// LL_ADC_REG_SetDMATransfer(ADC1, LL_ADC_REG_DMA_TRANSFER_NONE);
		/* Set ADC group regular overrun behavior */
		LL_ADC_REG_SetOverrun(ADC1, LL_ADC_REG_OVR_DATA_OVERWRITTEN);

		/* Set ADC group regular sequencer */
		/* Set ADC group regular continuous mode һ�δ���ת�������е�����ͨ��*/
//    	LL_ADC_REG_SetContinuousMode(ADC1, LL_ADC_REG_CONV_SINGLE);
		LL_ADC_REG_SetContinuousMode(ADC1, LL_ADC_REG_CONV_CONTINUOUS);
		LL_ADC_REG_SetSequencerDiscont(ADC1, LL_ADC_REG_SEQ_DISCONT_DISABLE);

		/* Set ADC group regular continuous mode һ�δ�����˳��ת�������е�һ��ͨ��*/
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
 *@brief    ��ȡADC1ĳ�������ϵ�ģ��ת�����
 *@param    channel��ADC1 ͨ��
 *@retval   ���������ȷ�򷵻ظ����ŵ�ģ���ѹֵ����Ӧ��12bit��ADCת�����
            ������Ŵ��󷵻�0��
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
 *@brief    DMA��ʽ��ȡһ��ָ��ADͨ��ת��ֵ
 *@param    channel��ADC1 ͨ��
 *@param    buffer: DMA������
 *@param 	size: DMA��������С
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
	// �����־��,����ͨ��
	LL_DMA_ClearFlag_TC1(DMA1);
	LL_ADC_REG_SetSequencerChannels(ADC1, *channel);
	// ����ADC,���ȴ�����
	LL_ADC_REG_StartConversion(ADC1);
	while (!LL_DMA_IsActiveFlag_TC1(DMA1));
}

/**
 *@name     uint16_t analog_read_voltage(uint32_t *channel)
 *@brief    ��ȡĳ�������ϵ�ģ���ѹ
 *@param    channel��ADC1���Ŷ�Ӧ��ͨ��
 *@retval   ���������ȷ�򷵻ظ����ŵ�ģ���ѹֵ����Ӧ��ģ���ѹ��Ĭ�ϲο���ѹΪ3.3V
*/
uint16_t analogin_read_voltage(uint32_t *channel){
	return __LL_ADC_CALC_DATA_TO_VOLTAGE(VDDA_APPLI,analogin_read(channel),LL_ADC_RESOLUTION_12B);
}

/**
 *@name     uint16_t analog_read_temperature(void)
 *@brief    ��ȡ�ڲ��¶ȴ�����
 *@param    void
 *@retval   �¶�ֵ
*/
uint16_t analog_read_temperature(void){
	uint32_t  channel;
	channel = LL_ADC_CHANNEL_TEMPSENSOR;
	// ��ֹû������ADC
	if (LL_ADC_IsEnabled(ADC1) == 0){
		ADC1_init();
	}
	return __LL_ADC_CALC_TEMPERATURE(VDDA_APPLI,analogin_read(&channel),LL_ADC_RESOLUTION_12B);
}


/**
  * @}
  */
