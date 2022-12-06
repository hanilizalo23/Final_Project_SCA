/*
 * ADC.c
 *
 *  Created on: 5 dic 2022
 *      Author: Oliver
 */

#include "ADC.h"

volatile	bool 		g_Adc16ConversionDoneFlag = false;
volatile	uint32_t 	g_Adc16ConversionValue;
volatile	uint32_t 	g_Adc16InterruptCounter;
static	 	BaseType_t		 xHigherPriorityTaskWoken = pdFALSE;
const 		uint32_t 	g_Adc16_12bitFullRange = 4096U;

SemaphoreHandle_t g_Semaphore_ADC;
QueueHandle_t ADC_queue;

void ADC16_IRQ_HANDLER_FUNC(void)
{

    g_Adc16ConversionDoneFlag = true;
    /* Read conversion result to clear the conversion completed flag. */
    g_Adc16ConversionValue = ADC16_GetChannelConversionValue(ADC16_BASE, ADC16_CHANNEL_GROUP);
    g_Adc16InterruptCounter++;
    xSemaphoreGiveFromISR(g_Semaphore_ADC, &xHigherPriorityTaskWoken);
    SDK_ISR_EXIT_BARRIER;
    portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
}

void InitADCModule(void)
{
    adc16_config_t adc16ConfigStruct;
    NVIC_SetPriority(ADC16_IRQn,4);
	EnableIRQ(ADC16_IRQn);
	ADC16_GetDefaultConfig(&adc16ConfigStruct);
	ADC16_Init(ADC16_BASE, &adc16ConfigStruct);
	ADC16_EnableHardwareTrigger(ADC16_BASE, false); /* Make sure the software trigger is used. */

	#if defined(FSL_FEATURE_ADC16_HAS_CALIBRATION) && FSL_FEATURE_ADC16_HAS_CALIBRATION
		if (kStatus_Success == ADC16_DoAutoCalibration(ADC16_BASE))
		{
			PRINTF("ADC16_DoAutoCalibration() Done.\r\n");
		}
		else
		{
			PRINTF("ADC16_DoAutoCalibration() Failed.\r\n");
		}
	#endif /* FSL_FEATURE_ADC16_HAS_CALIBRATION */
}

void ADC_TASK (void * parameters)
{
	adc16_channel_config_t adc16ChannelConfigStruct;
	g_Semaphore_ADC = xSemaphoreCreateBinary();

    adc16ChannelConfigStruct.channelNumber                        = ADC16_USER_CHANNEL;
    adc16ChannelConfigStruct.enableInterruptOnConversionCompleted = true; /* Enable the interrupt. */
#if defined(FSL_FEATURE_ADC16_HAS_DIFF_MODE) && FSL_FEATURE_ADC16_HAS_DIFF_MODE
    adc16ChannelConfigStruct.enableDifferentialConversion = false;
#endif /* FSL_FEATURE_ADC16_HAS_DIFF_MODE */

	uint16_t sendADCValue;

	InitADCModule();
	while(1)
	{
		ADC16_SetChannelConfig(ADC16_BASE,ADC16_CHANNEL_GROUP, &adc16ChannelConfigStruct);
		xSemaphoreTake(g_Semaphore_ADC,portMAX_DELAY);
		sendADCValue = g_Adc16ConversionValue;
		xQueueGenericSend(ADC_queue, &sendADCValue, portMAX_DELAY, queueOVERWRITE);
	}
}
