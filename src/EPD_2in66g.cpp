/*****************************************************************************
* | File        :   EPD_2in66g.c
* | Author      :   Waveshare team
* | Function    :   2.66inch e-Paper (G) Driver
* | Info        :
*----------------
* | This version:   V1.0
* | Date        :   2023-12-20
* | Info        :
* -----------------------------------------------------------------------------
#
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documnetation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to  whom the Software is
# furished to do so, subject to the following conditions:
#
# The above copyright notice and this permission notice shall be included in
# all copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS OR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
# THE SOFTWARE.
#
******************************************************************************/
#include "EPD_2in66g.h"

/******************************************************************************
function :  Software reset
parameter:
******************************************************************************/
static void EPD_2IN66G_Reset(void)
{
    digitalWrite(EPD_RST_PIN, 1);
    delay_ms(20);
    digitalWrite(EPD_RST_PIN, 0);
    delay_ms(2);
    digitalWrite(EPD_RST_PIN, 1);
    delay_ms(20);
}

/******************************************************************************
function :  send command
parameter:
     Reg : Command register
******************************************************************************/
static void EPD_2IN66G_SendCommand(uint8_t Reg)
{
    digitalWrite(EPD_DC_PIN, 0);
    digitalWrite(EPD_CS_PIN, 0);
    SPI_WriteByte(Reg);
    digitalWrite(EPD_CS_PIN, 1);
}

/******************************************************************************
function :  send data
parameter:
    Data : Write data
******************************************************************************/
static void EPD_2IN66G_SendData(uint8_t Data)
{
    digitalWrite(EPD_DC_PIN, 1);
    digitalWrite(EPD_CS_PIN, 0);
    SPI_WriteByte(Data);
    digitalWrite(EPD_CS_PIN, 1);
}

/******************************************************************************
function :  Wait until the busy_pin goes HIGH
parameter:
******************************************************************************/
static void EPD_2IN66G_ReadBusyH(void)
{
    Serial.println("e-Paper wait busy H");
    while(!digitalRead(EPD_BUSY_PIN)) {      //LOW: busy, HIGH: idle
        delay_ms(5);
    }
    Serial.println("e-Paper busy H release");
}

/******************************************************************************
function :  Turn On Display
parameter:
******************************************************************************/
static void EPD_2IN66G_TurnOnDisplay(void)
{
    EPD_2IN66G_SendCommand(0x12); // DISPLAY_REFRESH
    EPD_2IN66G_SendData(0x01);
    EPD_2IN66G_ReadBusyH();
}

/******************************************************************************
function :  Initialize the e-Paper register
parameter:
******************************************************************************/
void EPD_2IN66G_Init(void)
{
    EPD_2IN66G_Reset();
    EPD_2IN66G_ReadBusyH();

    EPD_2IN66G_SendCommand(0x4D);
    EPD_2IN66G_SendData(0x78);

    EPD_2IN66G_SendCommand(0x00);   //PSR
    EPD_2IN66G_SendData(0x0F);
    EPD_2IN66G_SendData(0x29);

    EPD_2IN66G_SendCommand(0x01);   //PWRR
    EPD_2IN66G_SendData(0x07);
    EPD_2IN66G_SendData(0x00);

    EPD_2IN66G_SendCommand(0x03);   //POFS
    EPD_2IN66G_SendData(0x10);
    EPD_2IN66G_SendData(0x54);
    EPD_2IN66G_SendData(0x44);

    EPD_2IN66G_SendCommand(0x06);   //BTST_P
    EPD_2IN66G_SendData(0x05);
    EPD_2IN66G_SendData(0x00);
    EPD_2IN66G_SendData(0x3F);
    EPD_2IN66G_SendData(0x0A);
    EPD_2IN66G_SendData(0x25);
    EPD_2IN66G_SendData(0x12);
    EPD_2IN66G_SendData(0x1A);

    EPD_2IN66G_SendCommand(0x50);   //CDI
    EPD_2IN66G_SendData(0x37);

    EPD_2IN66G_SendCommand(0x60);   //TCON
    EPD_2IN66G_SendData(0x02);
    EPD_2IN66G_SendData(0x02);

    EPD_2IN66G_SendCommand(0x61);   //TRES
    EPD_2IN66G_SendData(EPD_2IN66G_WIDTH/256);      // Source_BITS_H
    EPD_2IN66G_SendData(EPD_2IN66G_WIDTH%256);      // Source_BITS_L
    EPD_2IN66G_SendData(EPD_2IN66G_HEIGHT/256);     // Gate_BITS_H
    EPD_2IN66G_SendData(EPD_2IN66G_HEIGHT%256);     // Gate_BITS_L

    EPD_2IN66G_SendCommand(0xE7);
    EPD_2IN66G_SendData(0x1C);

    EPD_2IN66G_SendCommand(0xE3);
    EPD_2IN66G_SendData(0x22);

    EPD_2IN66G_SendCommand(0xB4);
    EPD_2IN66G_SendData(0xD0);
    EPD_2IN66G_SendCommand(0xB5);
    EPD_2IN66G_SendData(0x03);

    EPD_2IN66G_SendCommand(0xE9);
    EPD_2IN66G_SendData(0x01);

    EPD_2IN66G_SendCommand(0x30);
    EPD_2IN66G_SendData(0x08);

    EPD_2IN66G_SendCommand(0x04);   //PWR on
    EPD_2IN66G_ReadBusyH();
}

