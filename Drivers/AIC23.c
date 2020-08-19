// TI File $Revision:  $
// Checkin $Date:  $
//###########################################################################
//
// FILE:	AIC23.c
//
// TITLE:	TLV320AIC23B Driver Functions
//
// DESCRIPTION: 
//
// This file includes functions for initializing the AIC23 codec. You may
// modify/add to these functions to suit your application if desired.
//
//###########################################################################
// $TI Release:   $
// $Release Date:   $
//###########################################################################


#include "AIC23.h"
Uint16 softpowerdown (void) {          // Power down control 
    return (POWDCTL|(AIC23OUT|AIC23DAC|AIC23ADC|  // Power down: In/Out/DAC/ADC/Mic 
                       AIC23MIC|AIC23LINE));       // Leave AIC23 device, osc, clks on
}

Uint16 fullpowerup (void) {            // Power up all modules
    return (POWDCTL|0x0000);   
}    
Uint16 nomicpowerup (void) {           // Power up all modules except microphone
    return (POWDCTL|AIC23MIC);
}    
Uint16 linput_volctl (Uint16 volume) { // Left Line Input Volume Control 
    return (LRS|LLINVCTL|volume);
}

Uint16 rinput_volctl (Uint16 volume) { // Right Line Input Volume Control 
    return (RLS|RLINVCTL|volume);
}

Uint16 lhp_volctl (Uint16 volume) {    // Left Headphone Volume Control
    return (LRS|LHVCTL|LZC|volume);
}

Uint16 rhp_volctl (Uint16 volume) {    // Right Headphone Volume Control
    return (RLS|RHVCTL|RZC|volume);
}

Uint16 nomicaaudpath (void) {          // Analog audio path control
    return 	(AAUDCTL|DAC|MICMUTE);     // Turn on DAC, mute mic
}

Uint16 aaudpath (void){
    return (AAUDCTL|DAC|MICINSEL|MICBOOST); // Turn on DAC, mic
}

Uint16 digaudiopath (void) {           // Digital audio path control
//    return (DAUDCTL|0x0006);           // DAC mute,  ADC HP filter all disabled, de-emph,= 48K
    return (DAUDCTL|0x0000);           // DAC mute,  de-emph ADC HP filter all disabled

}

Uint16 DSPdigaudinterface (void) {     // Digital audio interface format
    return (DAUDINTF|(AIC23MS|         // AIC23 master mode, DSP mode,32-bit data,
            FORDSP|IWL16B|LRPZRO));         // LRP=1
}

Uint16 I2Sdigaudinterface (void) {     // Digital audio interface format
    return (DAUDINTF|(AIC23MS|         // AIC23 master mode, I2S mode,32-bit data,
            FORI2S|IWL32B|LRPZRO));         // LRP=0 right channel high
}


Uint16 USBsampleratecontrol (Uint16 ratecontrol)  {
    return (SMPLRCTL|(USB|ratecontrol)); // USB clock - 48 kHz sample rate in USB mode
}

Uint16 CLKsampleratecontrol (Uint16 ratecontrol)  {
    return (SMPLRCTL|ratecontrol);      // external clock - 48 kHz sample rate in external clock (12.288 MHz) mode
}


Uint16 digact (void) {                 // Digital Interface Activation       
    return (DIGINTFACT|ACT);           // Activate
} 

