/*****************************************************************************
* | File      	:   EPD_4in01f.c
* | Author      :   Waveshare team
* | Function    :   4.01inch e-paper
* | Info        :
*----------------
* |	This version:   V1.0
* | Date        :   2020-12-29
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
#include "EPD_4in01f.h"

/******************************************************************************
function :	Software reset
parameter:
******************************************************************************/
static void EPD_4IN01F_Reset(void)
{
    digitalWrite(EPD_RST_PIN, 1);
    delay_ms(200);
    digitalWrite(EPD_RST_PIN, 0);
    delay_ms(1);
    digitalWrite(EPD_RST_PIN, 1);
    delay_ms(200);
}

/******************************************************************************
function :	send command
parameter:
     Reg : Command register
******************************************************************************/
static void EPD_4IN01F_SendCommand(uint8_t Reg)
{
    digitalWrite(EPD_DC_PIN, 0);
    digitalWrite(EPD_CS_PIN, 0);
    SPI_WriteByte(Reg);
    digitalWrite(EPD_CS_PIN, 1);
}

/******************************************************************************
function :	send data
parameter:
    Data : Write data
******************************************************************************/
static void EPD_4IN01F_SendData(uint8_t Data)
{
    digitalWrite(EPD_DC_PIN, 1);
    digitalWrite(EPD_CS_PIN, 0);
    SPI_WriteByte(Data);
    digitalWrite(EPD_CS_PIN, 1);
}


static void EPD_4IN01F_BusyHigh(void)// If BUSYN=0 then waiting
{
    while(!(digitalRead(EPD_BUSY_PIN))) {
		delay_ms(1);
	};
}

static void EPD_4IN01F_BusyLow(void)// If BUSYN=1 then waiting
{
    while(digitalRead(EPD_BUSY_PIN)) {
		delay_ms(1);
	};
}

/******************************************************************************
function :	Initialize the e-Paper register
parameter:
******************************************************************************/
void EPD_4IN01F_Init(void)
{
	EPD_4IN01F_Reset();
    EPD_4IN01F_BusyHigh();
    EPD_4IN01F_SendCommand(0x00);
    EPD_4IN01F_SendData(0x2f);
    EPD_4IN01F_SendData(0x00);
    EPD_4IN01F_SendCommand(0x01);
    EPD_4IN01F_SendData(0x37);
    EPD_4IN01F_SendData(0x00);
    EPD_4IN01F_SendData(0x05);
    EPD_4IN01F_SendData(0x05);
    EPD_4IN01F_SendCommand(0x03);
    EPD_4IN01F_SendData(0x00);
    EPD_4IN01F_SendCommand(0x06);
    EPD_4IN01F_SendData(0xC7);
    EPD_4IN01F_SendData(0xC7);
    EPD_4IN01F_SendData(0x1D);
    EPD_4IN01F_SendCommand(0x41);
    EPD_4IN01F_SendData(0x00);
    EPD_4IN01F_SendCommand(0x50);
    EPD_4IN01F_SendData(0x37);
    EPD_4IN01F_SendCommand(0x60);
    EPD_4IN01F_SendData(0x22);
    EPD_4IN01F_SendCommand(0x61);
    EPD_4IN01F_SendData(0x02);
    EPD_4IN01F_SendData(0x80);
    EPD_4IN01F_SendData(0x01);
    EPD_4IN01F_SendData(0x90);
    EPD_4IN01F_SendCommand(0xE3);
    EPD_4IN01F_SendData(0xAA);
}

/******************************************************************************
function :	Clear screen
parameter:
******************************************************************************/
void EPD_4IN01F_Clear(uint8_t color)
{
    EPD_4IN01F_SendCommand(0x61);//Set Resolution setting
    EPD_4IN01F_SendData(0x02);
    EPD_4IN01F_SendData(0x80);
    EPD_4IN01F_SendData(0x01);
    EPD_4IN01F_SendData(0x90);
    EPD_4IN01F_SendCommand(0x10);
    for(int i=0; i<EPD_4IN01F_HEIGHT; i++) {
        for(int j=0; j<EPD_4IN01F_WIDTH/2; j++)
            EPD_4IN01F_SendData((color<<4)|color);
    }
    EPD_4IN01F_SendCommand(0x04);//0x04
    EPD_4IN01F_BusyHigh();
    EPD_4IN01F_SendCommand(0x12);//0x12
    EPD_4IN01F_BusyHigh();
    EPD_4IN01F_SendCommand(0x02);  //0x02
    EPD_4IN01F_BusyLow();
    delay_ms(500);
}

