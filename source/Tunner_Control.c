/**
 * @file    Tunner_Control.c
 * @brief   Application entry point.
 */
#include <stdio.h>
#include "board.h"
#include "peripherals.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "MK66F18.h"
#include "fsl_debug_console.h"
/* TODO: insert other include files here. */

#include "common.h"

#include "LCD/LCD_nokia.h"
#include "SPI/SPI.h"
#include "PWM/FTM.h"
#include "PID/PID.h"
#include "ADC/ADC.h"
#include "Conv/Correlation.h"


/* TODO: insert other definitions and declarations here. */

extern QueueHandle_t ADC_queue;
extern QueueHandle_t Freq_queue;
/*
 * @brief   Application entry point.
 */

int main(void) {

    /* Init board hardware. */
    BOARD_InitBootPins();
    BOARD_InitBootClocks();
    BOARD_InitBootPeripherals();

//    LCD_nokia_init();

    ADC_queue = xQueueCreate(1, sizeof(uint16_t));
    Freq_queue = xQueueCreate(1,sizeof(double));

    setup_pwm();

     changeDutyCycle(kMOTOR_RIGHT, 30);
    changeDutyCycle(kMOTOR_RIGHT, 50);

    xTaskCreate(ADC_TASK, "ADC", 600, NULL, 5, NULL);

    /*xTaskCreate(Display_TASK, "Display", 350, (void*) adc_data, 5, NULL);
*/
    xTaskCreate(PID_TASK, "PID", 500, (void*)Freq_queue, 5, NULL);

    xTaskCreate(AutoCorrelation_TASK, "ACorr", 600, NULL, 5, NULL);
  /*  xTaskCreate(Bottoms, "BOT", 250, (void*)adc_data, 5, NULL);
*/
    vTaskStartScheduler();

    while(1) {

    }
    return 0 ;
}