Uint16 reset (void) {                  // Reset AIC23
    return (RESETREG|RES);
}
void SmallDelay(){
    for( long  i = 0; i < 500000; i++);
}
//void InitAIC23( )
//{
//    SmallDelay();
//    uint16_t command;
//    command = reset();
//
//    BitBangedCodecSpiTransmit (command);
//    SmallDelay();
//    command = softpowerdown();       // Power down everything except device and clocks
//    BitBangedCodecSpiTransmit (command);
//    SmallDelay();
//    command = linput_volctl(LIV);    // Unmute left line input and maintain default volume
//    BitBangedCodecSpiTransmit (command);
//    SmallDelay();
//    command = rinput_volctl(RIV);    // Unmute right line input and maintain default volume
//    BitBangedCodecSpiTransmit (command);
//    SmallDelay();
//    command = lhp_volctl(LHV);       // Left headphone volume control
//    BitBangedCodecSpiTransmit (command);
//    SmallDelay();
//    command = rhp_volctl(RHV);       // Right headphone volume control
//    BitBangedCodecSpiTransmit (command);
//    SmallDelay();
//    command = nomicaaudpath();      // Turn on DAC, mute mic
//    BitBangedCodecSpiTransmit (command);
//    SmallDelay();
//    command = digaudiopath();       // Disable DAC mute, add de-emph
//    BitBangedCodecSpiTransmit (command);
//    SmallDelay();
//
//    // I2S
//    command = DSPdigaudinterface(); // AIC23 master mode, I2S mode,32-bit data, LRP=1 to match with XDATADLY=1
//    BitBangedCodecSpiTransmit (command);
//    SmallDelay();
//    command = CLKsampleratecontrol (SR48);
//    BitBangedCodecSpiTransmit (command);
//    SmallDelay();
//
//    command = digact();             // Activate digital interface
//    BitBangedCodecSpiTransmit (command);
//    SmallDelay();
//    command = nomicpowerup();      // Turn everything on except Mic.
//    BitBangedCodecSpiTransmit (command);
//
//}
void InitAIC23( )
{
    SmallDelay();
    uint16_t command;
    command = reset();
    SpiTransmit(command);
    SmallDelay();
    command = softpowerdown();       // Power down everything except device and clocks
    SpiTransmit(command);
    SmallDelay();
    command = linput_volctl(LIV);    // Unmute left line input and maintain default volume
    SpiTransmit(command);
    SmallDelay();
    command = rinput_volctl(RIV);    // Unmute right line input and maintain default volume
    SpiTransmit(command);
    SmallDelay();
    command = lhp_volctl(LHV);       // Left headphone volume control
    SpiTransmit (command);
    SmallDelay();
    command = rhp_volctl(RHV);       // Right headphone volume control
    SpiTransmit(command);
    SmallDelay();
    command = nomicaaudpath();      // Turn on DAC, mute mic
    SpiTransmit(command);
    SmallDelay();
    command = digaudiopath();       // Disable DAC mute, add de-emph
    SpiTransmit (command);
    SmallDelay();

    // I2S
    command = DSPdigaudinterface(); // AIC23 master mode, I2S mode,32-bit data, LRP=1 to match with XDATADLY=1
    SpiTransmit (command);
    SmallDelay();
    command = CLKsampleratecontrol (SR48);
    SpiTransmit (command);
    SmallDelay();

    command = digact();             // Activate digital interface
    SpiTransmit (command);
    SmallDelay();
    command = nomicpowerup();      // Turn everything on except Mic.
    SpiTransmit (command);

}
void Init_McBSPbInterrupt(void(*Msbr_isr)){
    PieVectTable.MCBSPB_RX_INT = Msbr_isr;
    PieCtrlRegs.PIEIER6.bit.INTx7 = 1;
    IER |= M_INT6;

}

