/*
 * Correlation.h
 *
 *  Created on: 5 dic 2022
 *      Author: Oliver
 */

#ifndef CONV_CORRELATION_H_
#define CONV_CORRELATION_H_

#include "common.h"
#include "fsl_adc16.h"

#define VECTOR_SIZE 500
#define CONST 1986 // CD voltage

#define T_ADC 0.000052631 //1/f_ADC //19KHz

#define OFFSET1 12

double frequencyvalue(void);
void AutoCorrelation_TASK (void * parameters);

#endif /* CONV_CORRELATION_H_ */
