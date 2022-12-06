/*
 * PID.h
 *
 *  Created on: 30 nov 2022
 *      Author: axel1
 */

#ifndef PID_H_
#define PID_H_

#include "common.h"
#include "PWM/FTM.h"

void PID_TASK(void *parameters);

void Angles_task(void *parameters);

#endif /* PID_H_ */
