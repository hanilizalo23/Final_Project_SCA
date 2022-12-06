/*
 * ADC.h
 *
 *  Created on: 5 dic 2022
 *      Author: Oliver
 */

#ifndef ADC_ADC_H_
#define ADC_ADC_H_

#include "common.h"
#include "fsl_debug_console.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "board.h"
#include "fsl_adc16.h"

#define ADC16_BASE          	ADC1
#define ADC16_CHANNEL_GROUP 	0U
#define ADC16_USER_CHANNEL  	23U

#define ADC16_IRQn             ADC1_IRQn
#define ADC16_IRQ_HANDLER_FUNC ADC1_IRQHandler

/*Functions*/

/*Initialice ADC*/
void InitADCModule(void);

/*Read values and send them by Queue*/
void ADC_TASK (void * parameters);


#endif /* ADC_ADC_H_ */
