//
// Created by nathanial on 5/13/26.
//
#pragma once
#include <stddef.h>
#include <stdint.h>

#include "lib_decl.h"

bool LoadOverlay(uint32_t lma, uint32_t ram_addr, size_t size);
uint8_t RunOverlay(GameInterface* spi, uint32_t lma, uint32_t ram_addr, size_t size);

MemoryInterface GetMemoryInterface();
void Print(const char* fmt, ...);
void PrintVar(const char* fmt, ...);