void InitMcBSPb()
{
    /* Init McBSPb GPIO Pins */

    //modify the GPxMUX, GPxGMUX, GPxQSEL
    //all pins should be set to asynch qualification

    /*
     * MDXB -> GPIO24
     * New MDXB  -> GPIO84
     * MDRB -> GPIO25
     * New MDRB -> GPIO13
     * MCLKRB -> GPIO60
     * New MCLKRB -> GPIO60
     * MCLKXB -> GPIO26
     * New MCLKXB -> GPIO26
     * MFSRB -> GPIO61
     * New MFSRB -> GPIO1
     * MFSXB -> GPIO27
     * New MFSXB -> GPIO27
     */
    EALLOW;

    // MDXB -> GPIO24 (GPIOA)
//    GpioCtrlRegs.GPCGMUX2.bit.GPIO84 = 1;
//    GpioCtrlRegs.GPCMUX2.bit.GPIO84 = 2;
//    GpioCtrlRegs.GPCQSEL2.bit.GPIO84 = 3;
//    GpioCtrlRegs.GPCDIR.bit.GPIO84 = 1;
    GpioCtrlRegs.GPAGMUX2.bit.GPIO24 = 0;
    GpioCtrlRegs.GPAMUX2.bit.GPIO24 = 3;
    GpioCtrlRegs.GPAQSEL2.bit.GPIO24 = 3;

    // MDRB -> GPIO25 (GPIOA)

    GpioCtrlRegs.GPAGMUX1.bit.GPIO13 = 0;
    GpioCtrlRegs.GPAMUX1.bit.GPIO13 = 3;
    GpioCtrlRegs.GPAQSEL1.bit.GPIO13 = 3;


//    GpioCtrlRegs.GPAGMUX2.bit.GPIO25 = 0;
//    GpioCtrlRegs.GPAMUX2.bit.GPIO25 = 3;
//    GpioCtrlRegs.GPAQSEL2.bit.GPIO25 = 3;

    // MFSRB -> GPIO61 (GPIOB)

//    GpioCtrlRegs.GPBGMUX2.bit.GPIO61 = 0;
//    GpioCtrlRegs.GPBMUX2.bit.GPIO61 = 1;
//    GpioCtrlRegs.GPBQSEL2.bit.GPIO61 = 3;

    GpioCtrlRegs.GPAGMUX1.bit.GPIO1 = 0;
    GpioCtrlRegs.GPAMUX1.bit.GPIO1 = 3;
    GpioCtrlRegs.GPAQSEL1.bit.GPIO1 = 3;
    // MFSXB -> GPIO27 (GPIOA)

    GpioCtrlRegs.GPAGMUX2.bit.GPIO27 = 0;
    GpioCtrlRegs.GPAMUX2.bit.GPIO27 = 3;
    GpioCtrlRegs.GPAQSEL2.bit.GPIO27 = 3;

    // MCLKRB -> GPIO60 (GPIOB)

//    GpioCtrlRegs.GPBGMUX2.bit.GPIO60 = 0;
//    GpioCtrlRegs.GPBMUX2.bit.GPIO60 = 1;
//    GpioCtrlRegs.GPBQSEL2.bit.GPIO60 = 3;
    GpioCtrlRegs.GPAGMUX1.bit.GPIO3 = 0;
    GpioCtrlRegs.GPAMUX1.bit.GPIO3 = 3;
    GpioCtrlRegs.GPAQSEL1.bit.GPIO3 = 3;

    // MCLKXB -> GPIO26 (GPIOA)
    // New MCLKXB -> GPIO14(GPIOA)

    GpioCtrlRegs.GPAGMUX1.bit.GPIO14= 0;
    GpioCtrlRegs.GPAMUX1.bit.GPIO14 = 3;
    GpioCtrlRegs.GPAQSEL1.bit.GPIO14 = 3;
    EDIS;
        EALLOW;
        McbspbRegs.SPCR2.all = 0; // Reset FS generator, sample rate generator & transmitter
        McbspbRegs.SPCR1.all = 0; // Reset Receiver, Right justify word
        McbspbRegs.SPCR1.bit.RJUST = 2; // left-justify word in DRR and zero-fill LSBs
        McbspbRegs.MFFINT.all=0x0; // Disable all interrupts
        McbspbRegs.SPCR1.bit.RINTM = 0; // McBSP interrupt flag - RRDY
        McbspbRegs.SPCR2.bit.XINTM = 0; // McBSP interrupt flag - XRDY
        // Clear Receive Control Registers
        McbspbRegs.RCR2.all = 0x0;
        McbspbRegs.RCR1.all = 0x0;
        // Clear Transmit Control Registers
        McbspbRegs.XCR2.all = 0x0;
        McbspbRegs.XCR1.all = 0x0;
        // Set Receive/Transmit to 32-bit operation
        McbspbRegs.RCR2.bit.RWDLEN2 = 5;
        McbspbRegs.RCR1.bit.RWDLEN1 = 5;
        McbspbRegs.XCR2.bit.XWDLEN2 = 5;
        McbspbRegs.XCR1.bit.XWDLEN1 = 5;
        McbspbRegs.RCR2.bit.RPHASE = 0; // Dual-phase frame for receive
        McbspbRegs.RCR1.bit.RFRLEN1 = 0; // Receive frame length = 1 word in phase 1
        McbspbRegs.RCR2.bit.RFRLEN2 = 0; // Receive frame length = 1 word in phase 2
        McbspbRegs.XCR2.bit.XPHASE = 0; // Dual-phase frame for transmit
        McbspbRegs.XCR1.bit.XFRLEN1 = 0; // Transmit frame length = 1 word in phase 1
        McbspbRegs.XCR2.bit.XFRLEN2 = 0; // Transmit frame length = 1 word in phase 2
        // I2S mode: R/XDATDLY = 1 always
        McbspbRegs.RCR2.bit.RDATDLY = 0;
        McbspbRegs.XCR2.bit.XDATDLY = 0;
        // Frame Width = 1 CLKG period, CLKGDV must be 1 as slave
        McbspbRegs.SRGR1.all = 0x0001;
        McbspbRegs.PCR.all=0x0000;
        // Transmit frame synchronization is supplied by an external source via the FSX pin
        McbspbRegs.PCR.bit.FSXM = 0;
        // Receive frame synchronization is supplied by an external source via the FSR pin
        McbspbRegs.PCR.bit.FSRM = 0;
        // Select sample rate generator to be signal on MCLKR pin
        McbspbRegs.PCR.bit.SCLKME = 1;
        McbspbRegs.SRGR2.bit.CLKSM = 0;
        // Receive frame-synchronization pulses are active low - (L-channel first)
        McbspbRegs.PCR.bit.FSRP = 0;
        // Transmit frame-synchronization pulses are active low - (L-channel first)
        McbspbRegs.PCR.bit.FSXP = 0;
        // Receive data is sampled on the rising edge of MCLKR
        McbspbRegs.PCR.bit.CLKRP = 1;



        // Transmit data is sampled on the rising edge of CLKX
        McbspbRegs.PCR.bit.CLKXP = 1;
        McbspbRegs.SPCR1.bit.RINTM=2;


        // The transmitter gets its clock signal from MCLKX
        McbspbRegs.PCR.bit.CLKXM = 0;
        // The receiver gets its clock signal from MCLKR
        McbspbRegs.PCR.bit.CLKRM = 0;
        // Enable Receive Interrupt
        McbspbRegs.MFFINT.bit.RINT = 1;
        // Ignore unexpected frame sync
        //McbspbRegs.XCR2.bit.XFIG = 1;
        McbspbRegs.SPCR2.all |=0x00C0; // Frame sync & sample rate generators pulled out of reset
        SmallDelay();
        McbspbRegs.SPCR2.bit.XRST=1; // Enable Transmitter
        McbspbRegs.SPCR1.bit.RRST=1; // Enable Receiver
        EDIS;
//    /* Init McBSPb for I2S mode */
//    EALLOW;
//    McbspbRegs.SPCR2.all = 0; // Reset FS generator, sample rate generator & transmitter
//    McbspbRegs.SPCR1.all = 0; // Reset Receiver, Right justify word
//    McbspbRegs.SPCR1.bit.RJUST = 2; // left-justify word in DRR and zero-fill LSBs
//    McbspbRegs.MFFINT.all=0x0; // Disable all interrupts
//    McbspbRegs.SPCR1.bit.RINTM = 0; // McBSP interrupt flag - RRDY
//    McbspbRegs.SPCR2.bit.XINTM = 0; // McBSP interrupt flag - XRDY
//    // Clear Receive Control Registers
//    McbspbRegs.RCR2.all = 0x0;
//    McbspbRegs.RCR1.all = 0x0;
//    // Clear Transmit Control Registers
//    McbspbRegs.XCR2.all = 0x0;
//    McbspbRegs.XCR1.all = 0x0;
//    // Set Receive/Transmit to 32-bit operation
//    McbspbRegs.RCR2.bit.RWDLEN2 = 5;
//    McbspbRegs.RCR1.bit.RWDLEN1 = 5;
//    McbspbRegs.XCR2.bit.XWDLEN2 = 5;
//    McbspbRegs.XCR1.bit.XWDLEN1 = 5;
//    McbspbRegs.RCR2.bit.RPHASE = 1; // Dual-phase frame for receive
//    McbspbRegs.RCR1.bit.RFRLEN1 = 0; // Receive frame length = 1 word in phase 1
//    McbspbRegs.RCR2.bit.RFRLEN2 = 0; // Receive frame length = 1 word in phase 2
//    McbspbRegs.XCR2.bit.XPHASE = 1; // Dual-phase frame for transmit
//    McbspbRegs.XCR1.bit.XFRLEN1 = 0; // Transmit frame length = 1 word in phase 1
//    McbspbRegs.XCR2.bit.XFRLEN2 = 0; // Transmit frame length = 1 word in phase 2
//    // I2S mode: R/XDATDLY = 1 always
//    McbspbRegs.RCR2.bit.RDATDLY = 1;
//    McbspbRegs.XCR2.bit.XDATDLY = 1;
//    // Frame Width = 1 CLKG period, CLKGDV must be 1 as slave
//    McbspbRegs.SRGR1.all = 0x0001;
//    McbspbRegs.PCR.all=0x0000;
//    // Transmit frame synchronization is supplied by an external source via the FSX pin
//    McbspbRegs.PCR.bit.FSXM = 0;
//    // Receive frame synchronization is supplied by an external source via the FSR pin
//    McbspbRegs.PCR.bit.FSRM = 0;
//    // Select sample rate generator to be signal on MCLKR pin
//    McbspbRegs.PCR.bit.SCLKME = 1;
//    McbspbRegs.SRGR2.bit.CLKSM = 0;
//    // Receive frame-synchronization pulses are active low - (L-channel first)
//    McbspbRegs.PCR.bit.FSRP = 1;
//    // Transmit frame-synchronization pulses are active low - (L-channel first)
//    McbspbRegs.PCR.bit.FSXP = 1;
//    // Receive data is sampled on the rising edge of MCLKR
//    McbspbRegs.PCR.bit.CLKRP = 1;
//
//
//
//    // Transmit data is sampled on the rising edge of CLKX
//    McbspbRegs.PCR.bit.CLKXP = 1;
//
//
//
//    // The transmitter gets its clock signal from MCLKX
//    McbspbRegs.PCR.bit.CLKXM = 0;
//    // The receiver gets its clock signal from MCLKR
//    McbspbRegs.PCR.bit.CLKRM = 0;
//    // Enable Receive Interrupt
//    McbspbRegs.MFFINT.bit.RINT = 1;
//    // Ignore unexpected frame sync
//    //McbspbRegs.XCR2.bit.XFIG = 1;
//    McbspbRegs.SPCR2.all |=0x00C0; // Frame sync & sample rate generators pulled out of reset
//    SmallDelay();
//    McbspbRegs.SPCR2.bit.XRST=1; // Enable Transmitter
//    McbspbRegs.SPCR1.bit.RRST=1; // Enable Receiver
//    EDIS;
}

