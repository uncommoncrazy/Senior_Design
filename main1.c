

/**
 * main.c
 */
#include<F28x_Project.h>
#include <stdio.h>
#include <stdlib.h>
#include "OneToOneI2CDriver.h"
#define     LEDPins     0x00ff
#define     nLEDPins    0xffffff00
#define     PBPins      0x0001C000
#define     SwitchPins  0x00000f00
#define     Switch      8
#define     PB          9
#define     PU          (PBPins|SwitchPins)
#define     upperNib    0xf0
#define     lowerNib    0x0f
#define     EnableHi    0x0C
#define     EnableLo    0x08
#define     RS          0x01
#define     RSEnableHi  0x0C | RS
#define     RSEnableLo  0x08 | RS
#define     LCD_init1   0x33
#define     LCD_init2   0x32
#define     Mode        0x28
#define     DCP         0x0f
#define     ClearDisp   0x01
#define     Line2       0xC0
#define     Home        0x02
#define     LCD_addr    0x27
#define     HighByte    7
#define     LowByte     9
#define     SRAM_SizeW  0x1ffff
#define     inst_Read   3<<8
#define     inst_Write  2<<8
#define     locRange    0x8000
#define     R0          0
#define     R1          0x8000
#define     R2          0x10000
#define     R3          0x18000
#define     R4          0x20000
#define     R5          0x28000
#define     R6          0x30000
#define     R7          0x38000
#define     R0v         0x2356
#define     R1v         0x6743
#define     R2v         0x4473
#define     R3v         0x8943
#define     R4v         0x1259
#define     R5v         0x9877
#define     R6v         0x8631
#define     R7v         0x7218




Uint16 ButCurrent =0;
Uint16 ButLast=0;
Uint16 LCDInit[] = {0x3C,0x38,0x3C,0x38,0x3C,0x38,0x2C,0x28,0x2C,0x28,0x8C,0x88,0x0C,0x08,0xFC,0xF8,0x0C,0x08,0x1C,0x18};

Uint16 LCDBuffer[4];
void Init_SPIB(void);
void Init_IO(void);
void LCD_Command(Uint16 Command, Uint16 * const Buffer);
void LCD_Data(char data, Uint16 * const Buffer);
void Init_LCD( Uint16 * const Buffer);
char * DataToString(Uint16 data);
char   IntToChar(Uint16 number);
Uint16 * copyToBuffer( Uint16 value, Uint16 size);
void WriteValue(Uint32 Adrress, Uint16 value, Uint32 size);
void LCD_String(char * str, Uint16 * const Buffer);
Uint16  ButtonPressed(Uint16* Current,Uint16* Last, Uint16* cond);
Uint16  ButtonPressedMiddle(Uint16* Current,Uint16* Last);
Uint16  ButtonPressedRight(Uint16* Current,Uint16* Last);

