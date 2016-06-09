//*****************************************************************************
//
// File Name    : 'lcd_lib.h'
// Title        : 8 and 4 bit LCd interface + mixed pins modes
// Author       : Scienceprog.com - Copyright (C) 2007
// Created      : 2007-03-29
// Revised      : 2011-12-13
// Version      : 2.0
// Target MCU   : Atmel AVR series
//
// This code is distributed under the GNU Public License
//      which can be found at http://www.gnu.org/licenses/gpl.txt
//
//*****************************************************************************
#include "lcd_lib.h"
#include <inttypes.h>
#include <avr/io.h>
#include <avr/pgmspace.h>
#include <util/delay.h>

#ifdef LCD_4BIT_M
static void LCDMix_4Bit(uint8_t data);
#endif
#ifdef LCD_8BIT_M
static void LCDMix_8Bit(uint8_t data);
#endif
const uint8_t LcdCustomChar[] PROGMEM=//define 8 custom LCD chars
{
    0x00, 0x1F, 0x00, 0x00, 0x00, 0x00, 0x1F, 0x00, // 0. 0/5 full progress block
    0x00, 0x1F, 0x10, 0x10, 0x10, 0x10, 0x1F, 0x00, // 1. 1/5 full progress block
    0x00, 0x1F, 0x18, 0x18, 0x18, 0x18, 0x1F, 0x00, // 2. 2/5 full progress block
    0x00, 0x1F, 0x1C, 0x1C, 0x1C, 0x1C, 0x1F, 0x00, // 3. 3/5 full progress block
    0x00, 0x1F, 0x1E, 0x1E, 0x1E, 0x1E, 0x1F, 0x00, // 4. 4/5 full progress block
    0x00, 0x1F, 0x1F, 0x1F, 0x1F, 0x1F, 0x1F, 0x00, // 5. 5/5 full progress block
    0x03, 0x07, 0x0F, 0x1F, 0x0F, 0x07, 0x03, 0x00, // 6. rewind arrow
    0x18, 0x1C, 0x1E, 0x1F, 0x1E, 0x1C, 0x18, 0x00  // 7. fast-forward arrow
};

