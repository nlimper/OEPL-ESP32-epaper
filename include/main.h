#include <Arduino.h>


// *** select your display type here (or with a build flag in platformio.ini)
#if !defined(EPD_401_ACEP) && !defined(EPD_730_SPECTRA) && !defined(EPD_266_G)
// #define EPD_401_ACEP
#define EPD_730_SPECTRA
// #define EPD_266_G
#endif
// ***************************



#ifdef EPD_401_ACEP
#define HWTYPE 0xC1 // ACEP
#define IMGWIDTH 640
#define IMGHEIGHT 400
#endif

#ifdef EPD_730_SPECTRA
// board with psram needed
#define HWTYPE 0xC2 // Spectra
#define IMGWIDTH 800
#define IMGHEIGHT 480
#endif

#if defined(EPD_266_G) && defined(EPD_266_G16)
#define HWTYPE 0xFD // Waveshare 2.66" (G), 16 colours in three refreshes
#define IMGWIDTH 360
#define IMGHEIGHT 184
#elif defined(EPD_266_G)
#define HWTYPE 0xFE // Waveshare 2.66" BWRY (G)
#define IMGWIDTH 360
#define IMGHEIGHT 184
#endif

void drawImage(uint8_t *buffer, uint8_t dataType);