Uint16 * SRAM_Read(Uint32 Addr, Uint16 size);
void SRAM_Write(Uint32 Addr, Uint16 * Buffer,Uint16 size);
Uint16* Test1(void);
Uint16* Test2(void);
int main(void)
{
    Uint16 * const I2C_Data = LCDBuffer;
    InitSysCtrl();
    Init_IO();
    Uint16 check = 0;
    Init_SPIB();
    WriteValue((Uint32)(R0),(Uint16)(R0v),(Uint32)(0x8000));
    WriteValue((Uint32)(R1),(Uint16)(R1v),(Uint32)(0x8000));
    WriteValue((Uint32)(R2),(Uint16)(R2v),(Uint32)(0x8000));
    WriteValue((Uint32)(R3),(Uint16)(R3v),(Uint32)(0x8000));
    WriteValue((Uint32)(R4),(Uint16)(R4v),(Uint32)(0x8000));
    WriteValue((Uint32)(R5),(Uint16)(R5v),(Uint32)(0x8000));
    WriteValue((Uint32)(R6),(Uint16)(R6v),(Uint32)(0x8000));
    WriteValue((Uint32)(R7),(Uint16)(R7v),(Uint32)(0x8000));
    Init_LCD(I2C_Data);
   // Uint16 temp []= {0x5599,0xAA88};
    //Buffer0= malloc(sizeof(Uint16));
   // *Buffer0 = 0x5555;
   // SRAM_Write(2,Buffer0,1);
   // Buffer1 =SRAM_Read(2,1);
    char * ranges[]={"R0","R1","R2","R3","R4","R5","R6","R7"};
    Uint32 rangesa[]={R0,R1,R2,R3,R4,R5,R6,R7};
    Uint16 index = 0;
    Uint16 *buf;
    char * txt;
    while(check!=2){
        while(!ButtonPressed(&ButCurrent,&ButLast,&check));
    }
    while(1){
        buf =SRAM_Read(rangesa[index]+5000,1);
        LCD_Command(Home,I2C_Data);
        LCD_String("Range ", I2C_Data);
        //ranges[index]
        txt = ranges[index];
        LCD_String(txt, I2C_Data);
        if(rangesa[index]<R4)
            LCD_String(" (SRAM0)", I2C_Data);
        else
            LCD_String(" (SRAM1)", I2C_Data);
        LCD_Command(Line2,I2C_Data);
        LCD_String("VAL:_ 0x", I2C_Data);
        txt =DataToString(*buf);
        LCD_String(txt,I2C_Data);
        free(txt);
        free(buf);
        while(!ButtonPressed(&ButCurrent,&ButLast,&check));

        switch(check){
            case 1:
            {
                if (index)
                    index--;
                else
                    index = 7;
                break;
            }
            case 2:{
                index = index;
                break;
            }
            case 3:
                if (index!=7)
                    index++;
                else
                    index = 0;
                break;
        }
    }
}

void LCD_String(char * str, Uint16 * const Buffer)
{
   Uint16 i = 0;
   while(*str){
       LCD_Data(*str, Buffer);
       str++;
       DELAY_US(2000);
   }
}
void Init_LCD( Uint16 * const Buffer)
{
    I2C_O2O_Master_Init(LCD_addr, 200.0,1.0);

    LCD_Command(LCD_init1, Buffer);
    LCD_Command(LCD_init2, Buffer);
    LCD_Command(Mode, Buffer);
    LCD_Command(DCP, Buffer);
    LCD_Command(ClearDisp, Buffer);

}
void Init_IO(void)
{
    EALLOW;
    Uint32 GPATemp = GpioCtrlRegs.GPADIR.all | LEDPins;
    GPATemp &= (~PU);
    GpioCtrlRegs.GPADIR.all = GPATemp;
    GpioCtrlRegs.GPAPUD.all &= ~(PU);
}
void LCD_Data(char data, Uint16 * const Buffer)
{
    Buffer[0]=  (Uint16)((data&0xf0)|RSEnableHi);
    Buffer[1]=  (Uint16)((data&0xf0)|RSEnableLo);
    Buffer[2]=  (Uint16)(((data<<4)&0xf0)|RSEnableHi);
    Buffer[3]=  (Uint16)(((data<<4)&0xf0)|RSEnableLo);
    I2C_O2O_SendBytes(Buffer, 4);

}
void LCD_Command(Uint16 Command, Uint16 * const Buffer)
{
    Buffer[0]= (Command&0xf0)|EnableHi;
    Buffer[1]= (Command&0xf0)|EnableLo;
    Buffer[2]= ((Command<<4)&0xf0)|EnableHi;
    Buffer[3]= ((Command<<4)&0xf0)|EnableLo;
    I2C_O2O_SendBytes(Buffer, 4);

}
Uint16  ButtonPressed(Uint16* Current,Uint16* Last, Uint16* cond)
{

    *Current = (GpioDataRegs.GPADAT.bit.GPIO16<<2) |(GpioDataRegs.GPADAT.bit.GPIO15<<1)|GpioDataRegs.GPADAT.bit.GPIO14;
    Uint16 C= *Current;
    Uint16 L=*Last;
    if((((*Current)&1) ==0)&&(((*Last)&1)==1)){
        *Last=*Current;
        *cond =1;
        return 1;
    }
    else if((((*Current)&2) ==0)&&(((*Last)&2)==2)){
        *Last=*Current;
        *cond =2;
        return 2;

    }
    else if((((*Current)&4) ==0)&&(((*Last)&4)==4)){
       *cond =3;
       *Last=*Current;
       return 3;

    }
    else{
        *Last=*Current;
        *cond =0;
        return 0;
    }
}

