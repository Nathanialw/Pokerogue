//
// Created by nathanial on 5/13/26.
//

#include "cartridge.h"
#include <stdint.h>
#include <string.h>

#include "cartridge_rom.h"
#include "lib_debugging.h"
#include "lib_decl.h"
#include "pico/time.h"

// Load an overlay by name, returns true on success
bool LoadOverlay(uint32_t lma, uint32_t ram_addr, size_t size)
{
    if (size == 0) return false;

    // Destination must fit in RAM – your linker script already reserves space
    // EEPROM_Read copies 'size' bytes from SPI flash offset 'lma' to 'ram_addr'
    memset((void*)ram_addr, 0, 4096);
    EEPROM_Read(lma, (uint8_t*)ram_addr, size);
    return true;
}

typedef uint8_t (*overlay_entry_t)(GameInterface*);

uint8_t RunOverlay(GameInterface *spi, uint32_t lma, uint32_t ram_addr, size_t size)
{
     // 1. Copy from EEPROM (offset 0x080000) to RAM OVERLAY_VMA
    DEBUG("Loading overlay from 0x%08X to 0x%08X", lma, ram_addr);
    // load_overlay(TITLE_LMA, OVERLAY_VMA, TITLE_SIZE);
    LoadOverlay(lma, ram_addr, size);

    // 2. Execute the entry function (Thumb bit = 1)
    overlay_entry_t entry = (overlay_entry_t)(ram_addr | 1);
    DEBUG("Running overlay at 0x%08X", (uint32_t)entry);
    return entry(spi);
}

void EnterBootloader()
{
}
