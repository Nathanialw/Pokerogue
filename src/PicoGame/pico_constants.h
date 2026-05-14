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


#define SPI_A       spi0
#define SPI_B       spi1


#define MAX_VOLUME  26500
#define MIN_VOLUME  500
#define ADS1115_I2C i2c1
#define FRAM_I2C    i2c0


/**********************************************************************************************************************/
/**  battery status
**********************************************************************************************************************/
#define LED         25      // low battery light

/**********************************************************************************************************************/
/**  button inputs
**********************************************************************************************************************/
#if defined(SN74HC165N_BITBANG)
#define SN74HC165N_SH_LD    0
#define SN74HC165N_SCLK     1
#define SN74HC165N_MISO     2
#elif defined(SN74HC165N_SPI)
#define SN74HC165N_SH_LD    0
#define SN74HC165N_SCLK     1
#define SN74HC165N_MISO     2
#elif defined(SN74HC165N_PIO)
// not currently used
#else
#error "Define SN74HC165N_BITBANG or SN74HC165N_SPI of SN74HC165N_PIO"
#endif

/**********************************************************************************************************************/
/**  SPI
/**********************************************************************************************************************/
#define SPIA_SCK      18
#define SPIA_MOSI       19
#define SPIA_MISO       16

/**********************************************************************************************************************/
/**  Cartridge I/O
/**********************************************************************************************************************/
#define FM24C256_SDA    20
#define FM24C256_SCL    21
//SPI 0
#define EEPROM_CART_CS 17

/**********************************************************************************************************************/
/**  psram
/**********************************************************************************************************************/
//SPI 0
#define ESP_PSRAM64_CS  15

/**********************************************************************************************************************/
/**  analog
**********************************************************************************************************************/
#define JSX         26
#define JSY         27
#define BATTERY     28

/**********************************************************************************************************************/
/**  speaker controller
**********************************************************************************************************************/
#define DIN         5
#define BCLK        6
#define LRC         7

// ADS1115
#define ADS_SDA     24
#define ADS_SCL     23

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
#define SPIB_SCK    10
#define SPIB_MOSI   11
#define RST         12
#define DC          13
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

//
// /**********************************************************************************************************************/
// /**  Cartridge SPI
// /**********************************************************************************************************************/
// #define SPI0_A_SCK        2
// #define CART_MOSI       3
// #define CART_MISO       4
// #define EEPROM_CART_CS  5
// #define CART_CS_FRAM    6
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


//
//
// #############################################################
// Pin layout:
//
// Display (PIO 8-bit) → GP0–GP9
// PSRAM (QSPI)        → GP10–GP15
// Flash (SPI)         → GP16–GP19
// I2C (FRAM+ADC)      → GP20–GP21
// Audio (I2S)         → GP22–GP24
// Buttons (165)       → GP25–GP27
// Backlight PWM       → GP28
// Battery LED         → GP29
//
// --------------------------------
//
// 8-bit Parallel LCD (ILI9341 etc.)
//     GP0  → LCD_D0
//     GP1  → LCD_D1
//     GP2  → LCD_D2
//     GP3  → LCD_D3
//     GP4  → LCD_D4
//     GP5  → LCD_D5
//     GP6  → LCD_D6
//     GP7  → LCD_D7
//
//     GP8  → LCD_WR
//     GP9  → LCD_DC
//
//     Fixed wiring (no GPIO):
//         LCD_CS  → GND
//         LCD_RD  → 3.3V
//         LCD_RST → 10k → 3.3V + 0.1µF → GND
//
// PSRAM (QSPI via PIO)
//     GP10 → PSRAM_CS
//     GP11 → PSRAM_CLK
//     GP12 → PSRAM_IO0
//     GP13 → PSRAM_IO1
//     GP14 → PSRAM_IO2
//     GP15 → PSRAM_IO3
//
// Cartridge Flash (SPI)
//     GP16 → FLASH_CS
//     GP17 → SPI_SCK
//     GP18 → SPI_MOSI
//     GP19 → SPI_MISO
//
// I2C Bus (FRAM + ADS1115)
//     GP20 → I2C_SDA
//     GP21 → I2C_SCL
//
//     Both devices share(4.7k pull-ups on SDA + SCL):
//         FRAM      → 0x50
//         ADS1115   → 0x48–0x4B
//
//
// I2S Audio (MAX98357A)
//     GP22 → I2S_BCLK
//     GP23 → I2S_LRCLK
//     GP24 → I2S_DIN
//
// Buttons (2× 74HC165 chained)
//     GP25 → 74HC165_LOAD (PL)
//     GP26 → 74HC165_CLK
//     GP27 → 74HC165_DATA
//
// Backlight PWM (screen dimmer)
//     GP28 → PWM → transistor/MOSFET → LCD LED (depends on screen pins)
//
// Battery Sense(use ADS1115)
//     Battery → divider → ADS1115 input
//
// Battery LED
//     GP29 → 1k resistor → LED → GND
//