Uint16 * SRAM_Read(Uint32 Addr, Uint16 size)
{
    EALLOW;
    Uint16 boundary = 0;
    Uint16 * Buffer =  (Uint16 *)malloc(size*sizeof(Uint16));
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
                 while(!SpibRegs.SPISTS.bit.INT_FLAG);
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
    SpibRegs.SPITXBUF = (Uint16)((Addr>>1)&0xffff);
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
                 GpioDataRegs.GPCSET.bit.GPIO66=1;
                 GpioDataRegs.GPCCLEAR.bit.GPIO67=1;
                 SpibRegs.SPIDAT = inst_Write;
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
    free(Buffer);
}
char   IntToChar(Uint16 number)
{
       return (char) (number+0x30);
}
char * DataToString(Uint16 data)
{
       char * str = malloc(5*sizeof(char));
       str[0] = IntToChar((data>>12)&0x000f);
       str[1] = IntToChar((data>>8)&0x000f);
       str[2] = IntToChar((data>>4)&0x000f);
       str[3] = IntToChar((data)&0x000f);
       str[4] = 0;
       return str;
}
Uint16 * copyToBuffer( Uint16 value, Uint16 size)
{
    Uint16 * Buffer =  (Uint16 *)malloc(size*sizeof(Uint16));
    for (int i = 0; i<size; i++)
        Buffer[i]= value;
    return Buffer;
}

