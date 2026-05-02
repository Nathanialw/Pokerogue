//
// Created by nathanial on 3/25/26.
//
#pragma once
#include "hardware/spi.h"
#include "hardware/i2c.h"

/**********************************************************************************************************************/
/**
**********************************************************************************************************************/
#define SN74HC165N_BITBANG
// #define SN74HC165N_SPI
// #define SN74HC165N_PIO

#define ILI9341
// #define ST7796S

#define DISPLAY_MODE_SPI
// #define DISPLAY_MODE_8BIT


#define SWAP_COLOR_BYTES




#define SPI_DISPLAY  spi1
#define SPI_BUTTONS  spi0

#define MAX_VOLUME  26500
#define MIN_VOLUME  500
#define I2C_PORT    i2c0


/**********************************************************************************************************************/
/**  battery status
**********************************************************************************************************************/
#define LED         25      // low battery light

/**********************************************************************************************************************/
/**  button inputs
**********************************************************************************************************************/
// #define LEFTBTN         1
// #define RIGHTBTN        2
// #define DOWNBTN         3
// #define UPBTN           4
//
// #define DCLICKBTN   5
// // #define JSCLICKBTN
//
// #define WHITEBTN        8
// #define BLUEBTN         6
// #define REDBTN          7
// #define GREENBTN        0
//
/**********************************************************************************************************************/
/**  analog
**********************************************************************************************************************/
#define JSX         26
#define JSY         27
#define BATTERY     28

/**********************************************************************************************************************/
/**  speaker controller
**********************************************************************************************************************/
#define DIN         17
#define BCLK        18
#define LRC         19

// ADS1115
#define ADS_SDA     20
#define ADS_SCL     21

#define BL_PWM     22


#if defined(DISPLAY_MODE_8BIT)
// #define TFT_WR      20
#define TFT_RS      23
#define TFT_RST     24
#endif

//  ILI9341 lcd display
#if defined(DISPLAY_MODE_8BIT)
#define TFT_D0      9
#define TFT_D1      10
#define TFT_D2      11
#define TFT_D3      12
#define TFT_D4      13
#define TFT_D5      14
#define TFT_D6      15
#define TFT_D7      16
#elif defined(DISPLAY_MODE_SPI)
#define CS          9
#define SCK         10
#define MOSI        11
#define RST         12
#define DC          13
// #define UNUSED      14
// #define UNUSED      15
// #define UNUSED      16
// #define UNUSED      23
// #define UNUSED      24
#else
#error "Define DISPLAY_MODE_8BIT or DISPLAY_MODE_SPI"
#endif

#define MODE_SPI_DISPLAY


//

// /**********************************************************************************************************************/
// /**  battery status
// **********************************************************************************************************************/
// // #define LED         3      // low battery light
//
// /**********************************************************************************************************************/
// /**  74HC165N button inputs
// **********************************************************************************************************************/
#define SN74HC165N_SH_LD   0
#define SN74HC165N_SCLK     1
#define SN74HC165N_MISO   2
//
// /**********************************************************************************************************************/
// /**  Cartridge SPI
// /**********************************************************************************************************************/
// #define CART_SCK        3
// #define CART_MOSI       4
// #define CART_MISO       5
// #define CART_CS_EEPROM  6
// #define CART_CS_FRAM    7
//
// /**********************************************************************************************************************/
// /**  PSRAM QSPI
// /**********************************************************************************************************************/
// #define PSRAM_SCK   8
// #define PSRAM_CS    9
// #define PSRAM_0     10
// #define PSRAM_1     11
// #define PSRAM_3     12
// #define PSRAM_2     13
// /**********************************************************************************************************************/
// /** ADS1115 - volume knob, battery voltage, jsX, jsY
// /**********************************************************************************************************************/
// #define ADS_SDA     14
// #define ADS_SCL     15
//
// /**********************************************************************************************************************/
// /**  ILI9341 lcd display
// /**********************************************************************************************************************/
// #if defined(DISPLAY_MODE_8BIT)
// #define TFT_RST     27
// #define TFT_RS      28
// #define TFT_WR      29
//
// #define TFT_D0      16
// #define TFT_D1      17
// #define TFT_D2      18
// #define TFT_D3      19
// #define TFT_D4      20
// #define TFT_D5      21
// #define TFT_D6      22
// #define TFT_D7      23
// #elif defined(DISPLAY_MODE_SPI)
// #define CS          19
// #define SCK         20
// #define MOSI        21
// #define RST         22
// #define DC          23
// #else
// #error "Define DISPLAY_MODE_8BIT or DISPLAY_MODE_SPI"
// #endif
//
// /**********************************************************************************************************************/
// /**  98357A speaker controller
// **********************************************************************************************************************/
// #define DIN         24
// #define BCLK        25
// #define LRC         26
//
//
//
// /**********************************************************************************************************************/
