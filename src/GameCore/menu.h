//
// Created by nathanial on 2/24/26.
//
#pragma once
#include "types.h"


uint16_t* MainMenuTilePtr(uint16_t tile_id);

bool MenuBack(MemoryInterface memory);
void GetMenuLine(MemoryInterface memory, char* text, uint8_t idx);
uint8_t GetSelectorY(void);
uint8_t GetSelectorX(void);
uint16_t ListSize(uint16_t n);

bool SetMenuDelta(HardwareInterface hardware, InputInterface input, MemoryInterface memory, Delta delta);
bool HandleMenuOverflow(HardwareInterface hardware, InputInterface input, MemoryInterface memory, Delta delta);

bool ToggleMenu(SubMainMenuWindow menuWin, uint8_t numMenuOptions);
bool Back(SubMainMenuWindow menuWin);
void ClearMenu(void);

bool ListJump(HardwareInterface hardware, InputInterface input, MemoryInterface memory);
void FillListByEntityID(MemoryInterface memory, uint8_t n, uint8_t type, const uint8_t* e_ids);
void FillListByTypeID(MemoryInterface memory, uint8_t n, uint8_t* ids);
