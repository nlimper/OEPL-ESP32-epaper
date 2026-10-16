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

#ifdef EPD_266_G16
/******************************************************************************
16-colour mode: three refreshes with waveforms loaded into the controller
(LUT_EN=1, command 0x20). Pass 1 sets the base colour (orange/white/yellow/red),
passes 2 and 3 edit it (to black/keep/lighter/darker) at two VSN_1 levels.
The waveforms (include/EPD_2in66g_waves.h) are the factory waveforms of a
2.66" BWRY electronic shelf label; see README.md for the sources.
******************************************************************************/
#include "EPD_2in66g_waves.h"

#define EPD_2IN66G_WAVE_SIZE 535
#define EPD_2IN66G_REFRESH_TIMEOUT_MS 60000

// palette index (as in resources/tagtypes/FD.json) -> pass 1 value, pass 2 value, pass 3 value
// pass 1: 0 orange, 1 white, 2 yellow, 3 red; passes 2/3: 0 black, 1 keep, 2 lighter, 3 darker
static const uint8_t EPD_2IN66G_PALETTE16[16][3] = {
    {2, 0, 0},  // black
    {1, 1, 1},  // white
    {2, 1, 1},  // yellow
    {3, 1, 1},  // red
    {0, 1, 1},  // orange
    {1, 0, 3},  // stone grey
    {1, 2, 1},  // slate grey
    {2, 0, 2},  // sand
    {2, 1, 3},  // olive
    {2, 3, 1},  // mocha
    {3, 1, 2},  // caramel
    {0, 2, 1},  // terracotta
    {0, 3, 1},  // brown
    {3, 2, 1},  // rust
    {0, 1, 0},  // dark brown
    {3, 0, 1},  // burgundy
};
static const uint8_t EPD_2IN66G_BAND_LIMITS[10] = {3, 6, 9, 12, 15, 20, 25, 30, 35, 127};
static const uint8_t EPD_2IN66G_VSN1[3] = {0, 0x14, 0x41};  // pass 2: -5.0 V, pass 3: -9.5 V (pass 1: from the header)

static bool EPD_2IN66G_WaitBusyH(uint32_t timeout_ms)
{
    uint32_t t = millis();
    while (!digitalRead(EPD_BUSY_PIN)) {
        if (millis() - t > timeout_ms) return false;
        delay_ms(5);
    }
    Serial.printf("e-Paper busy H release after %lu ms\n", millis() - t);
    return true;
}

// 3-wire read: command, then SDA turned around for the data bytes
static int8_t EPD_2IN66G_ReadTemperature(void)
{
    digitalWrite(EPD_DC_PIN, 0);
    digitalWrite(EPD_CS_PIN, 0);
    uint8_t c = 0x40;
    for (int i = 0; i < 8; i++) {
        digitalWrite(EPD_MOSI_PIN, (c & 0x80) ? 1 : 0);
        c <<= 1;
        digitalWrite(EPD_SCK_PIN, 1);
        digitalWrite(EPD_SCK_PIN, 0);
    }
    pinMode(EPD_MOSI_PIN, INPUT);
    digitalWrite(EPD_DC_PIN, 1);
    uint8_t t = 0;
    for (int i = 0; i < 8; i++) {
        digitalWrite(EPD_SCK_PIN, 1);
        delayMicroseconds(1);
        t = (t << 1) | digitalRead(EPD_MOSI_PIN);
        digitalWrite(EPD_SCK_PIN, 0);
        delayMicroseconds(1);
    }
    digitalWrite(EPD_CS_PIN, 1);
    pinMode(EPD_MOSI_PIN, OUTPUT);
    return (int8_t)t;
}

// A refresh that does not end ignores power off: reset the controller and run a plain power on/off,
// so it goes through its normal discharge sequence
static void EPD_2IN66G_Abort(void)
{
    Serial.println("e-Paper refresh timeout: reset");
    EPD_2IN66G_Reset();
    EPD_2IN66G_WaitBusyH(2000);
    EPD_2IN66G_SendCommand(0x04);
    EPD_2IN66G_WaitBusyH(5000);
    EPD_2IN66G_SendCommand(0x02);
    EPD_2IN66G_SendData(0x00);
    EPD_2IN66G_WaitBusyH(5000);
}

