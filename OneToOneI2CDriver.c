/*
 * OneToOneI2CDriver.c
 *
 *  Created on: Sep 24, 2016
 *      Author: Raz Aloni
 */


#include "OneToOneI2CDriver.h"
Uint16 I2CA_Status =0;
Uint16 I2CA_Write[10]={0};
Uint16 I2CA_Read[10]={0};
/* Ideal module clock frequency for I2C */
static const Uint16 IdealModClockFrqMHz = 12;

/*
 * <summary>
 * 	Initializes the GPIO for the I2C
 * </summary>
 */
static void InitI2CGpio();

/*
 * <summary>
 * 	Calculates and sets the ClockDivides for the I2C Module
 * </summary>
 * <param="sysClkMhz">System Clock Frequency in Mhz</param>
 * <param="I2CClkKHz">Desired I2C Clock Frequency in KHz</param>
 */
static inline void SetClockDivides(float32 sysClkMHz, float32 I2CClkKHz);

/*
 * <summary>
 * 	Initializes the I2C to run in Master Mode for a One-To-One connection
 * </summary>
 * <param="slaveAddress">Address of the slave device to write to</param>
 * <param="sysClkMhz">System Clock Frequency in Mhz</param>
 * <param="I2CClkKHz">Desired I2C Clock Frequency in KHz</param>
 */
void I2C_O2O_Master_Init(Uint16 slaveAddress, float32 sysClkMhz, float32 I2CClkKHz)
{
	// Init GPIO
	InitI2CGpio();

	EALLOW;

	// Enable Clock for I2C
	CpuSysRegs.PCLKCR9.bit.I2C_A = 1;

	// Put I2C into Reset Mode
	I2caRegs.I2CMDR.bit.IRS = 0;

	// Set Slave Address
	I2caRegs.I2CSAR.bit.SAR = slaveAddress;

	// Set Clocks
	SetClockDivides(sysClkMhz, I2CClkKHz);

	// Release from Reset Mode
	I2caRegs.I2CMDR.bit.IRS = 1;

	EDIS;
}


/*
 * <summary>
 * 	Sends bytes via I2C
 * </summary>
 * <param="values">Pointer to array of bytes to send</param>
 * <param-"length">Length of array</param>
 */
Uint16 I2C_O2O_SendBytes(Uint16 * const values, Uint16 length)
{
    EALLOW;
    if(I2caRegs.I2CSTR.bit.BB == 1)return I2CA_Busy;
	// Set to Master, Repeat Mode, TRX, FREE, Start
	I2caRegs.I2CMDR.all  = 0x66A0;

	while(I2caRegs.I2CMDR.bit.STT){}; //wait for start condition to be cleared

	// Write values to I2C
	for (Uint16 i = 0; i < length; i++)
	{
		// Wait if Transmit is not ready
		//if(I2caRegs.I2CSTR.bit.NACK)return I2CA_NACK;
		I2caRegs.I2CDXR.bit.DATA = values[i];
        while(!I2caRegs.I2CSTR.bit.XRDY);
		//for(Uint16 j = 0xffff; j>1 ; j--);
        //for(Uint16 j = 0xffff; j>1 ; j--);
        //for(Uint16 j = 0xffff; j>1 ; j--);


	}

	// Stop Bit
	I2caRegs.I2CMDR.bit.STP = 1;
	return I2CA_Succesful;
}
Uint16 I2C_O2O_ReadBytes(Uint16 * const values, Uint16 length)
{
    EALLOW;
    while(I2caRegs.I2CSTR.bit.BB);
    I2caRegs.I2CCNT= length;
    // Set to Master, Repeat Mode, FREE, Start
   do{ I2caRegs.I2CMDR.all = 0x6420;
       for(Uint16 j = 0x0fff; j>1 ; j--);
   }while(I2caRegs.I2CMDR.bit.STT&& !I2caRegs.I2CMDR.bit.MST);
    // Write values to I2C
    for (Uint16 i = 0; i < length; i++)
    {
        // Wait if Transmit is not ready
        while(!I2caRegs.I2CSTR.bit.RRDY);

        values[i] = I2caRegs.I2CDRR.bit.DATA;

      //  for(Uint16 j = 0xffff; j>1 ; j--);
       // for(Uint16 j = 0xffff; j>1 ; j--);
        //for(Uint16 j = 0xffff; j>1 ; j--);


    }
    I2caRegs.I2CMDR.bit.STP = 1;
    return I2CA_Succesful;
    // Stop Bit
}

/*
 * <summary>
 * 	Calculates and sets the ClockDivides for the I2C Module
 * </summary>
 * <param="sysClkMhz">System Clock Frequency in Mhz</param>
 * <param="I2CClkKHz">Desired I2C Clock Frequency in KHz</param>
 */
static inline void SetClockDivides(float32 sysClkMHz, float32 I2CClkKHz)
{
	/* Calculate Module Clock Frequency - Must be between 7-12 MHz
	 * Module Clock Frequency = sysClkMhz/(IPSC + 1)
	 */
	Uint16 IPSC = (Uint16)(sysClkMHz/IdealModClockFrqMHz);

	/* Calculate Divide Downs for SCL
	 * FreqMClk = sysClkMHz/((IPSC + 1)[(ICCL + d) + (ICCH + d)])
	 *
	 * Assume an even clock size -> ICCH == ICCL
	 * ICCL = ICCH = sysclkMHz/(2000 * I2CClkKHz * (IPSC + 1)) - d
	 */

	// Find value for d
	Uint16 d = 5;

	if (IPSC < 2)
	{
		d++;
		if (IPSC < 1)
		{
			d++;
		}
	}

	Uint16 ICCLH = (Uint16)(1000 * sysClkMHz/(2 * I2CClkKHz * (IPSC + 1)) - d);

	// Set values
	I2caRegs.I2CPSC.all = IPSC;
	I2caRegs.I2CCLKL = ICCLH;
	I2caRegs.I2CCLKH = ICCLH;
}


/*
 * <summary>
 * 	Initializes the GPIO for the I2C
 * </summary>
 */
static void InitI2CGpio()
{

   EALLOW;
/* Enable internal pull-up for the selected pins */
// Pull-ups can be enabled or disabled disabled by the user.
// This will enable the pullups for the specified pins.
// Comment out other unwanted lines.

	//GpioCtrlRegs.GPDPUD.bit.GPIO104 = 0;    // Enable pull-up for GPIO32 (SDAA)
	//GpioCtrlRegs.GPDPUD.bit.GPIO105 = 0;	   // Enable pull-up for GPIO33 (SCLA)

/* Set qualification for selected pins to asynch only */
// This will select asynch (no qualification) for the selected pins.
// Comment out other unwanted lines.

    GpioCtrlRegs.GPBQSEL1.bit.GPIO42 = 3;  // Asynch input GPIO32 (SDAA)
    GpioCtrlRegs.GPBQSEL1.bit.GPIO43 = 3;  // Asynch input GPIO33 (SCLA)

/* Configure SCI pins using GPIO regs*/
// This specifies which of the possible GPIO pins will be I2C functional pins.
// Comment out other unwanted lines.

	GpioCtrlRegs.GPBGMUX1.bit.GPIO42 = 1;   // Configure GPIO104 for SDAA operation
	GpioCtrlRegs.GPBGMUX1.bit.GPIO43 = 1;   // Configure GPIO105 for SCLA operation
	GpioCtrlRegs.GPBMUX1.bit.GPIO42 = 2;
	GpioCtrlRegs.GPBMUX1.bit.GPIO43 = 2;

    //EDIS;
}