//void InitMcBSPb()
//{
//    /* Init McBSPb GPIO Pins */
//
//    //modify the GPxMUX, GPxGMUX, GPxQSEL
//    //all pins should be set to asynch qualification
//
//    /*
//     * MDXB -> GPIO24
//     * New MDXB  -> GPIO84
//     * MDRB -> GPIO25
//     * New MDRB -> GPIO13
//     * MCLKRB -> GPIO60
//     * New MCLKRB -> GPIO60
//     * MCLKXB -> GPIO26
//     * New MCLKXB -> GPIO26
//     * MFSRB -> GPIO61
//     * New MFSRB -> GPIO1
//     * MFSXB -> GPIO27
//     * New MFSXB -> GPIO27
//     */
//    EALLOW;
//
//    // MDXB -> GPIO24 (GPIOA)
//    GpioCtrlRegs.GPCGMUX2.bit.GPIO84 = 1;
//    GpioCtrlRegs.GPCMUX2.bit.GPIO84 = 2;
//    GpioCtrlRegs.GPCQSEL2.bit.GPIO84 = 3;
//
////    GpioCtrlRegs.GPAGMUX2.bit.GPIO24 = 0;
////    GpioCtrlRegs.GPAMUX2.bit.GPIO24 = 3;
////    GpioCtrlRegs.GPAQSEL2.bit.GPIO24 = 3;
//
//    // MDRB -> GPIO25 (GPIOA)
//
//    GpioCtrlRegs.GPAGMUX1.bit.GPIO13 = 0;
//    GpioCtrlRegs.GPAMUX1.bit.GPIO13 = 3;
//    GpioCtrlRegs.GPAQSEL1.bit.GPIO13 = 3;
//
//
////    GpioCtrlRegs.GPAGMUX2.bit.GPIO25 = 0;
////    GpioCtrlRegs.GPAMUX2.bit.GPIO25 = 3;
////    GpioCtrlRegs.GPAQSEL2.bit.GPIO25 = 3;
//
//    // MFSRB -> GPIO61 (GPIOB)
//
////    GpioCtrlRegs.GPBGMUX2.bit.GPIO61 = 0;
////    GpioCtrlRegs.GPBMUX2.bit.GPIO61 = 1;
////    GpioCtrlRegs.GPBQSEL2.bit.GPIO61 = 3;
//
//    GpioCtrlRegs.GPAGMUX1.bit.GPIO1 = 0;
//    GpioCtrlRegs.GPAMUX1.bit.GPIO1 = 3;
//    GpioCtrlRegs.GPAQSEL1.bit.GPIO1 = 3;
//    // MFSXB -> GPIO27 (GPIOA)
//
//    GpioCtrlRegs.GPAGMUX2.bit.GPIO27 = 0;
//    GpioCtrlRegs.GPAMUX2.bit.GPIO27 = 3;
//    GpioCtrlRegs.GPAQSEL2.bit.GPIO27 = 3;
//
//    // MCLKRB -> GPIO60 (GPIOB)
//
////    GpioCtrlRegs.GPBGMUX2.bit.GPIO60 = 0;
////    GpioCtrlRegs.GPBMUX2.bit.GPIO60 = 1;
////    GpioCtrlRegs.GPBQSEL2.bit.GPIO60 = 3;
//    GpioCtrlRegs.GPAGMUX1.bit.GPIO3 = 0;
//    GpioCtrlRegs.GPAMUX1.bit.GPIO3 = 3;
//    GpioCtrlRegs.GPAQSEL1.bit.GPIO3 = 3;
//
//    // MCLKXB -> GPIO26 (GPIOA)
//
//    GpioCtrlRegs.GPAGMUX2.bit.GPIO26 = 0;
//    GpioCtrlRegs.GPAMUX2.bit.GPIO26 = 3;
//    GpioCtrlRegs.GPAQSEL2.bit.GPIO26 = 3;
//    EDIS;
//
//    /* Init McBSPb for I2S mode */
//    EALLOW;
//    McbspbRegs.SPCR2.all = 0; // Reset FS generator, sample rate generator & transmitter
//    McbspbRegs.SPCR1.all = 0; // Reset Receiver, Right justify word
//    McbspbRegs.SPCR1.bit.RJUST = 2; // left-justify word in DRR and zero-fill LSBs
//    McbspbRegs.MFFINT.all=0x0; // Disable all interrupts
//    McbspbRegs.SPCR1.bit.RINTM = 0; // McBSP interrupt flag - RRDY
//    McbspbRegs.SPCR2.bit.XINTM = 0; // McBSP interrupt flag - XRDY
//    // Clear Receive Control Registers
//    McbspbRegs.RCR2.all = 0x0;
//    McbspbRegs.RCR1.all = 0x0;
//    // Clear Transmit Control Registers
//    McbspbRegs.XCR2.all = 0x0;
//    McbspbRegs.XCR1.all = 0x0;
//    // Set Receive/Transmit to 32-bit operation
//    McbspbRegs.RCR2.bit.RWDLEN2 = 5;
//    McbspbRegs.RCR1.bit.RWDLEN1 = 5;
//    McbspbRegs.XCR2.bit.XWDLEN2 = 5;
//    McbspbRegs.XCR1.bit.XWDLEN1 = 5;
//    McbspbRegs.RCR2.bit.RPHASE = 0; // Dual-phase frame for receive
//    McbspbRegs.RCR1.bit.RFRLEN1 = 0; // Receive frame length = 1 word in phase 1
//    McbspbRegs.RCR2.bit.RFRLEN2 = 0; // Receive frame length = 1 word in phase 2
//    McbspbRegs.XCR2.bit.XPHASE = 0; // Dual-phase frame for transmit
//    McbspbRegs.XCR1.bit.XFRLEN1 = 0; // Transmit frame length = 1 word in phase 1
//    McbspbRegs.XCR2.bit.XFRLEN2 = 0; // Transmit frame length = 1 word in phase 2
//    // I2S mode: R/XDATDLY = 1 always
//    McbspbRegs.RCR2.bit.RDATDLY = 0;
//    McbspbRegs.XCR2.bit.XDATDLY = 0;
//    // Frame Width = 1 CLKG period, CLKGDV must be 1 as slave
//    McbspbRegs.SRGR1.all = 0x0001;
//    McbspbRegs.PCR.all=0x0000;
//    // Transmit frame synchronization is supplied by an external source via the FSX pin
//    McbspbRegs.PCR.bit.FSXM = 0;
//    // Receive frame synchronization is supplied by an external source via the FSR pin
//    McbspbRegs.PCR.bit.FSRM = 0;
//    // Select sample rate generator to be signal on MCLKR pin
//    McbspbRegs.PCR.bit.SCLKME = 1;
//    McbspbRegs.SRGR2.bit.CLKSM = 0;
//    // Receive frame-synchronization pulses are active low - (L-channel first)
//    McbspbRegs.PCR.bit.FSRP = 0;
//    // Transmit frame-synchronization pulses are active low - (L-channel first)
//    McbspbRegs.PCR.bit.FSXP = 0;
//    // Receive data is sampled on the rising edge of MCLKR
//    McbspbRegs.PCR.bit.CLKRP = 1;
//
//
//
//    // Transmit data is sampled on the rising edge of CLKX
//    McbspbRegs.PCR.bit.CLKXP = 1;
//    McbspbRegs.SPCR1.bit.RINTM=2;
//
//
//    // The transmitter gets its clock signal from MCLKX
//    McbspbRegs.PCR.bit.CLKXM = 0;
//    // The receiver gets its clock signal from MCLKR
//    McbspbRegs.PCR.bit.CLKRM = 0;
//    // Enable Receive Interrupt
//    McbspbRegs.MFFINT.bit.RINT = 1;
//    // Ignore unexpected frame sync
//    //McbspbRegs.XCR2.bit.XFIG = 1;
//    McbspbRegs.SPCR2.all |=0x00C0; // Frame sync & sample rate generators pulled out of reset
//    SmallDelay();
//    McbspbRegs.SPCR2.bit.XRST=1; // Enable Transmitter
//    McbspbRegs.SPCR1.bit.RRST=1; // Enable Receiver
//    EDIS;
//}

