/*
 * SRAMDriver.c
 *
 *  Created on: Feb 23, 2019
 *      Author: Marco
 */
#include<F28x_Project.h>
#include <stdio.h>
#include <stdlib.h>
#include "SRAMDriver.h"
void Init_SRAM(void){
    EALLOW;
       GpioCtrlRegs.GPBGMUX2.bit.GPIO63 = 3;
       GpioCtrlRegs.GPBMUX2.bit.GPIO63 = 3;
       GpioCtrlRegs.GPBDIR.bit.GPIO63=1;
       GpioCtrlRegs.GPBPUD.bit.GPIO63=0;
       //GpioCtrlRegs.GPBCSEL4.bit.GPIO63 = 3
       GpioCtrlRegs.GPCGMUX1.bit.GPIO64=3;
       GpioCtrlRegs.GPCMUX1.bit.GPIO64=3;
       GpioCtrlRegs.GPCDIR.bit.GPIO64=0;
       GpioCtrlRegs.GPCPUD.bit.GPIO64=0;

       GpioCtrlRegs.GPCCSEL1.bit.GPIO64 = 3;
       GpioCtrlRegs.GPCGMUX1.bit.GPIO65=3;
       GpioCtrlRegs.GPCMUX1.bit.GPIO65=3;
       GpioCtrlRegs.GPCDIR.bit.GPIO65=1;
       // CS0
       GpioCtrlRegs.GPCGMUX1.bit.GPIO66=0;
       GpioCtrlRegs.GPCMUX1.bit.GPIO66=0;
       GpioCtrlRegs.GPCDIR.bit.GPIO66=1;
       //GpioCtrlRegs.GPCINV.bit.GPIO66=1;
       GpioDataRegs.GPCSET.bit.GPIO66=1;

       // CS1
       GpioCtrlRegs.GPCGMUX1.bit.GPIO67=0;
       GpioCtrlRegs.GPCMUX1.bit.GPIO67=0;
       GpioCtrlRegs.GPCDIR.bit.GPIO67=1;

       //GpioCtrlRegs.GPCINV.bit.GPIO67=1;
       GpioDataRegs.GPCSET.bit.GPIO67=1;


       //GpioCtrlRegs.GPCCSEL1.bit.GPIO65 = 3;
       SpibRegs.SPICCR.bit.SPISWRESET=0;
       SpibRegs.SPICTL.bit.MASTER_SLAVE=1;
       SpibRegs.SPICTL.bit.CLK_PHASE=1;
       ClkCfgRegs.LOSPCP.bit.LSPCLKDIV= 0;
       SpibRegs.SPICCR.bit.HS_MODE=0;
       SpibRegs.SPIBRR.bit.SPI_BIT_RATE = 4;
       SpibRegs.SPICCR.bit.SPICHAR=7;
       SpibRegs.SPISTS.bit.OVERRUN_FLAG=1;
       SpibRegs.SPICTL.bit.TALK=1;
       SpibRegs.SPICCR.bit.SPISWRESET=1;
       SpibRegs.SPIPRI.bit.FREE=1;


}
Uint16 * SRAM_Read(Uint32 Addr, Uint16 size)
{
    EALLOW;
    Uint16 boundary = 0;
    Uint16 * Buffer =  malloc(size*sizeof(Uint16));
    //address decoder
    switch(Addr & 0x20000)
    {
        case 0:// lower address space
            GpioDataRegs.GPCCLEAR.bit.GPIO66=1;
            SpibRegs.SPIDAT = inst_Read;
            // highest Byte Shifted
            SpibRegs.SPITXBUF = (Uint16) (Addr>>HighByte);
        break;
        default:// higher address space
            boundary=2;
            GpioDataRegs.GPCCLEAR.bit.GPIO67=1;
            SpibRegs.SPIDAT = inst_Read;
            //while(SpibRegs.SPISTS.bit.BUFFULL_FLAG);
            SpibRegs.SPITXBUF = (Uint16) ((Addr&SRAM_SizeW)>>HighByte);
        break;
    }
    // remainder of the address
    while(SpibRegs.SPISTS.bit.BUFFULL_FLAG);
    SpibRegs.SPITXBUF = (Uint16)((Addr<<1));
    while(SpibRegs.SPISTS.bit.BUFFULL_FLAG);
    SpibRegs.SPITXBUF = (Uint16)((Addr)<<LowByte);
    //dummy cycles
    while(SpibRegs.SPISTS.bit.BUFFULL_FLAG);
    SpibRegs.SPITXBUF=0;
    while(SpibRegs.SPISTS.bit.BUFFULL_FLAG);
    Buffer[0] = SpibRegs.SPIRXBUF&0x00ff;
    SpibRegs.SPIDAT=0;
    while(!SpibRegs.SPISTS.bit.INT_FLAG);
    Buffer[0] = SpibRegs.SPIRXBUF&0x00ff;
    // read array
    for (Uint32 i = 0; i < size; i++)
    {
        EALLOW;
        // check if boundary has been passed
        switch(boundary)
        {
            case 0:
                boundary = ((Addr+i) & 0x20000)>>17;
            break;
        }
        // switch SRAM if boundary has been passed
        switch(boundary)
         {
             case 1 :
                 boundary = 2;
                 GpioDataRegs.GPCSET.bit.GPIO66=1;
                 GpioDataRegs.GPCCLEAR.bit.GPIO67=1;
                 SpibRegs.SPIDAT = inst_Read;
                 for(int j = 0; j<3; j++){//write starting at higher sram
                     while(SpibRegs.SPISTS.bit.BUFFULL_FLAG);
                     SpibRegs.SPITXBUF = 0;
                 }
                 while(SpibRegs.SPISTS.bit.BUFFULL_FLAG);
                 SpibRegs.SPITXBUF=0;
                 while(SpibRegs.SPISTS.bit.BUFFULL_FLAG);
                 Buffer[i] = SpibRegs.SPIRXBUF&0x00ff;
                 SpibRegs.SPIDAT=0;
                 while(!SpibRegs.SPISTS.bit.INT_FLAG);
                 Buffer[i] = SpibRegs.SPIRXBUF&0x00ff;
             break;
         }
        SpibRegs.SPIDAT=0;
        while(!SpibRegs.SPISTS.bit.INT_FLAG);
        Buffer[i] = SpibRegs.SPIRXBUF&0x00ff;
        SpibRegs.SPIDAT=0;
        while(!SpibRegs.SPISTS.bit.INT_FLAG);
        Buffer[i] |= SpibRegs.SPIRXBUF<<8;
    }
    GpioDataRegs.GPCSET.bit.GPIO66=1;
    GpioDataRegs.GPCSET.bit.GPIO67=1;
    return Buffer;
}
void SRAM_Write(Uint32 Addr, Uint16 * Buffer,Uint16 size)
{
    EALLOW;
    Uint16 boundary = 0;
    Uint16 temp = 0;
    //address decoder
    //while(SpibRegs.SPISTS.bit.BUFFULL_FLAG);
    switch(Addr & 0x20000)
    {
        case 0: // lower address space
            GpioDataRegs.GPCCLEAR.bit.GPIO66=1;
            SpibRegs.SPIDAT = inst_Write;
            // highest Byte Shifted
           // while(SpibRegs.SPISTS.bit.BUFFULL_FLAG);
            SpibRegs.SPITXBUF = (Uint16) (Addr>>HighByte);
        break;
        default:// higher address space
            boundary=2;
            GpioDataRegs.GPCCLEAR.bit.GPIO67=1;
            SpibRegs.SPIDAT = inst_Write;
            //while(SpibRegs.SPISTS.bit.BUFFULL_FLAG);
            SpibRegs.SPITXBUF = (Uint16) ((Addr&SRAM_SizeW)>>HighByte);
        break;
    }
    while(SpibRegs.SPISTS.bit.BUFFULL_FLAG);
    SpibRegs.SPITXBUF = (Uint16)((Addr<<1)&0xffff);
    while(SpibRegs.SPISTS.bit.BUFFULL_FLAG);
    SpibRegs.SPITXBUF = (Uint16)(Addr<<LowByte);
    for (Uint16 i = 0; i < size; i++)
    {
        EALLOW;
        // check if boundary has been passed
        switch(boundary)
        {
            case 0:
                boundary = ((Addr+i) & 0x20000)>>17;
        }
        // switch SRAM if boundary has been passed
        switch(boundary)
         {
             case 1 :
             {
                 boundary = 2;
                 while(SpibRegs.SPISTS.bit.BUFFULL_FLAG);
                 SpibRegs.SPITXBUF = 0;
                 while(SpibRegs.SPISTS.bit.BUFFULL_FLAG);
                 GpioDataRegs.GPCSET.bit.GPIO66=1;
                 SpibRegs.SPITXBUF = inst_Write;
                 while(SpibRegs.SPISTS.bit.BUFFULL_FLAG);
                 GpioDataRegs.GPCCLEAR.bit.GPIO67=1;


                 for(int j = 0; j<3; j++){//write starting higher sram
                     while(SpibRegs.SPISTS.bit.BUFFULL_FLAG);
                     SpibRegs.SPITXBUF = 0;
                 }
             }
         }
        // sequential writes
        while(SpibRegs.SPISTS.bit.BUFFULL_FLAG);
        SpibRegs.SPITXBUF = (Buffer[i]<<8)&0xff00;
        while(SpibRegs.SPISTS.bit.BUFFULL_FLAG);
        SpibRegs.SPITXBUF = Buffer[i]&0xff00;

    }
    while(SpibRegs.SPISTS.bit.BUFFULL_FLAG);
    SpibRegs.SPITXBUF = 0;
    while(SpibRegs.SPISTS.bit.BUFFULL_FLAG);
    //SpibRegs.SPITXBUF = 0;
    //while(SpibRegs.SPISTS.bit.BUFFULL_FLAG);
    GpioDataRegs.GPCSET.bit.GPIO66=1;
    GpioDataRegs.GPCSET.bit.GPIO67=1;
    temp=  SpibRegs.SPIRXBUF;
    while(!SpibRegs.SPISTS.bit.INT_FLAG);
    far_free(Buffer);
}
Uint16 * copyToBuffer( Uint16 value, Uint16 size)
{
    Uint16 * Buffer =  (Uint16 *)malloc(size*sizeof(Uint16));
    for (int i = 0; i<size; i++)
        Buffer[i]= value;
    return Buffer;
}

