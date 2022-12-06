#include <stdio.h>
#include "board.h"
#include "peripherals.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "MK66F18.h"
#include "fsl_debug_console.h"

/* TODO: insert other include files here. */
#include "fsl_port.h"
#include "fsl_gpio.h"
#include "fsl_common.h"

/* FreeRTOS.org includes. */
#include "FreeRTOS.h"
#include "task.h"

#define STACK          1000
#define INITIAL 	   0
#define INIT_PRIOR     5
#define SWUP_PIN       16U
#define SWDOWN_PIN     8U
#define SWRIGHT_PIN    4U
#define SWLEFT_PIN     3U
#define ISR_PRIOR      0x3

#define GPIOC_16 0x10000
#define GPIOC_8	 0x100
#define GPIOC_4	 0x10
#define GPIOC_3	 0x8

void PORTC_IRQHandler(void);

static void init(void *pvParameters);

typedef enum{GPIO_A, /*!< Definition to select GPIO A */
			 GPIO_B, /*!< Definition to select GPIO B */
			 GPIO_C, /*!< Definition to select GPIO C */
			 GPIO_D, /*!< Definition to select GPIO D */
			 GPIO_E, /*!< Definition to select GPIO E */
			} gpio_port_name_t;

typedef enum{FALSE, TRUE} boolean_t;

int main(void)
{
	PRINTF("Hello World\n\r");

	xTaskCreate(init, "init", STACK, NULL, INIT_PRIOR, NULL);

	vTaskStartScheduler();

	for (;;)
	{
		;
	}

	return 0;
}

uint32_t GPIO_read_port(gpio_port_name_t port_name)
{
	uint32_t read_data;
	switch(port_name)
			{
				case GPIO_A: /** GPIO A is selected*/
					read_data = GPIOA->PDIR; /** Reads data from port A**/
					break;
				case GPIO_B: /** GPIO B is selected*/
					read_data = GPIOB->PDIR; /** Reads data from port B**/
					break;
				case GPIO_C: /** GPIO C is selected*/
					read_data = GPIOC->PDIR; /** Reads data from port C**/
					break;
				case GPIO_D: /** GPIO D is selected*/
					read_data = GPIOD->PDIR; /** Reads data from port D**/
					break;
				case GPIO_E: /** GPIO E is selected*/
					read_data = GPIOE->PDIR; /** Reads data from port E**/
					break;
				default:/**If doesn't exist the option*/
					read_data = FALSE;
			}
	return(read_data);
}

void PORTD_IRQHandler(void)
{
	BaseType_t xHigherPriorityTaskWoken;

	uint32_t read = GPIO_read_port(GPIO_C);

	if(GPIOC_16 == read)
	{
		GPIO_PortClearInterruptFlags(GPIOC, 1U << SWUP_PIN);
	}
	else if(GPIOC_8 == read)
	{
		GPIO_PortClearInterruptFlags(GPIOC, 1U << SWDOWN_PIN);
	}
	else if (GPIOC_4 == read)
	{
		GPIO_PortClearInterruptFlags(GPIOC, 1U << SWRIGHT_PIN);
	}
	else if (GPIOC_3 == read)
	{
		GPIO_PortClearInterruptFlags(GPIOC, 1U << SWLEFT_PIN);
	}

	xHigherPriorityTaskWoken = pdFALSE;

	portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
}

void init (void *pvParameters)
{
	  gpio_pin_config_t sw_config = {kGPIO_DigitalInput, INITIAL, };

	  CLOCK_EnableClock(kCLOCK_PortC);

	  const port_pin_config_t portc16_pinM9_config = {
			  kPORT_PullUp,
			  kPORT_FastSlewRate,
			  kPORT_PassiveFilterDisable,
			  kPORT_OpenDrainDisable,
			  kPORT_LowDriveStrength,
			  kPORT_MuxAsGpio,
			  kPORT_UnlockRegister
	  };

	  const port_pin_config_t portc8_pinM9_config = {
			  kPORT_PullUp,
			  kPORT_FastSlewRate,
			  kPORT_PassiveFilterDisable,
			  kPORT_OpenDrainDisable,
			  kPORT_LowDriveStrength,
			  kPORT_MuxAsGpio,
			  kPORT_UnlockRegister
	  };

	  const port_pin_config_t portc4_pinM9_config = {
			  kPORT_PullUp,
			  kPORT_FastSlewRate,
			  kPORT_PassiveFilterDisable,
			  kPORT_OpenDrainDisable,
			  kPORT_LowDriveStrength,
			  kPORT_MuxAsGpio,
			  kPORT_UnlockRegister
	  };

	  const port_pin_config_t portc3_pinM9_config = {
			  kPORT_PullUp,
			  kPORT_FastSlewRate,
			  kPORT_PassiveFilterDisable,
			  kPORT_OpenDrainDisable,
			  kPORT_LowDriveStrength,
			  kPORT_MuxAsGpio,
			  kPORT_UnlockRegister
	  };

	  PORT_SetPinConfig(PORTC, SWUP_PIN, &portc16_pinM9_config);
	  PORT_SetPinConfig(PORTC, SWDOWN_PIN, &portc8_pinM9_config);
	  PORT_SetPinConfig(PORTC, SWRIGHT_PIN, &portc4_pinM9_config);
	  PORT_SetPinConfig(PORTC, SWLEFT_PIN, &portc3_pinM9_config);

	  /* Init board hardware. */
	  BOARD_InitBootPins();
	  BOARD_InitBootClocks();
	  BOARD_InitBootPeripherals();
	  /* Init FSL debug console. */
	  BOARD_InitDebugConsole();

	  PORT_SetPinInterruptConfig(PORTC, SWUP_PIN, kPORT_InterruptFallingEdge);
	  PORT_SetPinInterruptConfig(PORTC, SWDOWN_PIN, kPORT_InterruptFallingEdge);
	  PORT_SetPinInterruptConfig(PORTC, SWRIGHT_PIN, kPORT_InterruptFallingEdge);
	  PORT_SetPinInterruptConfig(PORTC, SWLEFT_PIN, kPORT_InterruptFallingEdge);

	  NVIC_SetPriority(PORTC_IRQn, ISR_PRIOR);

	  EnableIRQ(PORTC_IRQn);

	  //GPIO_PinInit(BOARD_SW_GPIO, BOARD_SW_GPIO_PIN, &sw_config);
	  vTaskDelete(NULL);
}
