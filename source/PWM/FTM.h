/*
 * FTM.h
 */

#ifndef PWM_FTM_H_
#define PWM_FTM_H_
#include "common.h"
#include "fsl_ftm.h"

#define FTM_SOURCE_CLOCK CLOCK_GetFreq(kCLOCK_BusClk)
#define FREQUENCY_PWM 50
#ifndef FTM_PWM_ON_LEVEL
#define FTM_PWM_ON_LEVEL kFTM_HighTrue
#endif

typedef enum _motor_selector {
	kMOTOR_RIGHT = 0U, kMOTOR_LEFT
} motor_selector;
uint8_t setup_pwm(void);

void changeDutyCycle(motor_selector Mx, float dutyCycle);
#endif /* PWM_FTM_H_ */
