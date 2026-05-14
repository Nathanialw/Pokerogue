//
// Created by nathanial on 2/20/26.
//
#include "init.h"

#include <stdlib.h>

#include "lib_debugging.h"

#include "pico/stdlib.h"
#include "pico/bootrom.h"
#include "hardware/adc.h"
#include "hardware/clocks.h"
#include "hardware/i2c.h"

#include "string.h"

#include "ili9341.h"
#include "input.h"
#include "hardware.h"
#include "pico_constants.h"
#include "cartridge_save.h"
#include "cartridge_rom.h"
#include "memory_psram.h"
#include "cartridge.h"
#include "pico/multicore.h"

#define PICO

void InitBtn(int btn)
{
    gpio_init(btn);
    gpio_set_dir(btn, GPIO_IN);
    gpio_pull_up(btn);
}


void SleepMS(uint32_t t);
void HardwareReset(void);
uint8_t GetRandomUniform(uint8_t min, uint8_t max);
uint8_t GetRandom_uint8_t(uint8_t min, uint8_t max);

HardwareInterface GetHardwareInterface()
{
    HardwareInterface hardware =
    {
        .HardwareReset = HardwareReset,
        .SleepMS = SleepMS,
        .MemSet = memset,
        .GetRandomUniform = GetRandomUniform,
        .GetRandom_uint8_t = GetRandom_uint8_t,
        .StrChr = strchr,
    };

    return hardware;
}


void SleepMS(uint32_t t)
{
    sleep_ms(t);
}

/**********************************************************************************************************************/
/**  Triggers reset pin on the Pico
**********************************************************************************************************************/
bool Pico_Reset(void)
{
    reset_usb_boot(0, 0);
    return true;
}

void HardwareReset(void)
{
    Pico_Reset();
}

/**********************************************************************************************************************/
/** Returns a random int between given min and max
**********************************************************************************************************************/
uint8_t GetRandomUniform(uint8_t min, uint8_t max)
{
    uint8_t range = max - min + 1;
    uint8_t max_acceptable = (RAND_MAX / range) * range;

    uint8_t r;
    do
    {
        r = rand();
    }
    while (r >= max_acceptable);

    return min + (r % range);
}

/**********************************************************************************************************************/
/** Returns a random int between given min and max
**********************************************************************************************************************/
uint8_t GetRandom_uint8_t(uint8_t min, uint8_t max)
{
    return min + rand() % ((max - min) + 1);
}


/**********************************************************************************************************************/
/**  updates the battery data after each generated song
 *  routines to run on core 1
 *      -Audio
 *      -Battery status
 *  //TODO: more tasks, perhaps input polling
**********************************************************************************************************************/
void ThreadTwo(HardwareInterface hardware)
{
    // MusicData music_data = InitMusicData();
    while (1)
    {
        Pico_BatteryStatus();

        // if (g_run.music.master_volume <= MIN_VOLUME)
        // {
        //     CheckVolume();
        //     sleep_ms(200);
        // }
        // else
        // {
        // GenerateDungeonMelody(hardware, g_run.music.notes);
        // AudioPlayVoices(&music_data, g_run.music.notes);
        // }
    }
}


/**********************************************************************************************************************/
/**  Pico main init function
**********************************************************************************************************************/
void Pico_Init(void)
{
#ifdef ENABLE_DEBUG_OUTPUT
#include <stdio.h>
    stdio_init_all();
    sleep_ms(5000);
#endif

    uint32_t freq = clock_get_hz(clk_sys);
    DEBUG("Current CPU Frequency: %lu.%03lu MHz", freq / 1000000, (freq % 1000000) / 1000);
    gpio_init(LED);
    gpio_set_dir(LED, GPIO_OUT);

#if defined(SN74HC165N_SPI)
    SN74HC165N_Setup();
#elif defined(SN74HC165N_BITBANG)
    SN74HC165N_BitBangInit();
#elif defined(SN74HC165N_PIO)
#error "SN74HC165N_PIO not yet implemented"
#else
#error "Define SN74HC165N_BITBANG or SN74HC165N_SPI or SN74HC165N_PIO"
#endif

    SPI0_Init();
    InitCart();
    FRAM_Init();
    InitPSram();

    Backlight_Init();
    adc_init();
    adc_gpio_init(JSX); // VRx
    adc_gpio_init(JSY); // VRy
    adc_gpio_init(BATTERY); // VRy

    // Pico_ADS1115_Init();
    Pico_ili9341_Init();
    sleep_ms(200);

    // DEBUG("----- BEGIN TESTS ---");
    // EEPROM_FullTest();
    // DEBUG("----- EEPROM Test Complete ---");
    // FRAM_FullTest();
    // DEBUG("----- FRAM Test Complete ---");
    // PSRAM_FullTest();
    // DEBUG("----- PSRAM Test Complete ---");
    //
    uint32_t size = 4 * 1024 * 1024;
    // EEPROM_VerifySize(size);
    // EEPROM_FullMemoryTest(size);
    // EEPROM_RetentionCheck(size);
    //
    // EEPROM_Clear(size); // erase whole chip first
    // printf("Erased.\n");
    // EEPROM_Flash();
    // //
    // EEPROM_Verify(size);
    // EEPROM_Dump(size);
}