#ifdef LCD_4BIT_M
//sets four port pins to corresponding nibble
//for high nibble use mask data & 0b11110000
//for low nibble use (data & 0b00001111)<<4
static void LCDMix_4Bit(uint8_t data)
{
  if((data)&(0b10000000)) LDPD7 |=1<<LCD_D7;
    else LDPD7 &=~(1<<LCD_D7);
  if((data)&(0b01000000)) LDPD6 |=1<<LCD_D6;
    else LDPD6 &=~(1<<LCD_D6);
  if((data)&(0b00100000)) LDPD5 |=1<<LCD_D5;
    else LDPD5&=~(1<<LCD_D5);
  if((data)&(0b00010000)) LDPD4 |=1<<LCD_D4;
    else LDPD4 &=~(1<<LCD_D4);
}
#endif
#ifdef LCD_8BIT_M
static void LCDMix_8Bit(uint8_t data)
{
  if((data)&(0b10000000)) LDPD7 |=1<<LCD_D7;
    else LDPD7 &=~(1<<LCD_D7);
  if((data)&(0b01000000)) LDPD6 |=1<<LCD_D6;
    else LDPD6 &=~(1<<LCD_D6);
  if((data)&(0b00100000)) LDPD5 |=1<<LCD_D5;
    else LDPD5&=~(1<<LCD_D5);
  if((data)&(0b00010000)) LDPD4 |=1<<LCD_D4;
    else LDPD4 &=~(1<<LCD_D4);
  if((data)&(0b00001000)) LDPD3 |=1<<LCD_D3;
    else LDPD3 &=~(1<<LCD_D3);
  if((data)&(0b00000100)) LDPD2 |=1<<LCD_D2;
    else LDPD2 &=~(1<<LCD_D2);
  if((data)&(0b00000010)) LDPD1 |=1<<LCD_D1;
    else LDPD1&=~(1<<LCD_D1);
  if((data)&(0b00000001)) LDPD0 |=1<<LCD_D0;
    else LDPD0 &=~(1<<LCD_D0);
}
#endif
void LCDinit(void)//Initializes LCD
{
#ifdef LCD_4BIT
    //4 bit part
    _delay_ms(15);
    //zero to data pins
    LDP &=~(1<<LCD_D7|1<<LCD_D6|1<<LCD_D5|1<<LCD_D4);
    //zero to control pins
    LCP &= ~(1<<LCD_E|1<<LCD_RW|1<<LCD_RS);
    LDDR|=1<<LCD_D7|1<<LCD_D6|1<<LCD_D5|1<<LCD_D4;
    LCDR|=1<<LCD_E|1<<LCD_RW|1<<LCD_RS;
   //---------one------
    LDP=0<<LCD_D7|0<<LCD_D6|1<<LCD_D5|1<<LCD_D4; //4 bit mode
    LCP|=1<<LCD_E|0<<LCD_RW|0<<LCD_RS;
    _delay_ms(1);
    LCP&=~(1<<LCD_E);
    _delay_ms(1);
    //-----------two-----------
    LDP=0<<LCD_D7|0<<LCD_D6|1<<LCD_D5|1<<LCD_D4; //4 bit mode
    LCP|=1<<LCD_E|0<<LCD_RW|0<<LCD_RS;
    _delay_ms(1);
    LCP&=~(1<<LCD_E);
    _delay_ms(1);
    //-------three-------------
    LDP=0<<LCD_D7|0<<LCD_D6|1<<LCD_D5|0<<LCD_D4; //4 bit mode
    LCP|=1<<LCD_E|0<<LCD_RW|0<<LCD_RS;
    _delay_ms(1);
    LCP&=~(1<<LCD_E);
    _delay_ms(1);
    //--------4 bit--dual line---------------
    LCDsendCommand(0b00101000);
   //-----increment address, invisible cursor shift------
    LCDsendCommand(0b00001100);
#endif
#ifdef LCD_8BIT
    //8 bit part
    _delay_ms(15);
    //zero to datapins
    LDP =0x00;
    //zero to control pins
    LCP &=(1<<LCD_E|1<<LCD_RW|1<<LCD_RS);
    //set direction pins for data and control
    LDDR|=1<<LCD_D7|1<<LCD_D6|1<<LCD_D5|1<<LCD_D4|1<<LCD_D3
            |1<<LCD_D2|1<<LCD_D1|1<<LCD_D0;
    LCDR|=1<<LCD_E|1<<LCD_RW|1<<LCD_RS;
   //---------one------
    LDP=0<<LCD_D7|0<<LCD_D6|1<<LCD_D5|1<<LCD_D4|0<<LCD_D3
            |0<<LCD_D2|0<<LCD_D1|0<<LCD_D0; //8 it mode
    LCP|=1<<LCD_E|0<<LCD_RW|0<<LCD_RS;
    _delay_ms(1);
    LCP&=~(1<<LCD_E);
    _delay_ms(1);
    //-----------two-----------
    LDP=0<<LCD_D7|0<<LCD_D6|1<<LCD_D5|1<<LCD_D4|0<<LCD_D3
            |0<<LCD_D2|0<<LCD_D1|0<<LCD_D0; //8 it mode
    LCP|=1<<LCD_E|0<<LCD_RW|0<<LCD_RS;
    _delay_ms(1);
    LCP&=~(1<<LCD_E);
    _delay_ms(1);
    //-------three-------------
    LDP=0<<LCD_D7|0<<LCD_D6|1<<LCD_D5|1<<LCD_D4|0<<LCD_D3
            |0<<LCD_D2|0<<LCD_D1|0<<LCD_D0; //8 it mode
    LCP|=1<<LCD_E|0<<LCD_RW|0<<LCD_RS;
    _delay_ms(1);
    LCP&=~(1<<LCD_E);
    _delay_ms(1);
    //--------8 bit dual line----------
    LCDsendCommand(0b00111000); //8 bit mode
    /*LDP=0<<LCD_D7|0<<LCD_D6|1<<LCD_D5|1<<LCD_D4|1<<LCD_D3
            |0<<LCD_D2|0<<LCD_D1|0<<LCD_D0;
    LCP|=1<<LCD_E|0<<LCD_RW|0<<LCD_RS;
    _delay_ms(1);
    LCP&=~(1<<LCD_E);
    _delay_ms(1); */
   //-----increment address, invisible cursor shift------
    LCDsendCommand(0b00001100);
    /*LDP=0<<LCD_D7|0<<LCD_D6|0<<LCD_D5|0<<LCD_D4|1<<LCD_D3
            |1<<LCD_D2|0<<LCD_D1|0<<LCD_D0;
    LCP|=1<<LCD_E|0<<LCD_RW|0<<LCD_RS;
    _delay_ms(1);
    LCP&=~(1<<LCD_E);
    _delay_ms(5);*/

#endif

#ifdef LCD_4BIT_M
    //4 mixed bit part
    _delay_ms(15);
    //zero to data pins
    LCDMix_4Bit(0b00000000);
    //zero to control pins
    LDPRS &=~(1<<LCD_RS);
    LDPRW &=~(1<<LCD_RW);
    LDPE &=~(1<<LCD_E);
    //set data direction
    LDDD4 |=1<<LCD_D4;
    LDDD5 |=1<<LCD_D5;
    LDDD6 |=1<<LCD_D6;
    LDDD7 |=1<<LCD_D7;
    //control direction pins
    LDDRS |=1<<LCD_RS;
    LDDRW |=1<<LCD_RW;
    LDDE |=1<<LCD_E;
   //---------one------
    LCDMix_4Bit(0b00110000);
    LDPE |=1<<LCD_E;
    _delay_ms(1);
    LDPE &=~(1<<LCD_E);
    _delay_ms(1);
    //-----------two-----------
    LCDMix_4Bit(0b00110000);
    LDPE |=1<<LCD_E;
    _delay_ms(1);
    LDPE &=~(1<<LCD_E);
    _delay_ms(1);
    //-------three-------------
    LCDMix_4Bit(0b00100000);
    LDPE |=1<<LCD_E;
    _delay_ms(1);
    LDPE &=~(1<<LCD_E);
    _delay_ms(1);
    //--------4 bit--dual line---------------
    LCDsendCommand(0b00101000);
   //-----increment address, invisible cursor shift------
    LCDsendCommand(0b00001100);
#endif

#ifdef LCD_8BIT_M
    //8 mixed bits part
    _delay_ms(15);
    //zero to data pins
    LCDMix_8Bit(0b00000000);
    //zero to control pins
    LDPRS &=~(1<<LCD_RS);
    LDPRW &=~(1<<LCD_RW);
    LDPE &=~(1<<LCD_E);
    //set data direction
    LDDD0 |=1<<LCD_D0;
    LDDD1 |=1<<LCD_D1;
    LDDD2 |=1<<LCD_D2;
    LDDD3 |=1<<LCD_D3;
    LDDD4 |=1<<LCD_D4;
    LDDD5 |=1<<LCD_D5;
    LDDD6 |=1<<LCD_D6;
    LDDD7 |=1<<LCD_D7;
    //control direction pins
    LDDRS |=1<<LCD_RS;
    LDDRW |=1<<LCD_RW;
    LDDE |=1<<LCD_E;
   //---------one------
    LCDMix_8Bit(0b00110000);
    /*LDPD4 |=1<<LCD_D4;
    LDPD5 |=1<<LCD_D5;*/
    LDPE |=1<<LCD_E;
    _delay_ms(1);
    LDPE &=~(1<<LCD_E);
    _delay_ms(1);
    //-----------two-----------
    LDPE |=1<<LCD_E;
    _delay_ms(1);
    LDPE &=~(1<<LCD_E);
    _delay_ms(1);
    //-------three-------------
    LDPE |=1<<LCD_E;
    _delay_ms(1);
    LDPE &=~(1<<LCD_E);
    _delay_ms(1);
    //--------8 bit dual line----------
    LCDsendCommand(0b00111000);
   //-----increment address, invisible cursor shift------
    LCDsendCommand(0b00001100);
#endif
        //init 8 custom chars
    uint8_t ch=0, chn=0;
    while(ch<64)
    {
        LCDdefinechar((LcdCustomChar+ch),chn++);
        ch=ch+8;
    }
}
void LCDsendChar(uint8_t ch)        //Sends Char to LCD
{

#ifdef LCD_4BIT
    //4 bit part
    LDP=(ch & 0b11110000);
    LCP|=1<<LCD_RS;
    LCP|=1<<LCD_E;
    _delay_ms(1);
    LCP&=~(1<<LCD_E);
    LCP&=~(1<<LCD_RS);
    _delay_ms(1);
    LDP=((ch & 0b00001111)<<4);
    LCP|=1<<LCD_RS;
    LCP|=1<<LCD_E;
    _delay_ms(1);
    LCP&=~(1<<LCD_E);
    LCP&=~(1<<LCD_RS);
    _delay_ms(1);
#endif
#ifdef LCD_8BIT
    //8 bit part
    LDP=ch;
    LCP|=1<<LCD_RS;
    LCP|=1<<LCD_E;
    _delay_ms(1);
    LCP&=~(1<<LCD_E);
    LCP&=~(1<<LCD_RS);
    _delay_ms(1);
#endif
#ifdef LCD_4BIT_M
    LCDMix_4Bit(ch & 0b11110000);
    LDPRS |=1<<LCD_RS;
    LDPE |=1<<LCD_E;
    _delay_ms(1);
    LDPE &=~(1<<LCD_E);
    LDPRS &=~(1<<LCD_RS);
    _delay_ms(1);
    LCDMix_4Bit((ch & 0b00001111)<<4);
    LDPRS |=1<<LCD_RS;
    LDPE |=1<<LCD_E;
    _delay_ms(1);
    LDPE &=~(1<<LCD_E);
    LDPRS &=~(1<<LCD_RS);
    _delay_ms(1);
#endif
#ifdef LCD_8BIT_M
    LCDMix_8Bit(ch);
    LDPRS |=1<<LCD_RS;
    LDPE |=1<<LCD_E;
    _delay_ms(1);
    LDPE &=~(1<<LCD_E);
    LDPRS &=~(1<<LCD_RS);
    _delay_ms(1);
#endif
}
void LCDsendCommand(uint8_t cmd)    //Sends Command to LCD
{
#ifdef LCD_4BIT
    //4 bit part
    LDP=(cmd & 0b11110000);
    LCP|=1<<LCD_E;
    _delay_ms(1);
    LCP&=~(1<<LCD_E);
    _delay_ms(1);
    LDP=((cmd & 0b00001111)<<4);
    LCP|=1<<LCD_E;
    _delay_ms(1);
    LCP&=~(1<<LCD_E);
    _delay_ms(1);
#endif
#ifdef LCD_8BIT
    //8 bit part
    LDP=cmd;
    LCP|=1<<LCD_E;
    _delay_ms(1);
    LCP&=~(1<<LCD_E);
    _delay_ms(1);
#endif
#ifdef LCD_4BIT_M
    LCDMix_4Bit(cmd & 0b11110000);
    LDPE |=1<<LCD_E;
    _delay_ms(1);
    LDPE &=~(1<<LCD_E);
    _delay_ms(1);
    LCDMix_4Bit((cmd & 0b00001111)<<4);
    LDPE |=1<<LCD_E;
    _delay_ms(1);
    LDPE &=~(1<<LCD_E);
    _delay_ms(1);
#endif
#ifdef LCD_8BIT_M
    LCDMix_8Bit(cmd);
    LDPE |=1<<LCD_E;
    _delay_ms(1);
    LDPE &=~(1<<LCD_E);
    _delay_ms(1);
#endif
}
void LCDclr(void)               //Clears LCD
{
    LCDsendCommand(1<<LCD_CLR);
}
void LCDhome(void)          //LCD cursor home
{
    LCDsendCommand(1<<LCD_HOME);
}
void LCDstring(uint8_t* data, uint8_t nBytes)   //Outputs string to LCD
{
register uint8_t i;

    // check to make sure we have a good pointer
    if (!data) return;

    // print data
    for(i=0; i<nBytes; i++)
    {
        LCDsendChar(data[i]);
    }
}
void LCDGotoXY(uint8_t x, uint8_t y)    //Cursor to X Y position
{
    register uint8_t DDRAMAddr;
    // remap lines into proper order
    switch(y)
    {
    case 0: DDRAMAddr = LCD_LINE0_DDRAMADDR+x; break;
    case 1: DDRAMAddr = LCD_LINE1_DDRAMADDR+x; break;
    case 2: DDRAMAddr = LCD_LINE2_DDRAMADDR+x; break;
    case 3: DDRAMAddr = LCD_LINE3_DDRAMADDR+x; break;
    default: DDRAMAddr = LCD_LINE0_DDRAMADDR+x;
    }
    // set data address
    LCDsendCommand(1<<LCD_DDRAM | DDRAMAddr);

}
//Copies string from flash memory to LCD at x y position
//const uint8_t welcomeln1[] PROGMEM="AVR LCD DEMO\0";
//CopyStringtoLCD(welcomeln1, 3, 1);
void CopyStringtoLCD(const uint8_t *FlashLoc, uint8_t x, uint8_t y)
{
    uint8_t i;
    LCDGotoXY(x,y);
    for(i=0;(uint8_t)pgm_read_byte(&FlashLoc[i]);i++)
    {
        LCDsendChar((uint8_t)pgm_read_byte(&FlashLoc[i]));
    }
}
//defines char symbol in CGRAM
/*
const uint8_t backslash[] PROGMEM=
{
0b00000000,//back slash
0b00010000,
0b00001000,
0b00000100,
0b00000010,
0b00000001,
0b00000000,
0b00000000
};
LCDdefinechar(backslash,0);
*/
void LCDdefinechar(const uint8_t *pc,uint8_t char_code){
    uint8_t a, pcc;
    uint16_t i;
    a=(char_code<<3)|0x40;
    for (i=0; i<8; i++){
        pcc=pgm_read_byte(&pc[i]);
        LCDsendCommand(a++);
        LCDsendChar(pcc);
        }
}