/******************************************************************************
function :	Sends the image buffer in RAM to e-Paper and displays
parameter:
******************************************************************************/
void EPD_4IN01F_Display(const uint8_t *image)
{
    uint16_t i, j, WIDTH;
	uint8_t Rdata, Rdata1, shift;
	uint32_t Addr;
	WIDTH = EPD_4IN01F_WIDTH*3%8 == 0 ? EPD_4IN01F_WIDTH*3/8 : EPD_4IN01F_WIDTH*3/8+1;
    EPD_4IN01F_SendCommand(0x61);//Set Resolution setting
    EPD_4IN01F_SendData(0x02);
    EPD_4IN01F_SendData(0x80);
    EPD_4IN01F_SendData(0x01);
    EPD_4IN01F_SendData(0x90);
    EPD_4IN01F_SendCommand(0x10);
    for(i=0; i<EPD_4IN01F_HEIGHT; i++) {
        for(j=0; j<EPD_4IN01F_WIDTH/2; j++) {
					shift = (j + i*EPD_4IN01F_WIDTH/2) % 4;
					Addr = (j*3/4 + i * WIDTH);

					if(shift == 0) {
						Rdata = image[Addr];
						EPD_4IN01F_SendData(((Rdata >> 1) & 0x70) | ((Rdata >> 2) & 0x07));
					}
					else if(shift == 1) {
						Rdata = image[Addr];
						Rdata1 = image[Addr + 1];
						EPD_4IN01F_SendData(((Rdata << 5) & 0x60) | ((Rdata1 >> 3) & 0x10) | ((Rdata1 >> 4) & 0x07));
					}
					else if(shift == 2) {
						Rdata = image[Addr];
						Rdata1 = image[Addr + 1];
						EPD_4IN01F_SendData(((Rdata << 3) & 0x70) | ((Rdata << 2) & 0x04) | ((Rdata1 >> 6) & 0x03));
					}
					else if(shift == 3) {
						Rdata = image[Addr];
						EPD_4IN01F_SendData(((Rdata << 1) & 0x70) | (Rdata & 0x07));
					}
				}
    }
    EPD_4IN01F_SendCommand(0x04);//0x04
    EPD_4IN01F_BusyHigh();
    EPD_4IN01F_SendCommand(0x12);//0x12
    EPD_4IN01F_BusyHigh();
    EPD_4IN01F_SendCommand(0x02);  //0x02
    EPD_4IN01F_BusyLow();
	delay_ms(200);
	
}

/******************************************************************************
function :	Sends the part image buffer in RAM to e-Paper and displays
parameter:
******************************************************************************/
void EPD_4IN01F_Display_part(const uint8_t *image, uint16_t xstart, uint16_t ystart, 
									uint16_t image_width, uint16_t image_heigh)
{
    unsigned long i,j;
    EPD_4IN01F_SendCommand(0x61);//Set Resolution setting
    EPD_4IN01F_SendData(0x02);
    EPD_4IN01F_SendData(0x80);
    EPD_4IN01F_SendData(0x01);
    EPD_4IN01F_SendData(0x90);
    EPD_4IN01F_SendCommand(0x10);
    for(i=0; i<EPD_4IN01F_HEIGHT; i++) {
        for(j=0; j< EPD_4IN01F_WIDTH/2; j++) {
						if(i<image_heigh+ystart && i>=ystart && j<(image_width+xstart)/2 && j>=xstart/2) {
							EPD_4IN01F_SendData(image[(j-xstart/2) + (image_width/2*(i-ystart))]);
						}
						else {
							EPD_4IN01F_SendData(0x11);
						}
				}
    }
    EPD_4IN01F_SendCommand(0x04);//0x04
    EPD_4IN01F_BusyHigh();
    EPD_4IN01F_SendCommand(0x12);//0x12
    EPD_4IN01F_BusyHigh();
    EPD_4IN01F_SendCommand(0x02);  //0x02
    EPD_4IN01F_BusyLow();
	delay_ms(200);
	
}

/******************************************************************************
function :	Enter sleep mode
parameter:
******************************************************************************/
void EPD_4IN01F_Sleep(void)
{
    delay_ms(100);
    EPD_4IN01F_SendCommand(0x07);
    EPD_4IN01F_SendData(0xA5);
}

