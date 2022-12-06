/*
 * PID.c
 *
 *  Created on: 30 nov 2022
 *      Author: axel1
 */

#include "PID.h"

extern QueueHandle_t Freq_queue;

void PID_TASK(void *parameters)
{
	// For 6ta String

	const float T_muestreo = 0.01;

	float frequency = 0;

	float error_string = 0;
	float error_string_m1 = 0;

	float Out = 0;
	float PID_Integral = 0;
	float PID_Integral_m1 = 0;
	float PID_Derv = 0;

	const float KP_s6 = 0.8;
	const float KI_s6 = 0;
	const float KD_s6 = 0;
	float ref_string6 = 100;

	const float KP_s1 = 0.8;
	const float KI_s1 = 0;
	const float KD_s1 = 0;
	float ref_string1 = 329.63;

	float ref = 0;
	float KP = 0;
	float KI = 0;
	float KD = 0;

	uint8_t string = 6;

	double frequency_data = 0;

	while(1)
	{

		xQueueReceive(Freq_queue, &frequency_data, portMAX_DELAY);

		frequency = frequency_data;

		if(6 == string)
		{
			ref = ref_string6;
			KP = KP_s6;
			KI = KI_s6;
			KD = KD_s6;
		}
		else if(1 == string)
		{
			ref = ref_string1;
			KP = KP_s1;
			KI = KI_s1;
			KD = KD_s1;
		}

		error_string = ref - frequency;
		PID_Integral = PID_Integral_m1 +KI * (float) T_muestreo * error_string;

		PID_Derv = (KD/(float)T_muestreo)*(error_string-error_string_m1);

		Out = KP*error_string+PID_Derv+(PID_Integral);

		error_string_m1 = error_string;
		PID_Integral_m1 = PID_Integral;

		if(10 <= Out)
		{
			Out = 10;
		}

		float U1 = Out*10;

		changeDutyCycle(kMOTOR_RIGHT, U1);

		EnableIRQ(ADC1_IRQn);
	}


}