void LCDshiftLeft(uint8_t n)    //Scrol n of characters Right
{
    uint8_t i;
    for (i=0;i<n;i++)
    {
        LCDsendCommand(0x1E);
    }
}
void LCDshiftRight(uint8_t n)   //Scrol n of characters Left
{
    uint8_t i;
    for (i=0;i<n;i++)
    {
        LCDsendCommand(0x18);
    }
}
void LCDcursorOn(void) //displays LCD cursor
{
    LCDsendCommand(0x0E);
}
void LCDcursorOnBlink(void) //displays LCD blinking cursor
{
    LCDsendCommand(0x0F);
}
void LCDcursorOFF(void) //turns OFF cursor
{
    LCDsendCommand(0x0C);
}
void LCDblank(void)     //blanks LCD
{
    LCDsendCommand(0x08);
}
void LCDvisible(void)       //Shows LCD
{
    LCDsendCommand(0x0C);
}
void LCDcursorLeft(uint8_t n)   //Moves cursor by n poisitions left
{
    uint8_t i;
    for (i=0;i<n;i++)
    {
        LCDsendCommand(0x10);
    }
}
void LCDcursorRight(uint8_t n)  //Moves cursor by n poisitions left
{
    uint8_t i;
    for (i=0;i<n;i++)
    {
        LCDsendCommand(0x14);
    }
}
//adapted fro mAVRLIB
void LCDprogressBar(uint8_t progress, uint8_t maxprogress, uint8_t length)
{
    uint8_t i;
    uint16_t pixelprogress;
    uint8_t c;

    // draw a progress bar displaying (progress / maxprogress)
    // starting from the current cursor position
    // with a total length of "length" characters
    // ***note, LCD chars 0-5 must be programmed as the bar characters
    // char 0 = empty ... char 5 = full

    // total pixel length of bargraph equals length*PROGRESSPIXELS_PER_CHAR;
    // pixel length of bar itself is
    pixelprogress = ((progress*(length*PROGRESSPIXELS_PER_CHAR))/maxprogress);

    // print exactly "length" characters
    for(i=0; i<length; i++)
    {
        // check if this is a full block, or partial or empty
        // (u16) cast is needed to avoid sign comparison warning
        if( ((i*(uint16_t)PROGRESSPIXELS_PER_CHAR)+5) > pixelprogress )
        {
            // this is a partial or empty block
            if( ((i*(uint16_t)PROGRESSPIXELS_PER_CHAR)) > pixelprogress )
            {
                // this is an empty block
                // use space character?
                c = 0;
            }
            else
            {
                // this is a partial block
                c = pixelprogress % PROGRESSPIXELS_PER_CHAR;
            }
        }
        else
        {
            // this is a full block
            c = 5;
        }

        // write character to display
        LCDsendChar(c);
    }

}