void WriteValue(Uint32 Adrress, Uint16 value, Uint32 size)
{

    Uint16 * Buffer = copyToBuffer(value, 0x100);
    while (size>0){
        if(size>0x500){
            SRAM_Write(Adrress,Buffer,0x100);
            size = size-0x100;
        }
        else{
             SRAM_Write(Adrress,Buffer,size);
             size = 0;
        }
        free(Buffer);
    }

}
void SRAM_Wr(Uint32 Addr, Uint16 data)
{
    EALLOW;
    Uint16 boundary = 0;
    Uint16 temp = 0;
    //address decoder
    //while(SpibRegs.SPISTS.bit.BUFFULL_FLAG);
    switch(Addr & 0x20000)
    {
        case 0: // lower address space
            GpioDataRegs.GPCCLEAR.bit.GPIO66=1;
            SpibRegs.SPIDAT = inst_Write;
            // highest Byte Shifted
           // while(SpibRegs.SPISTS.bit.BUFFULL_FLAG);
            SpibRegs.SPITXBUF = (Uint16) (Addr>>HighByte);
        break;
        default:// higher address space
            boundary=2;
            GpioDataRegs.GPCCLEAR.bit.GPIO67=1;
            SpibRegs.SPIDAT = inst_Write;
            //while(SpibRegs.SPISTS.bit.BUFFULL_FLAG);
            SpibRegs.SPITXBUF = (Uint16) ((Addr&SRAM_SizeW)>>HighByte);
        break;
    }
    while(SpibRegs.SPISTS.bit.BUFFULL_FLAG);
    SpibRegs.SPITXBUF = (Uint16)((Addr<<1)&0xffff);
    while(SpibRegs.SPISTS.bit.BUFFULL_FLAG);
    SpibRegs.SPITXBUF = (Uint16)(Addr<<LowByte);

    EALLOW;
    // check if boundary has been passed
    // sequential writes
    while(SpibRegs.SPISTS.bit.BUFFULL_FLAG);
    SpibRegs.SPITXBUF = (data<<8)&0xff00;
    while(SpibRegs.SPISTS.bit.BUFFULL_FLAG);
    SpibRegs.SPITXBUF = data&0xff00;


    while(SpibRegs.SPISTS.bit.BUFFULL_FLAG);
    SpibRegs.SPITXBUF = 0;
    while(SpibRegs.SPISTS.bit.BUFFULL_FLAG);
    //SpibRegs.SPITXBUF = 0;
    //while(SpibRegs.SPISTS.bit.BUFFULL_FLAG);
    GpioDataRegs.GPCSET.bit.GPIO66=1;
    GpioDataRegs.GPCSET.bit.GPIO67=1;
}
Uint16 SRAM_Rd(Uint32 Addr)
{
    EALLOW;
    Uint16 boundary = 0;
    Uint16 data = 0 ;
    //address decoder
    switch(Addr & 0x20000)
    {
        case 0:// lower address space
            GpioDataRegs.GPCCLEAR.bit.GPIO66=1;
            SpibRegs.SPIDAT = inst_Read;
            // highest Byte Shifted
            SpibRegs.SPITXBUF = (Uint16) (Addr>>HighByte);
        break;
        default:// higher address space
            boundary=2;
            GpioDataRegs.GPCCLEAR.bit.GPIO67=1;
            SpibRegs.SPIDAT = inst_Read;
            //while(SpibRegs.SPISTS.bit.BUFFULL_FLAG);
            SpibRegs.SPITXBUF = (Uint16) ((Addr&SRAM_SizeW)>>HighByte);
        break;
    }
    // remainder of the address
    while(SpibRegs.SPISTS.bit.BUFFULL_FLAG);
    SpibRegs.SPITXBUF = (Uint16)((Addr<<1));
    while(SpibRegs.SPISTS.bit.BUFFULL_FLAG);
    SpibRegs.SPITXBUF = (Uint16)((Addr)<<LowByte);
    //dummy cycles
    while(SpibRegs.SPISTS.bit.BUFFULL_FLAG);
    SpibRegs.SPITXBUF=0;
    while(SpibRegs.SPISTS.bit.BUFFULL_FLAG);
    data = SpibRegs.SPIRXBUF&0x00ff;
    SpibRegs.SPIDAT=0;
    while(!SpibRegs.SPISTS.bit.INT_FLAG);
    data = SpibRegs.SPIRXBUF&0x00ff;
    // read array
    EALLOW;

    SpibRegs.SPIDAT=0;
    while(!SpibRegs.SPISTS.bit.INT_FLAG);
    data = SpibRegs.SPIRXBUF&0x00ff;
    SpibRegs.SPIDAT=0;
    while(!SpibRegs.SPISTS.bit.INT_FLAG);
    data |= SpibRegs.SPIRXBUF<<8;

    GpioDataRegs.GPCSET.bit.GPIO66=1;
    GpioDataRegs.GPCSET.bit.GPIO67=1;
    return data;
}
