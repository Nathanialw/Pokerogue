//
// Created by nathanial on 5/2/26.
//
#pragma once

/**********************************************************************************************************************/
/**  GRAPHICS PLATFORM DEFINED
**********************************************************************************************************************/
uint16_t* GetFrameBufferFront(void);
uint16_t* GetFrameBufferBack(void);
uint8_t* GetFrameBuffer1byte(void);
uint16_t* GetFrameBuffer2bytes(void);
uint16_t GetBufferWidth(void);
uint16_t GetBufferHeight(void);

void ClearBuffer(void);
void DrawBuffer(const FrameBuffer f);
void DrawSprite(FrameBuffer f, const uint8_t* sprite);
void DrawToBuffer(const FrameBuffer frameBuffer, const uint16_t* pixels, const Rect_16 rect);
void DrawTileKeyed(uint16_t x, uint16_t y, uint16_t w, uint16_t h, const uint16_t* data);
void Draw(uint16_t x, uint16_t y, uint16_t w, uint16_t h, const uint8_t* data);

void SetBuffer(uint16_t length, uint16_t* p, uint16_t rgb565);
void SetBufferColor(uint16_t length, uint16_t* p, Color rgb565);
void SetFrameBuffer(uint16_t rgb565);
void SetFrameBufferColor(Color rgb565);
void FillRect(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t rgb565);
void FillRectColor(uint16_t x, uint16_t y, uint16_t w, uint16_t h, Color rgb565);
void FillScreen(uint16_t rgb565);
void FillScreenColor(Color rgb565);

void TestAnimation(FrameBuffer f, Rect_16 r, uint16_t color1);


/**********************************************************************************************************************/
/**  INPUT PLATFORM DEFINED
**********************************************************************************************************************/
KeyState GetInputKeyState(void);
bool GetButtonA(void);
bool GetButtonB(void);
bool GetButtonY(void);
bool GetButtonX(void);
bool GetButtonSelect(void);
bool GetButtonStart(void);
bool GetButtonUp(void);
bool GetButtonDown(void);
bool GetButtonLeft(void);
bool GetButtonRight(void);
bool GetButtonJSClick(void);
bool GetButtonDPClick(void);