

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
    Uint16 darkGreen = genColor(0,100,0);
    Uint16 yellow= genColor(0xff,0xff,0);
    Uint16 magenta= genColor(0xff,0,0xff);

    masterVolume=1.0;
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
    ConfigCpuTimer(&CpuTimer1, 200, 100000);
    ConfigCpuTimer(&CpuTimer0, 200, 50000);
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
    Button ADSRButton = {
           .color={ColorButtonOff,ColorButtonOn},
           .colorText={ColorText,ColorText},
           .height=80,
           .width = 50,
           .x = 170,
           .y = 320-80,
           .string = "ADSR",
           .state = Button_Off
      };
    Button ReverbButton = {
               .color={ColorButtonOff,ColorButtonOn},
               .colorText={ColorText,ColorText},
               .height=80,
               .width = 20,
               .x = 50,
               .y = 200,
               .string = "Reverb",
               .state = Button_Off
          };
    Button VolButton = {
                  .color={magenta,darkGreen},
                  .colorText={darkGreen,yellow},
                  .height=80,
                  .width = 20,
                  .x = 220,
                  .y = 0,
                  .string = "Volume",
                  .state = Button_Off
     };
      Button FMButton = {
               .color={ColorButtonOff,ColorButtonOn},
               .colorText={ColorText,ColorText},
               .height=80,
               .width = 20,
               .x = 100,
               .y = 60,
               .string = "FM Mod",
               .state = Button_Off
          };
      Button AMButton = {
                 .color={ColorButtonOff,ColorButtonOn},
                 .colorText={ColorText,ColorText},
                 .height=80,
                 .width = 20,
                 .x = 100,
                 .y = 180,
                 .string = "AM Mod",
                 .state = Button_Off
            };
      //drawButton(ReverbButton, 0);
      drawButton(FMButton, 0);
      drawButton(AMButton, 0);
//      drawButton(ReverbButton, 0);
      drawButton(VolButton, 0);
      setReverbDecay(0.3);

    //drawButton(testButton,te);
    while(1){
        //fillScreen(color[1]);
        //drawText(Result);

       // drawChar('A', color[0], capitalLetter10, 100, 100);
        //test(100, 100, 20)
       // drawThousandsFloat(3.25,10TS_Position0,100, color[0]);
       TS_checkInteraction();
       checkButtonPress(&ADSRButton);
       ADSRGate = ADSRButton.state>>1;

      // checkButtonPress(&ReverbButton);
       checkButtonPress(&VolButton);

       checkButton(&FMButton);
       if(FMButton.state)
                  AMButton.state=0;
       checkButton(&AMButton);
       if(AMButton.state)
           FMButton.state=0;
       //ADSRGate = testButton.state>>1;
       if(DrawCheck){
           //drawButton(ReverbButton, ReverbButton.state);
           drawButton(ADSRButton, ADSRButton.state>>1);
           drawButton(VolButton, VolButton.state>>1);
           //drawButton(ReverbButton, ReverbButton.state>>1);
           drawButton(FMButton, FMButton.state&1);

           drawButton(AMButton, AMButton.state&1);
           DrawCheck=0;


       }
       if(AudioIsReady){
                   //drawText(ADCValue);
                   //drawThousands(adcSignal>>2,200,100, color[0]);
                  //adcSignal = AdcaResultRegs.ADCRESULT0;
                  conversion=(convertADC(adcSignal)/3.3)*3.0;
                  difference=conversion-lastconversion;
                  if(difference>0.01 || difference<-0.01){
                      lastconversion = conversion;

                  if(VolButton.state>>1)
                      masterVolume = conversion;

                      //fillScreen(color[1]);
                      textFill = 0;
                      drawBar(masterVolume/3.0, 220, 80, 20, 240, genColor(0xf0,0xf0,0));
                      //drawCharQ('A', 200, drawFloat(conversion,200, 50, color[0]));
                  }

                   //audioOut = (int16)(((float)((int16)(channel1))*((float)((int16)(channel2)))/32768.0));

                  if(BufferFilled&& (FMButton.state&1))  PhaseModulation();
                  if(AMButton.state&1)AmpModulation();
                  AudioIsReady=0;
//                  if(ReverbButton.state>>1){
//                        setReverbDelay(conversion);
//                        reverb();
//                  }
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


__interrupt void cpu_timer0_isr(void)
{
   EALLOW;
   CpuTimer0.InterruptCount++;
   //fillRect(200, 200, 10, 10, color[CpuTimer0.InterruptCount&1]);
   TS_Check=1;
          I2C_StatusCheck();
   if(!(CpuTimer0.InterruptCount&0X3) ){
       DrawCheck=1;
   }



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
          McbspbRegs.DXR1.all = (int16)(AudioOut*masterVolume);
          McbspbRegs.DXR2.all = (int16)(AudioOut*masterVolume);
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
