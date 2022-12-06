/*
 * FTM.c
 *
 *  Created on: 1 dic 2022
 *      Author: axel1
 */

#include "FTM.h"

uint8_t setup_pwm(void)
{
	ftm_config_t ftmInfo;
	ftm_chnl_pwm_signal_param_t ftmParam;
	ftm_chnl_pwm_signal_param_t ftmParam2;

	ftm_pwm_level_select_t pwmLevel = FTM_PWM_ON_LEVEL;

	/*Preliminary duty cycles*/
	float duty1 = 10;
	float duty2 = 0;

	/* Fill in the FTM config struct with the default settings */
	FTM_GetDefaultConfig(&ftmInfo);

	/* Calculate the clock division based on the PWM frequency to be obtained */
	ftmInfo.prescale = FTM_CalculateCounterClkDiv(FTM0, FREQUENCY_PWM, FTM_SOURCE_CLOCK);

	/* Initialize FTM module */
	FTM_Init(FTM0, &ftmInfo);;

	/* Configure ftm params with frequency 50HZ */

	/*Motor 2*/
	ftmParam.chnlNumber = kFTM_Chnl_1;
	ftmParam.level = pwmLevel;
	ftmParam.dutyCyclePercent = duty1;
	ftmParam.firstEdgeDelayPercent = 0U;
	ftmParam.enableComplementary = false;
	ftmParam.enableDeadtime = false;

	/*Motor 3*/
	ftmParam2.chnlNumber = kFTM_Chnl_2;
	ftmParam2.level = pwmLevel;
	ftmParam2.dutyCyclePercent = duty2;
	ftmParam2.firstEdgeDelayPercent = 0U;
	ftmParam2.enableComplementary = false;
	ftmParam2.enableDeadtime = false;

	/*Setup 2 PWM*/

	if (kStatus_Success != FTM_SetupPwm(FTM0, &ftmParam, 1U, kFTM_EdgeAlignedPwm,FREQUENCY_PWM, FTM_SOURCE_CLOCK)) {
		return -1;
	}
	if (kStatus_Success != FTM_SetupPwm(FTM0, &ftmParam2, 1U, kFTM_EdgeAlignedPwm,FREQUENCY_PWM, FTM_SOURCE_CLOCK)) {
		return -1;
	}

	/*Start both FTMs*/
	FTM_StartTimer(FTM0, kFTM_SystemClock);

	return 1;
}

void changeDutyCycle(motor_selector Mx, float dutyCycle) {

	FTM_Type *ftm_select;
	ftm_chnl_t ftm_channel;

	if (Mx == 0) {
		ftm_select = FTM0;
		ftm_channel = kFTM_Chnl_1;

	} else if (Mx == 1) {
		ftm_select = FTM0;
		ftm_channel = kFTM_Chnl_2;
	}


	FTM_UpdateChnlEdgeLevelSelect(ftm_select, ftm_channel, 0U);
	if (kStatus_Success!= FTM_UpdatePwmDutycycle(ftm_select, ftm_channel,kFTM_EdgeAlignedPwm, dutyCycle)) {
	}

	FTM_SetSoftwareTrigger(ftm_select, true);

	FTM_UpdateChnlEdgeLevelSelect(ftm_select, ftm_channel, FTM_PWM_ON_LEVEL);

}