void WriteValue(Uint32 Address, Uint16 value, Uint32 size)
{

    Uint16 * Buffer;
    while (size>0){
        Buffer= copyToBuffer(value, 0x100);
        if(size>0x100){
            SRAM_Write(Address,Buffer,0x100);
            size = size-0x100;
        }
        else{
             SRAM_Write(Address,Buffer,size);
             size = 0;
        }
    }
   // free(Buffer);


}
/*Uint32 * Test1(void){
    Uint32 addr = 0;
    while(!ButtonPressed(&ButCurrent,&ButLast));

    // Read
    while(!ButtonPressed(&ButCurrent,&ButLast));
    addr = 0;
    Uint16 check = 0;
    GpioDataRegs.GPCCLEAR.bit.GPIO66=1;
    SpibRegs.SPIDAT = inst_Read;
    // highest Byte Shifted
    for(int j = 0; j<3; j++){//write starting higher sram
        SpibRegs.SPITXBUF = 0;
        while(SpibRegs.SPISTS.bit.BUFFULL_FLAG);
    }
    // dummy bits
    SpibRegs.SPITXBUF=0;
    while(SpibRegs.SPISTS.bit.BUFFULL_FLAG);
    check = SpibRegs.SPIRXBUF&0x00ff;
    SpibRegs.SPIDAT=0;
    while(!SpibRegs.SPISTS.bit.INT_FLAG);
    check = SpibRegs.SPIRXBUF&0x00ff;
    check = 0xAA;
    while((addr++<0x3ffff) && (check==0xAA))
    {
        SpibRegs.SPIDAT=0;
        while(!SpibRegs.SPISTS.bit.INT_FLAG);
        check = SpibRegs.SPIRXBUF&0x00ff;
    }
    GpioDataRegs.GPCSET.bit.GPIO66=1;
    if (check != 0xAA)
    {
        Uint32 * Buffer =  (Uint32  *)malloc(3*sizeof(Uint32 *));
        Buffer[0]=2;
        Buffer[1]=addr-1;
        Buffer[2]=check;
        return Buffer;
    }
    GpioDataRegs.GPCCLEAR.bit.GPIO67=1;
    SpibRegs.SPIDAT = inst_Read;
    // highest Byte Shifted
    for(int j = 0; j<3; j++){//write starting higher sram
    SpibRegs.SPITXBUF = 0;
    while(SpibRegs.SPISTS.bit.BUFFULL_FLAG);
    }
    // dummy bits
    SpibRegs.SPITXBUF=0;
    while(SpibRegs.SPISTS.bit.BUFFULL_FLAG);
    check = SpibRegs.SPIRXBUF&0x00ff;
    SpibRegs.SPIDAT=0;
    while(!SpibRegs.SPISTS.bit.INT_FLAG);
    check = SpibRegs.SPIRXBUF&0x00ff;
    check = 0xAA;
    while((addr++<0x7ffff)& (check==0xAA))
    {
       SpibRegs.SPIDAT=0;
       while(!SpibRegs.SPISTS.bit.INT_FLAG);
       check = SpibRegs.SPIRXBUF&0x00ff;
    }
    GpioDataRegs.GPCSET.bit.GPIO67=1;
    if (check != 0xAA)
      {
        Uint32 * Buffer =  (Uint32 *)malloc(3*sizeof(Uint32 *));
          Buffer[0]=2;
          Buffer[1]=addr-1;
          Buffer[2]=check;
          return Buffer;
      }
    Uint32 * Buffer =  (Uint32 * )malloc(1*sizeof(Uint32));
    Buffer[0]=1;
    return Buffer;
}
void WriteCount(void)
{
    GpioDataRegs.GPCCLEAR.bit.GPIO66=1;
       SpibRegs.SPIDAT = inst_Write;
       // highest Byte Shifted
       for(int j = 0; j<3; j++){//write starting higher sram
           SpibRegs.SPITXBUF = 0;
           while(SpibRegs.SPISTS.bit.BUFFULL_FLAG);
       }
       while(addr<0x1ffff)
       {
          SpibRegs.SPITXBUF = (addr<<8)&0xff00;
          while(SpibRegs.SPISTS.bit.BUFFULL_FLAG);
          SpibRegs.SPITXBUF = addr++&0xff00;
          while(SpibRegs.SPISTS.bit.BUFFULL_FLAG);

       }
       SpibRegs.SPITXBUF = 0x0;
       while(SpibRegs.SPISTS.bit.BUFFULL_FLAG);
       GpioDataRegs.GPCSET.bit.GPIO66=1;

       GpioDataRegs.GPCCLEAR.bit.GPIO67=1;
       SpibRegs.SPIDAT = inst_Write;
       // highest Byte Shifted
       for(int j = 0; j<3; j++){//write starting higher sram
          SpibRegs.SPITXBUF = 0;
          while(SpibRegs.SPISTS.bit.BUFFULL_FLAG);
       }
       while(addr++<0x3ffff)
       {
           SpibRegs.SPITXBUF = (addr<<8)&0xff00;
           while(SpibRegs.SPISTS.bit.BUFFULL_FLAG);
           SpibRegs.SPITXBUF = addr++&0xff00;
           while(SpibRegs.SPISTS.bit.BUFFULL_FLAG);
       }
       SpibRegs.SPITXBUF = 0x0;
       while(SpibRegs.SPISTS.bit.BUFFULL_FLAG);
       GpioDataRegs.GPCSET.bit.GPIO67=1;
}
void Test2(void){
    Uint32 addr = 0;
    while(!ButtonPressed(&ButCurrent,&ButLast));
    WriteCount();

    // Read
    while(!ButtonPressed(&ButCurrent,&ButLast));
    addr = 0;
    Uint16 check = 0;
    GpioDataRegs.GPCCLEAR.bit.GPIO66=1;
    SpibRegs.SPIDAT = inst_Read;
    // highest Byte Shifted
    for(int j = 0; j<3; j++){//write starting higher sram
        SpibRegs.SPITXBUF = 0;
        while(SpibRegs.SPISTS.bit.BUFFULL_FLAG);
    }
    // dummy bits
    SpibRegs.SPITXBUF=0;
    while(SpibRegs.SPISTS.bit.BUFFULL_FLAG);
    check = SpibRegs.SPIRXBUF&0x00ff;
    SpibRegs.SPIDAT=0;
    while(!SpibRegs.SPISTS.bit.INT_FLAG);
    check = SpibRegs.SPIRXBUF&0x00ff;
    check = 0;
    while((addr<0x1ffff) && (check==(Uint16)(addr&0xffff))
    {
        addr+=1;
        SpibRegs.SPIDAT=0;
        while(!SpibRegs.SPISTS.bit.INT_FLAG);
        check = SpibRegs.SPIRXBUF&0x00ff;
        SpibRegs.SPIDAT=0;
        while(!SpibRegs.SPISTS.bit.INT_FLAG);
        check |= SpibRegs.SPIRXBUF<<8;
    }
    GpioDataRegs.GPCSET.bit.GPIO66=1;
    if (check != (Uint16)(addr&0xffff))
    {
        Uint32 * Buffer =  (Uint32 *)malloc(3*sizeof(Uint32));
        Buffer[0]=2;
        Buffer[1]=addr;
        Buffer[2]=check;
        return Buffer;
    }
    GpioDataRegs.GPCCLEAR.bit.GPIO67=1;
    SpibRegs.SPIDAT = inst_Read;
    // highest Byte Shifted
    for(int j = 0; j<3; j++){//write starting higher sram
    SpibRegs.SPITXBUF = 0;
    while(SpibRegs.SPISTS.bit.BUFFULL_FLAG);
    }
    // dummy bits
    SpibRegs.SPITXBUF=0;
    while(SpibRegs.SPISTS.bit.BUFFULL_FLAG);
    check = SpibRegs.SPIRXBUF&0x00ff;
    SpibRegs.SPIDAT=0;
    while(!SpibRegs.SPISTS.bit.INT_FLAG);
    check = SpibRegs.SPIRXBUF&0x00ff;
    check = (Uint16)(addr&0xffff);
    while((addr<0x3ffff)&& (check != (Uint16)(addr&0xffff)))
    {
        addr+=1;
        SpibRegs.SPIDAT=0;
        while(!SpibRegs.SPISTS.bit.INT_FLAG);
        check = SpibRegs.SPIRXBUF&0x00ff;
        SpibRegs.SPIDAT=0;
        while(!SpibRegs.SPISTS.bit.INT_FLAG);
        check |= SpibRegs.SPIRXBUF<<8;
    }
    GpioDataRegs.GPCSET.bit.GPIO67=1;
    if (check != (Uint16)(addr&0xffff))
    {
      Uint32 * Buffer =  (Uint32 *)malloc(3*sizeof(Uint32));
      Buffer[0]=2;
      Buffer[1]=addr;
      Buffer[2]=check;
      return Buffer;
    }
    Uint16 * Buffer =  (Uint16 *)malloc(1*sizeof(Uint16));
    Buffer[0]=1;
    return Buffer;
}*/
void Init_SPIB(void){
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
   SpibRegs.SPIBRR.bit.SPI_BIT_RATE = 50;
   SpibRegs.SPICCR.bit.SPICHAR=7;
   SpibRegs.SPISTS.bit.OVERRUN_FLAG=1;
   SpibRegs.SPICTL.bit.TALK=1;
   SpibRegs.SPICCR.bit.SPISWRESET=1;
   SpibRegs.SPIPRI.bit.FREE=1;

}



