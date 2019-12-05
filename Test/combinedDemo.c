

/**
 * main.c
 */
/*Descriptions
 * This codes use given driver to write name and course code on the lcd
 */
#include<F28x_Project.h>
#include <stdio.h>
#include <stdlib.h>
#include "FPU.h"
#include "InitAIC23.h"
#include "IODriver.h"
#include "TFTLCD_Driver.h"
#include "TouchScreenDriver.h"
#include "DisplayLibrary.h"
#include "AudioEffectsLite.h"
#include "ADCDriver.h"
#define     left        0
#define     right       1

Uint16 channel1,channel2;

Uint16  channel = left;
Uint16  interruptStore=0;
Uint32  leftChannel = 0;
Uint32  rightChannel =0;
__interrupt void cpu_timer0_isr(void);
__interrupt void adca1_isr(void);
__interrupt void MSBR_isr(void);
__interrupt void BUT1_isr(void);
__interrupt void BUT2_isr(void);
__interrupt void BUT3_isr(void);
#define cosSize   0000
Uint16 ColorButtonOn = genColor(0,0,0xff);
Uint16 ColorButtonOff= genColor(0xff,0,0);
Uint16 ColorText = genColor(0,0xff,0);
Uint16 Buttons[3]={0,0,0};
Uint16  index = 0;
Uint16 adcSignal = 0;
Uint16 testCheck = 1;
Uint16 color[2];
Uint16 DrawCheck=0;

Uint16 temp=0;
float conversion;
__interrupt void adca1_isr(void);

__interrupt void MCBSP_isr(void);

int main(void)

{
    Button testButton = {
         .color={ColorButtonOff,ColorButtonOn},
         .colorText={ColorText,ColorText},
         .height=80,
         .width = 50,
         .x = 100,
         .y = 150,
         .string = "ADSR",
         .state = Button_Off
    };

    InitSysCtrl();
    InitGpio();

    InitSPIA();
    InitMcBSPb();
    InitAIC23();
    Init_ADC();
    Init_LCDPins();
    startLCD();
    color[0] = genColor(0xff, 0xff, 0xff);
    color[1] = genColor(0, 0, 0);
    fillScreen(color[1]);
    float lastconversion = 0.0,difference=0.0;

    EALLOW;
    DINT;
    InitPieCtrl();
    IER = 0x0000;
    IFR = 0x0000;
    InitPieVectTable();
    EALLOW;
    PieVectTable.ADCA1_INT = &adca1_isr; //function for ADCA interrupt
    PieVectTable.TIMER0_INT = &cpu_timer0_isr;
    PieVectTable.MCBSPB_RX_INT = &MCBSP_isr;
    IER |= M_INT6; //Enable group 1 interrupts
   //Enable group 1 interrupts
    EDIS;
    InitCpuTimers();
    ConfigCpuTimer(&CpuTimer1, 200, 1000000);
    ConfigCpuTimer(&CpuTimer0, 200, 100000);
    //EINT;  // Enable Global interrupt INTM
    CpuTimer0Regs.TCR.all = 0x4001;
    PieCtrlRegs.PIEIER6.bit.INTx7 = 1;
    PieCtrlRegs.PIEIER1.bit.INTx7 = 1;
    PieCtrlRegs.PIEIER1.bit.INTx1 = 1;

    IER |= M_INT1;
    EINT;  // Enable Global interrupt INTM
    ERTM;  // Enable Global realtime interrupt DBGM
    //Init_LCDPins();
    //startLCD();
    SetupADCTimer1();

 //   Uint16 ID=getID();

    Text Result = { .string = "pass", .color =color[0], .x =150, .y=100};

    DELAY_US(10000);
    TS_init(40);
    Init_ADSRPins();
    Uint16 lastState;
    setReverbDecay(0.3);

    //drawButton(testButton,te);
    while(1){
        //fillScreen(color[1]);
        //drawText(Result);

       // drawChar('A', color[0], capitalLetter10, 100, 100);
        //test(100, 100, 20)
       // drawThousandsFloat(3.25,10TS_Position0,100, color[0]);
       ADSRGate = 1;
       TS_checkInteraction();
       checkButtonPress(&testButton);
       ADSRGate = 0;
      // ADSRGate = testButton.state>>1;
       if(DrawCheck){
       //    ADSRGate = 1;
           drawButton(testButton, testButton.state>>1);
       //    ADSRGate = 0;

           DrawCheck=0;

       }
       if(AudioIsReady){
                   //drawText(ADCValue);
                   //drawThousands(adcSignal>>2,200,100, color[0]);
                  //adcSignal = AdcaResultRegs.ADCRESULT0;
                 // ADSRGate = 1;
                  conversion=convertADC(adcSignal)/3.3;
                  difference=conversion-lastconversion;
                  if(difference>0.01 || difference<-0.01){
                      lastconversion = conversion;
                      setReverbDelay(conversion);
                      //fillScreen(color[1]);
                      textFill = 0;
                      drawCharQ('s', 200, drawFloat(conversion,200, 50, color[0]));
                  }
                 // ADSRGate = 0;

                   //audioOut = (int16)(((float)((int16)(channel1))*((float)((int16)(channel2)))/32768.0));
                  if(ReverbFilled&& lastconversion>0.009)  reverb();
                  AudioIsReady=0;
       }

//              temp= printD("Pressed",110,0) +10;
//              ltoa( LastPressedInfo[0].x, string);
//              printD(string, 100,  50);
//              memset_fast(string,0,50);
//              ltoa( LastPressedInfo[0].y, string);
//              printD(string, 90, 50);
//              memset_fast(string,0,50);
//
//              ltoa( LastPressedInfo[1].x, string);
//              printD(string, 100, temp);
//              memset_fast(string,0,50);
//              ltoa( LastPressedInfo[1].y, string);
//              printD(string, 90, temp);
//              memset_fast(string,0,50);
//              temp= printD("Let go",110,temp) +10;
//
//              printD("Time Held",110,temp);
//              ltoa(TS_holdtime, string);
//              printD(string, 90, temp);
//              memset_fast(string,0,50);
       //fillScreen(color[1]);
       //DELAY_US(100000);

        //fillRect(50, 100, 20, 20, color[0]);
//        if(touched()){
//            point=getTouchPoint(0);
//            drawHorzLine( 0, point.y, 300, genColor(0,0xff,0));
//            drawVertLine( point.x, 0, 800, genColor(0,0xff,0));
//        }
        //DELAY_US(10000);
        //fillScreen(color[0]);

    }
}