void InitBigBangedCodecSPI(){
    /*
     * GPIO19 - CS
     * GPIO18 - CLK
     * GPIO58 - SPISIMOA
     */

    EALLOW;

    //enable pullups
    GpioCtrlRegs.GPAPUD.bit.GPIO19 = 0;
    GpioCtrlRegs.GPAPUD.bit.GPIO18 = 0;
    GpioCtrlRegs.GPAPUD.bit.GPIO16 = 0;

    GpioCtrlRegs.GPAGMUX2.bit.GPIO19 = 0;
    GpioCtrlRegs.GPAGMUX2.bit.GPIO18 = 0;
    GpioCtrlRegs.GPAGMUX2.bit.GPIO16 = 0;

    GpioCtrlRegs.GPAMUX2.bit.GPIO19 = 0;
    GpioCtrlRegs.GPAMUX2.bit.GPIO18 = 0;
    GpioCtrlRegs.GPAMUX2.bit.GPIO16 = 0;

    GpioCtrlRegs.GPADIR.bit.GPIO19 = 1;
    GpioCtrlRegs.GPADIR.bit.GPIO18 = 1;
    GpioCtrlRegs.GPADIR.bit.GPIO16 = 1;

    GpioDataRegs.GPASET.bit.GPIO19 = 1;
    GpioDataRegs.GPACLEAR.bit.GPIO18 = 1;



    EDIS;
}

