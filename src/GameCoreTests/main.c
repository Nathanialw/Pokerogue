//
// Created by nathanial on 2/21/26.
//

#include "animation_tests.h"
#include "collision.h"
#include "combat_tests.h"
#include "core.h"
#include "creatures.h"
#include "effects.h"
#include "lib_debugging.h"
#include "memory_ram.h"
#include "utils_tests.h"

State HandleInput(State state)
{
    return state;
}


void SleepMS(uint32_t t)
{
};

void ClearBuffer()
{
};

void SetFrameBuffer(Color rgb565)
{
};

void DrawBuffer(FrameBuffer f)
{
};

void DrawSprite(FrameBuffer f, const uint8_t* sprite)
{
};

void DrawToBuffer(const FrameBuffer frameBuffer, const uint16_t* pixels, const Rect_16 rect)
{
};

void FillRect(uint16_t x, uint16_t y, uint16_t w, uint16_t h, Color rgb565)
{
};

void TestAnimation(FrameBuffer f, Rect_16 r, uint16_t color1)
{
};

void DrawTileKeyed(uint16_t x, uint16_t y, uint16_t w, uint16_t h, const uint16_t* data)
{
};

void Draw(uint16_t x, uint16_t y, uint16_t w, uint16_t h, const uint8_t* data)
{
};

void FillScreen(Color rgb565)
{
}

void PlaySoundEffect()
{
}

void SetBuffer()
{
}

bool GetButtonA()
{
    return false;
}

bool GetButtonB()
{
    return false;
}

bool GetButtonY()
{
    return false;
}

bool GetButtonX()
{
    return false;
}

bool GetButtonSelect()
{
    return false;
}

bool GetButtonStart()
{
    return false;
}

bool GetButtonUp()
{
    return false;
}

bool GetButtonDown()
{
    return false;
}

bool GetButtonLeft()
{
    return false;
}

bool GetButtonRight()
{
    return false;
}

bool GetButtonJSClick()
{
    return false;
}

bool GetButtonDPClick()
{
    return false;
}

uint16_t* GetFrameBufferFront()
{
    return NULL;
}

uint16_t* GetFrameBufferBack()
{
    return NULL;
}

uint8_t* GetFrameBuffer1byte()
{
    return NULL;
}

uint16_t* GetFrameBuffer2bytes()
{
    return NULL;
}

uint16_t GetBufferWidth()
{
    return 0;
}

uint16_t GetBufferHeight()
{
    return 0;
}

KeyState GetInputKeyState()
{
    KeyState k = {0};
    return k;
}


int main()
{
    InitGame();
    DEBUG("===");
    MapDescend(g_run.player.id);
    DEBUG("---");

    // RunCombatTests();
    CreatureTests();
    CheckAnimationImplemented();


    return 0;
};
