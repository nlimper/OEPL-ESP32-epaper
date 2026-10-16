/*****************************************************************************
* | File        :   EPD_2in66g.h
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
#ifndef __EPD_2IN66G_H_
#define __EPD_2IN66G_H_

#include "EPDcommon.h"

// Display resolution (native, portrait)
#define EPD_2IN66G_WIDTH       184
#define EPD_2IN66G_HEIGHT      360

/**********************************
Color Index
**********************************/
#define EPD_2IN66G_BLACK   0x0   /// 00
#define EPD_2IN66G_WHITE   0x1   /// 01
#define EPD_2IN66G_YELLOW  0x2   /// 10
#define EPD_2IN66G_RED     0x3   /// 11

void EPD_2IN66G_Init(void);
void EPD_2IN66G_Clear(uint8_t color);
void EPD_2IN66G_Display(uint8_t *Image);
void EPD_2IN66G_DisplayPlanes(const uint8_t *plane0, const uint8_t *plane1);
void EPD_2IN66G_Sleep(void);

#endif