void BitBangedCodecSpiTransmit(Uint16 data){
    CodecSPI_CS_LOW;
    /* Transmit 16 bit data */
    //send data out MSB first
    for(Uint16 i = 16; i > 0; i--){
        GpioDataRegs.GPADAT.bit.GPIO16 = (data >> (i-1)) & 1;
        CodecSPI_CLK_PULS;
    }

    CodecSPI_CS_HIGH;
}


void InitSPIA()
{
    /* Init GPIO pins for SPIA */

    //enable pullups for each pin
    //set to asynch qualification
    //configure each mux

    //SPISTEA -> GPIO19
    //SPISIMOA -> GPIO58
    //New SPISIMOA -> GPIO16

    //SPICLKA -> GPIO18

    EALLOW;
    //enable pullups
    GpioCtrlRegs.GPAPUD.bit.GPIO19 = 0;
    GpioCtrlRegs.GPAPUD.bit.GPIO18 = 0;
    //GpioCtrlRegs.GPAPUD.bit.GPIO58 = 0;
    GpioCtrlRegs.GPAPUD.bit.GPIO16 = 0;

    GpioCtrlRegs.GPAGMUX2.bit.GPIO19 = 0;
    GpioCtrlRegs.GPAGMUX2.bit.GPIO18 = 0;
   // GpioCtrlRegs.GPBGMUX2.bit.GPIO58 = 3;
    GpioCtrlRegs.GPAGMUX2.bit.GPIO16 = 0;

    GpioCtrlRegs.GPAMUX2.bit.GPIO19 = 1;
    GpioCtrlRegs.GPAMUX2.bit.GPIO18 = 1;
    //GpioCtrlRegs.GPBMUX2.bit.GPIO58 = 3;
    GpioCtrlRegs.GPAMUX2.bit.GPIO16 = 1;

    //asynch qual
    GpioCtrlRegs.GPAQSEL2.bit.GPIO19 = 3;
    GpioCtrlRegs.GPAQSEL2.bit.GPIO18 = 3;
    //GpioCtrlRegs.GPBQSEL2.bit.GPIO58 = 3;
    GpioCtrlRegs.GPAQSEL2.bit.GPIO16 = 3;

    EDIS;

    /* Init SPI peripheral */
    SpiaRegs.SPICCR.all = 0x5F; //CLKPOL = 0, SOMI = SIMO (loopback), 16 bit characters
    SpiaRegs.SPICTL.all = 0x06; //master mode, enable transmissions
    SpiaRegs.SPIBRR.all = 50; //gives baud rate of approx 850 kHz

    SpiaRegs.SPICCR.bit.SPISWRESET = 1;
    SpiaRegs.SPIPRI.bit.FREE = 1;

}
void SpiTransmit(uint16_t data)
{
    /* Transmit 16 bit data */
    SpiaRegs.SPIDAT = data; //send data to SPI register
    while(SpiaRegs.SPISTS.bit.INT_FLAG == 0); //wait until the data has been sent
    Uint16 dummyLoad = SpiaRegs.SPIRXBUF; //reset flag
}
Uint16 lowerVol( Uint16 current, Uint16 amount)
{
    Uint16 volume = current;
    if((current) == (Uint16)volumeMinI)
    {
        volume = volumeMinI;
    }else{
        volume-=amount;
    }
    SpiTransmit(linput_volctl(volume));
    SpiTransmit(rinput_volctl(volume));
    return volume&volumeMaxI;
}
Uint16 raiseVol( Uint16 current, Uint16 amount)
{
    Uint16 volume = current;
    if((current+amount) >= (Uint16)(volumeMaxI))
    {
        volume = volumeMaxI;
    }else{
        volume += amount;
    }
    SpiTransmit(linput_volctl(volume));
    SpiTransmit(rinput_volctl(volume));
    return volume&volumeMaxI;
}

