
#include "SPI.h"


void SPI_config_Screen(void)
{
	dspi_master_config_t masterConfig;

	CLOCK_SetSimSafeDivs();
	CLOCK_EnableClock(kCLOCK_PortD);
	/*PORT C is use for the memory pins*/
	CLOCK_EnableClock(kCLOCK_PortC);

	PORT_SetPinMux(PORTD, PIN0_IDX, kPORT_MuxAlt2);
	PORT_SetPinMux(PORTD, PIN1_IDX, kPORT_MuxAlt2);
	PORT_SetPinMux(PORTD, PIN2_IDX, kPORT_MuxAlt2);
//	PORT_SetPinMux(PORTD, PIN3_IDX, kPORT_MuxAlt2);

	/*This pin is use as output of memory (chip enable)*/

	PORT_SetPinMux(PORTC, PIN3_IDX, kPORT_MuxAlt2);

	masterConfig.whichCtar = kDSPI_Ctar0;
	masterConfig.ctarConfig.baudRate = TRANSFER_BAUDRATE_SCREEN;
	masterConfig.ctarConfig.bitsPerFrame = 8U;
	masterConfig.ctarConfig.cpol = kDSPI_ClockPolarityActiveHigh;
	masterConfig.ctarConfig.cpha = kDSPI_ClockPhaseFirstEdge;
	masterConfig.ctarConfig.direction = kDSPI_MsbFirst;
	masterConfig.ctarConfig.pcsToSckDelayInNanoSec = 1000000000
			/ TRANSFER_BAUDRATE_SCREEN;
	masterConfig.ctarConfig.lastSckToPcsDelayInNanoSec = 1000000000
			/ TRANSFER_BAUDRATE_SCREEN;
	masterConfig.ctarConfig.betweenTransferDelayInNanoSec = 1000000000
			/ TRANSFER_BAUDRATE_SCREEN;

	masterConfig.whichPcs = kDSPI_Pcs0;
	masterConfig.pcsActiveHighOrLow = kDSPI_Pcs0ActiveLow;

	masterConfig.enableContinuousSCK = false;
	masterConfig.enableRxFifoOverWrite = false;
	masterConfig.enableModifiedTimingFormat =false;
	masterConfig.samplePoint = kDSPI_SckToSin0Clock;

	uint32_t srcClock_Hz = CLOCK_GetFreq(DSPI0_CLK_SRC);
	static const IRQn_Type s_spiIrqs[] = SPI_IRQS;
	uint32_t instance = DSPI_GetInstance(SPI0);
	NVIC_SetPriority(s_spiIrqs[instance], 2);
	DSPI_MasterInit(SPI0, &masterConfig, srcClock_Hz);
}
void SPI_config_Memory(void)
{
	dspi_master_config_t masterConfig;
	//Config SPI for memory, baudrate is 2MHz
	masterConfig.whichCtar = kDSPI_Ctar1;
	masterConfig.ctarConfig.baudRate = TRANSFER_BAUDRATE_MEMORY;
	masterConfig.ctarConfig.bitsPerFrame = 8U;
	masterConfig.ctarConfig.cpol = kDSPI_ClockPolarityActiveLow;
	masterConfig.ctarConfig.cpha = kDSPI_ClockPhaseSecondEdge;
	masterConfig.ctarConfig.direction = kDSPI_MsbFirst;
	masterConfig.ctarConfig.pcsToSckDelayInNanoSec = 0
			;
	masterConfig.ctarConfig.lastSckToPcsDelayInNanoSec = 0
			;
	masterConfig.ctarConfig.betweenTransferDelayInNanoSec = 1000000000
			;

	masterConfig.whichPcs = kDSPI_Pcs1;
	masterConfig.pcsActiveHighOrLow = kDSPI_PcsActiveLow;

	masterConfig.enableContinuousSCK = false;
	masterConfig.enableRxFifoOverWrite = false;
	masterConfig.enableModifiedTimingFormat =false;
	masterConfig.samplePoint = kDSPI_SckToSin0Clock;

	uint32_t srcClock_Hz = CLOCK_GetFreq(DSPI0_CLK_SRC);
	//Init SPI with previous configuration
	DSPI_MasterInit(SPI0, &masterConfig, srcClock_Hz);
}
