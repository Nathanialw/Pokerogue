#ifdef STANDALONE
#include "game_state.h"
#endif
#include "cartridge.h"
#include "cartridge_rom.h"
#include "sounds.h"
#include "ili9341.h"
#include "init.h"
#include "input.h"
#include "lib_debugging.h"
#include "memory_ram.h"
#include "pico/multicore.h"
#include "pico/time.h"

/**********************************************************************************************************************/
/*
**********************************************************************************************************************/

#define CORE_VMA    0x20000000
#define OVERLAY_VMA 0x20010000
#define END_VMA     0x20020000

int main()
{
    Pico_Init();
    InitRam();
    sleep_ms(1000);;
    // Pico_AudioInit(); // Initialize audio system (this must be done on core 0)

    GameInterface api;
    api.hardware = GetHardwareInterface();
    api.input = GetInputInterface();
    api.graphics = GetGraphicsInterface();
    api.audio = SetAudioInterface();
    api.memory = GetMemoryInterface();

    Pico_TestColors();

    while (1)
    {
        api.input.HandleInput();
        if (api.input.GetButtonB())
        {
            uint32_t size = 4 * 1024 * 1024;
            EEPROM_Clear(size); // erase whole chip first
            printf("Erased.\n");
            EEPROM_Flash();
            break;
        }
        if (api.input.GetButtonA())
        {
            break;
        }
    }

    // load overlay table - save it into ram
    // on return jump to new overlay table
    // common data will be stored in psram
    // save data in psram
    // static data in eeprom

    uint8_t n;
    uint16_t s = sizeof(Overlay);
    EEPROM_Read(0, &n, 1);
    DEBUG("EEPROM: %d %d %d", n, s, n*s);
    EEPROM_Read(1, g_pico_ram.overlays.bytes, n * s);

    for (int i = 0; i < n; i++)
    {
        DEBUG("addr: %lu size: %lu", (g_pico_ram.overlays.overlay[i].addr), (g_pico_ram.overlays.overlay[i].size));
    }

    uint8_t overlay_idx = 0;
    LoadOverlay(g_pico_ram.overlays.overlay[overlay_idx].addr, CORE_VMA, g_pico_ram.overlays.overlay[overlay_idx].size);
    DEBUG("Reserved Core    RAM size: %d bytes - Used: %d bytes", OVERLAY_VMA - CORE_VMA, g_pico_ram.overlays.overlay[overlay_idx].size);
    if (g_pico_ram.overlays.overlay[overlay_idx].size >= OVERLAY_VMA - CORE_VMA) DEBUG("ERROR CORE EXCEEDS RESERVED SPACE");

    // uint32_t overlay_table_addr = 0x20000000 + g_pico_ram.overlays.overlay[overlay_idx].size;
    overlay_idx = 1;
    while (1)
    {
        DEBUG("Reserved Overlay RAM size: %d bytes - Used: %d bytes", END_VMA - OVERLAY_VMA, g_pico_ram.overlays.overlay[overlay_idx].size);
        overlay_idx = RunOverlay(&api, g_pico_ram.overlays.overlay[overlay_idx].addr, OVERLAY_VMA, g_pico_ram.overlays.overlay[overlay_idx].size);
        if (overlay_idx == 0)
        {
            DEBUG("OVERLAY RESERVED FOR CORE CODE, %d", overlay_idx);
            break;
        }
    }

    DEBUG("Program Done!~");

#ifdef STANDALONE
    // multicore_launch_core1(ThreadTwo, api.hardware);
    GameLoop(&api);
#endif
}