static bool EPD_2IN66G_Pass(const uint8_t *Image, int pass, const uint8_t *wave)
{
    uint8_t w[EPD_2IN66G_WAVE_SIZE];
    memcpy(w, wave, EPD_2IN66G_WAVE_SIZE);
    if (pass > 0) w[3] = EPD_2IN66G_VSN1[pass];

    EPD_2IN66G_Reset();
    EPD_2IN66G_WaitBusyH(2000);
    EPD_2IN66G_SendCommand(0x4D); EPD_2IN66G_SendData(0x78);
    EPD_2IN66G_SendCommand(0xAE); EPD_2IN66G_SendData(0x0F);
    EPD_2IN66G_SendCommand(0xB6); EPD_2IN66G_SendData(0x0F);
    EPD_2IN66G_SendCommand(0xBA); EPD_2IN66G_SendData(0x2A);
    EPD_2IN66G_SendCommand(0x00);   //PSR: LUT_EN=1 (waveform from register), FOPT=0 (one GND frame after the waveform)
    EPD_2IN66G_SendData(0x0F);
    EPD_2IN66G_SendData(0x88);
    EPD_2IN66G_SendCommand(0x01);   //PWR: VGPN, VSPL_0, VSP_1, VSN_1, VSPL_1 from the waveform header
    EPD_2IN66G_SendData(0x07);
    for (int i = 0; i < 5; i++) EPD_2IN66G_SendData(w[i]);
    EPD_2IN66G_SendCommand(0x03);
    EPD_2IN66G_SendData(0x10);
    EPD_2IN66G_SendData(0x54);
    EPD_2IN66G_SendData(0x44);
    EPD_2IN66G_SendCommand(0x06);
    EPD_2IN66G_SendData(0x05);
    EPD_2IN66G_SendData(0x00);
    EPD_2IN66G_SendData(0x3F);
    EPD_2IN66G_SendData(0x0A);
    EPD_2IN66G_SendData(0x25);
    EPD_2IN66G_SendData(0x12);
    EPD_2IN66G_SendData(0x1A);
    EPD_2IN66G_SendCommand(0x30); EPD_2IN66G_SendData(w[6]);
    EPD_2IN66G_SendCommand(0x50);   //CDI: border follows pixel value 1 (white in pass 1, keep in passes 2 and 3)
    EPD_2IN66G_SendData(0x37);
    EPD_2IN66G_SendCommand(0x60);
    EPD_2IN66G_SendData(0x02);
    EPD_2IN66G_SendData(0x02);
    EPD_2IN66G_SendCommand(0x61);
    EPD_2IN66G_SendData(EPD_2IN66G_WIDTH/256);
    EPD_2IN66G_SendData(EPD_2IN66G_WIDTH%256);
    EPD_2IN66G_SendData(EPD_2IN66G_HEIGHT/256);
    EPD_2IN66G_SendData(EPD_2IN66G_HEIGHT%256);
    EPD_2IN66G_SendCommand(0x65);
    for (int i = 0; i < 4; i++) EPD_2IN66G_SendData(0x00);
    EPD_2IN66G_SendCommand(0x82); EPD_2IN66G_SendData(w[5] ^ 0x80);
    EPD_2IN66G_SendCommand(0xE3); EPD_2IN66G_SendData(0x22);
    EPD_2IN66G_SendCommand(0xE7); EPD_2IN66G_SendData(0x1C);
    EPD_2IN66G_SendCommand(0xE9); EPD_2IN66G_SendData(0x01);

    EPD_2IN66G_SendCommand(0x20);
    for (int i = 0; i < EPD_2IN66G_WAVE_SIZE; i++) EPD_2IN66G_SendData(w[i]);

    // 4-bit palette indices (2 pixels per byte, high nibble first) -> this pass's 2-bit values
    EPD_2IN66G_SendCommand(0x10);
    const uint32_t pixels = (uint32_t)EPD_2IN66G_WIDTH * EPD_2IN66G_HEIGHT;
    uint8_t out = 0;
    for (uint32_t p = 0; p < pixels; p++) {
        uint8_t idx = (Image[p / 2] >> ((p & 1) ? 0 : 4)) & 0x0F;
        out = (out << 2) | EPD_2IN66G_PALETTE16[idx][pass];
        if (p % 4 == 3) {
            EPD_2IN66G_SendData(out);
            out = 0;
        }
    }

    EPD_2IN66G_SendCommand(0x04);
    if (!EPD_2IN66G_WaitBusyH(5000)) { EPD_2IN66G_Abort(); return false; }
    EPD_2IN66G_SendCommand(0x12);
    EPD_2IN66G_SendData(0x00);
    if (!EPD_2IN66G_WaitBusyH(EPD_2IN66G_REFRESH_TIMEOUT_MS)) { EPD_2IN66G_Abort(); return false; }
    EPD_2IN66G_SendCommand(0x02);
    EPD_2IN66G_SendData(0x00);
    EPD_2IN66G_WaitBusyH(5000);
    return true;
}

/******************************************************************************
function :  Display a 16-colour image (4 bits per pixel, palette of FD.json) in three refreshes
******************************************************************************/
void EPD_2IN66G_Display16(const uint8_t *Image)
{
    EPD_2IN66G_Reset();
    EPD_2IN66G_WaitBusyH(2000);
    int8_t temp = EPD_2IN66G_ReadTemperature();
    int band = 0;
    while (band < 9 && temp > (int8_t)EPD_2IN66G_BAND_LIMITS[band]) band++;
    Serial.printf("e-Paper temperature %d C, waveform band %d\n", temp, band);

    // pass 1: colour waveform; passes 2 and 3: edit waveform
    for (int pass = 0; pass < 3; pass++) {
        if (!EPD_2IN66G_Pass(Image, pass, EPD_2IN66G_WAVES[band][pass == 0 ? 0 : 1])) break;
    }
    EPD_2IN66G_SendCommand(0x07); // DEEP_SLEEP
    EPD_2IN66G_SendData(0xA5);
}
#endif
