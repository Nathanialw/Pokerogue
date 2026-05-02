//
// Created by nathanial on 2/20/26.
//
#include "ili9341.h"
#include "pico/stdlib.h"
#include "pico/multicore.h"
#include "hardware/adc.h"

#include "input.h"
#include "pico_constants.h"

#include "init.h"

// #include "core.h"
#include "hardware.h"
// #include "memory_ram.h"
// #include "sound.h"
#include "lib_debugging.h"
#include "sounds.h"
#include "hardware/clocks.h"
#include "hardware/i2c.h"
#include "hardware/structs/clocks.h"

#define PICO

void InitBtn(int btn)
{
    gpio_init(btn);
    gpio_set_dir(btn, GPIO_IN);
    gpio_pull_up(btn);
}


void SleepMS(uint32_t t)
{
    sleep_ms(t);
}


/**********************************************************************************************************************/
/**  updates the battery data after each generated song
 *  routines to run on core 1
 *      -Audio
 *      -Battery status
 *  //TODO: more tasks, perhaps input polling
**********************************************************************************************************************/
void ThreadTwo()
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
        // GenerateDungeonMelody(g_run.music.notes);
        // AudioPlayVoices(&music_data, g_run.music.notes);
        // }
    }
}

void SN74HC165N_Setup()
{
    // Initialize SPI at 1 khz
    spi_init(SPI_BUTTONS, 100 * 1000);
    spi_set_format(SPI_BUTTONS, 8, SPI_CPOL_0, SPI_CPHA_1, SPI_MSB_FIRST);

    // Assign SPI pins
    gpio_set_function(SN74HC165N_SCLK, GPIO_FUNC_SPI);
    gpio_set_function(SN74HC165N_MISO, GPIO_FUNC_SPI);

    // SH/LD as GPIO output, initially high
    gpio_init(SN74HC165N_SH_LD);
    gpio_set_dir(SN74HC165N_SH_LD, GPIO_OUT);
    gpio_put(SN74HC165N_SH_LD, 1);
    DEBUG("SN74HC165N_Setup");
}

void SN74HC165N_BitBangInit()
{
    gpio_init(SN74HC165N_SH_LD);
    gpio_set_dir(SN74HC165N_SH_LD, GPIO_OUT);
    gpio_put(SN74HC165N_SH_LD, 1);

    gpio_init(SN74HC165N_SCLK);
    gpio_set_dir(SN74HC165N_SCLK, GPIO_OUT);
    gpio_put(SN74HC165N_SCLK, 0);

    gpio_init(SN74HC165N_MISO);
    gpio_set_dir(SN74HC165N_MISO, GPIO_IN);
    DEBUG("SN74HC165N_BITBANG init");
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
    // DEBUG("STARTUP!\n");
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

    Backlight_Init();
    adc_init();
    adc_gpio_init(JSX); // VRx
    adc_gpio_init(JSY); // VRy
    adc_gpio_init(BATTERY); // VRy

    Pico_ADS1115_Init();
    Pico_ili9341_Init();
    sleep_ms(200);

    // Initialize audio system (this must be done on core 0)
    Pico_AudioInit();

    multicore_launch_core1(ThreadTwo);
    // DEBUG("init complete - game on core 0,  audio running on core 1");
}