/******************************************************************************
function :  Clear screen
parameter:
******************************************************************************/
void EPD_2IN66G_Clear(uint8_t color)
{
    uint16_t Width, Height;
    Width = (EPD_2IN66G_WIDTH % 4 == 0)? (EPD_2IN66G_WIDTH / 4 ): (EPD_2IN66G_WIDTH / 4 + 1);
    Height = EPD_2IN66G_HEIGHT;

    EPD_2IN66G_SendCommand(0x10);
    for (uint16_t j = 0; j < Height; j++) {
        for (uint16_t i = 0; i < Width; i++) {
            EPD_2IN66G_SendData((color << 6) | (color << 4) | (color << 2) | color);
        }
    }

    EPD_2IN66G_TurnOnDisplay();
}

/******************************************************************************
function :  Sends the image buffer in RAM to e-Paper and displays
parameter:  Image: 2 bits per pixel, 4 pixels per byte, MSB first
******************************************************************************/
void EPD_2IN66G_Display(uint8_t *Image)
{
    uint16_t Width, Height;
    Width = (EPD_2IN66G_WIDTH % 4 == 0)? (EPD_2IN66G_WIDTH / 4 ): (EPD_2IN66G_WIDTH / 4 + 1);
    Height = EPD_2IN66G_HEIGHT;

    EPD_2IN66G_SendCommand(0x10);
    for (uint16_t j = 0; j < Height; j++) {
        for (uint16_t i = 0; i < Width; i++) {
            EPD_2IN66G_SendData(Image[i + j * Width]);
        }
    }

    EPD_2IN66G_TurnOnDisplay();
}

/******************************************************************************
function :  Sends an OEPL two-plane image to e-Paper and displays
parameter:  plane0: 1 for white/red, plane1: 1 for yellow/red, 1 bit per pixel
            each; plane1 may be NULL (black/white only). The color index
            plane0 | plane1 << 1 matches the panel's color codes.
******************************************************************************/
void EPD_2IN66G_DisplayPlanes(const uint8_t *plane0, const uint8_t *plane1)
{
    const uint32_t pixels = (uint32_t)EPD_2IN66G_WIDTH * EPD_2IN66G_HEIGHT;

    EPD_2IN66G_SendCommand(0x10);
    uint8_t out = 0;
    for (uint32_t p = 0; p < pixels; p++) {
        uint8_t mask = 0x80 >> (p % 8);
        uint8_t color = (plane0[p / 8] & mask) ? 1 : 0;
        if (plane1 && (plane1[p / 8] & mask)) color |= 2;
        out = (out << 2) | color;
        if (p % 4 == 3) {
            EPD_2IN66G_SendData(out);
            out = 0;
        }
    }

    EPD_2IN66G_TurnOnDisplay();
}

/******************************************************************************
function :  Enter sleep mode
parameter:
******************************************************************************/
void EPD_2IN66G_Sleep(void)
{
    EPD_2IN66G_SendCommand(0x02); // POWER_OFF
    EPD_2IN66G_SendData(0X00);
    EPD_2IN66G_ReadBusyH();

    EPD_2IN66G_SendCommand(0x07); // DEEP_SLEEP
    EPD_2IN66G_SendData(0XA5);
}