//__interrupt void MSBR_isr(void)
//{
//        // right channel transmission
//       interruptStore=IER;
//       IER &= M_INT6;
//       if(channel)
//       {
//           rightChannel = (Uint32)(McbspbRegs.DRR1.all);
//           rightChannel |= ((Uint32)(McbspbRegs.DRR2.all))<<16;
//           McbspbRegs.DXR1.all =0;
//           McbspbRegs.DXR2.all =(Uint16) ((rightChannel>>16)&0xffff);
//           channel = left;
//       }
//       // left channel transmission
//       else
//       {
//           McbspbRegs.DXR1.all = McbspbRegs.DRR1.all;
//           McbspbRegs.DXR2.all = McbspbRegs.DRR2.all;
//           channel = right;
//       }
//       PieCtrlRegs.PIEACK.all |= PIEACK_GROUP6;
//       IER|=interruptStore;
//}
__interrupt void BUT1_isr(void)
{
    Buttons[0] = 1;
    PieCtrlRegs.PIEACK.all |= PIEACK_GROUP1;
}
__interrupt void BUT2_isr(void)
{
    Buttons[1] = 1;
    PieCtrlRegs.PIEACK.all |= PIEACK_GROUP1;

}
__interrupt void BUT3_isr(void)
{
    Buttons[2] = 1;
    PieCtrlRegs.PIEACK.all |= PIEACK_GROUP12;
}
__interrupt void cpu_timer0_isr(void)
{
   EALLOW;
   CpuTimer0.InterruptCount++;
   //fillRect(200, 200, 10, 10, color[CpuTimer0.InterruptCount&1]);

 //  if(CpuTimer0.InterruptCount&4){
       TS_Check=1;
       I2C_StatusCheck();
  // }
   DrawCheck=1;


   // Acknowledge this __interrupt to receive more __interrupts from group 1
   //
   PieCtrlRegs.PIEACK.all |= PIEACK_GROUP1;
}
__interrupt void MCBSP_isr(void)
{

    EALLOW;
    // right channel transmission
 //interruptStore=IER;
//        IER &= M_INT6;
 //      interruptStore=IER;
   //       IER &= M_INT6;
          channel1 = McbspbRegs.DRR1.all;
          channel2 = McbspbRegs.DRR2.all;
          McbspbRegs.DXR1.all = AudioOut;
          McbspbRegs.DXR2.all = AudioOut;
          AudioIsReady=1;
          AudioOut = (int16)(((float)((int16)(channel1))+(float)((int16)(channel2)))*0.5);
          record(AudioOut);
          // fill audio buffer
          PieCtrlRegs.PIEACK.all |= PIEACK_GROUP6;

}
__interrupt void adca1_isr(void)
{
    adcSignal = AdcaResultRegs.ADCRESULT0;
    EALLOW;
    AdcaRegs.ADCINTFLGCLR.bit.ADCINT1 = 1; //clear INT1 flag
    PieCtrlRegs.PIEACK.all |= PIEACK_GROUP1;
    CpuTimer1.RegsAddr->TCR.bit.TIF = 1;
}
