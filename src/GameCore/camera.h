//
// Created by nathanial on 2/20/26.
//
#pragma once
#include "types.h"
#include "lib_types.h"

Camera InitCamera(uint8_t x, uint8_t y, uint16_t w, uint16_t h);
Camera SetCamera(uint8_t x, uint8_t y);
void SetCameraPlayer(void);
Camera GetCamera(void);
bool CameraContains(uint8_t x, uint8_t y);
