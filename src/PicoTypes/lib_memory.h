//
// Created by nathanial on 5/14/26.
//
#ifdef CARTRIDGE
#define SET_MEMORY(fmt) __attribute__((section(fmt), used, aligned(4)))
#else
#define SET_MEMORY(fmt)
#endif

