/*
 * Correlation.c
 *
 *  Created on: 5 dic 2022
 *      Author: Oliver
 */

#include "Correlation.h"

extern QueueHandle_t ADC_queue;

QueueHandle_t Freq_queue;

int32_t vectorCorr[VECTOR_SIZE*2] = {0};

void AutoCorrelation_TASK (void * parameters)
{
	uint16_t ADC_data;
	int16_t ADC_values[VECTOR_SIZE] = {0};
	uint32_t counter = 0;

	double frequency;

	while(1)
	{
		while(VECTOR_SIZE > counter)
		{
			xQueueReceive(ADC_queue, &ADC_data, portMAX_DELAY);
			ADC_values[counter] = ADC_data;
			counter++;
		}
		counter = 0;
		DisableIRQ(ADC1_IRQn);

/* Signal Preprosesing */

		for(uint16_t v = 0 ; v < VECTOR_SIZE; v++)
		{
			if(ADC_values[v] != 0)
			{
				ADC_values[v] = ADC_values[v]-CONST;
			}
		}

/* Autocorrelation */

		uint32_t index_A;
		uint32_t index_B;

		for(uint16_t ctmp = 1; ctmp<= (VECTOR_SIZE*2)-1 ; ctmp++)
		{
			index_A = VECTOR_SIZE-ctmp;

			if(ctmp < VECTOR_SIZE)
			{
				for(index_B = 0; index_B<= ctmp ; index_B++)
				{
					vectorCorr[ctmp] += ADC_values[index_A+index_B]*ADC_values[index_B];
				}
			}
			else
			{
				for(index_B = ctmp-VECTOR_SIZE; index_B < VECTOR_SIZE ; index_B++)
				{
					vectorCorr[ctmp] += ADC_values[index_A+index_B]*ADC_values[index_B];
				}
			}
			vectorCorr[ctmp] /= 10000;
		}

/* END */
		frequency = frequencyvalue()/2+5;


		xQueueGenericSend(Freq_queue, &frequency, portMAX_DELAY, queueOVERWRITE);

		vTaskDelay(20/portTICK_PERIOD_MS);
	}
}
double frequencyvalue()
{
	uint8_t flag_down = 0;
	uint16_t tmp_index = 0;
	uint16_t tmp_index_base = 0;
	double x = 0;
	tmp_index = VECTOR_SIZE-2;
	while(flag_down<2)
	{
		if(tmp_index>=1)
		{
			if(((vectorCorr[tmp_index] > vectorCorr[tmp_index-1]))&&(flag_down == 0))
			{
				flag_down++;
				tmp_index_base = tmp_index;
			}
			else if(((vectorCorr[tmp_index]<vectorCorr[tmp_index-1]))&&(flag_down == 1))
			{
				flag_down++;
			}
			tmp_index--;
		}
		else
		{
			return 0;
		}
	}
	x = (1/(((float)tmp_index_base-(float)tmp_index)*T_ADC));
	return x;
}
