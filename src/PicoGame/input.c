//
// Created by nathanial on 2/19/26.
//
#include <stdlib.h>
#include "stdbool.h"

#include "pico/bootrom.h"
#include "hardware/adc.h"

#include "input.h"
#include "lib_types.h"
#include "camera.h"

#include "game_state.h"
#include "lib_debugging.h"
#include "pico_constants.h"
#include "pico_ram.h"

/**********************************************************************************************************************/
/*
**********************************************************************************************************************/
KeyState GetInputKeyState()
{
    return g_pico_ram.input.keyState;
}


#define DEADZONE 600

/**********************************************************************************************************************/
/**  Debounce check
**********************************************************************************************************************/
bool Pico_ButtonDownStable(uint8_t btn)
{
    if (gpio_get(btn) != 0) return false;
    sleep_ms(15);
    return gpio_get(btn) == 0;
}

/**********************************************************************************************************************/
/**  Triggers reset pin on the Pico
**********************************************************************************************************************/
bool Pico_Reset(void)
{
    reset_usb_boot(0, 0);
    return true;
}

/**********************************************************************************************************************/
/**  Checks for an action button pressed
**********************************************************************************************************************/
bool Pico_CheckBtn(uint16_t btn, uint16_t pressDuration)
{
    if (Pico_ButtonDownStable(btn))
    {
        // pressed
        sleep_ms(pressDuration);
        if (Pico_ButtonDownStable(btn)) // still pressed
        {
            return true;
        }
    }
    return false;
}


/****************************************************************************************************************/
/**  checks for analog stick input
 *  ON SUCCESS - return true
 *  ON FAIL - return false
**********************************************************************************************************************/
Delta Pico_InputDelta(void)
{
    Delta d = {0, 0};
    int8_t joy_dx = 0;
    int8_t joy_dy = 0;

    adc_select_input(0);
    sleep_us(5);
    uint16_t jy = adc_read();
    if (jy < 2048 - DEADZONE) joy_dy = -1;
    else if (jy > 2048 + DEADZONE) joy_dy = 1;

    adc_select_input(1);
    sleep_us(5);
    uint16_t jx = adc_read();
    if (jx < 2048 - DEADZONE) joy_dx = 1;
    else if (jx > 2048 + DEADZONE) joy_dx = -1;

    if (abs(joy_dx) > abs(joy_dy))
    {
        d.x = joy_dx;
        d.y = 0;
    }
    else
    {
        d.x = 0;
        d.y = joy_dy;
    }

    // if (d.x == 0 && d.y == 0) return false;


    return d;
}

/**********************************************************************************************************************/
/**  Reads input from the D pad
 *  ON SUCCESS - return true
 *  ON FAIL - return false
**********************************************************************************************************************/
Delta Pico_InputDeltaDPad(void)
{
    Delta d = {0, 0};
    int8_t dx = 0;
    int8_t dy = 0;

    // if (Pico_CheckBtn(UPBTN, 0)) dy = -1;
    // else if (Pico_CheckBtn(DOWNBTN, 0)) dy = 1;
    // else if (Pico_CheckBtn(LEFTBTN, 0)) dx = -1;
    // else if (Pico_CheckBtn(RIGHTBTN, 0)) dx = 1;
    // if (dx == 0 && dy == 0) return false;
    d.x = dx;
    d.y = dy;

    return d;
}


uint8_t SN74HC165N_ReadShiftRegister()
{
    uint8_t data = 0;
    gpio_put(SN74HC165N_SH_LD, 0);
    busy_wait_us(1);
    gpio_put(SN74HC165N_SH_LD, 1);
    busy_wait_us(10); // <-- extra delay for first bt to stabilise
    spi_read_blocking(SPI_BUTTONS, 0, &data, 1);
    return data;
}


uint16_t SN74HC165N_BitBangShiftRegister()
{
    gpio_put(SN74HC165N_SH_LD, 0);
    busy_wait_us(1);
    gpio_put(SN74HC165N_SH_LD, 1);
    busy_wait_us(1);

    uint16_t data = 0;
    for (int i = 0; i < 16; i++)
    {
        data = (data << 1) | gpio_get(SN74HC165N_MISO);
        gpio_put(SN74HC165N_SCLK, 1);
        busy_wait_us(1);
        gpio_put(SN74HC165N_SCLK, 0);
        busy_wait_us(1);
    }
    return data;
}

void PrintInput(uint16_t n, char* binary)
{
    for (int i = 16 - 1; i >= 0; i--)
    {
        binary[i] = (n & 1) ? '1' : '0';
        n >>= 1;
    }
    binary[16] = '\0';
}

bool GetButtonA(void)
{
    return g_pico_ram.input.keyState.buttons >> 0 & 1 ? 1 : 0;
}

bool GetButtonB(void)
{
    return g_pico_ram.input.keyState.buttons >> 1 & 1 ? 1 : 0;
}

bool GetButtonY(void)
{
    return g_pico_ram.input.keyState.buttons >> 2 & 1 ? 1 : 0;
}

bool GetButtonX(void)
{
    return g_pico_ram.input.keyState.buttons >> 3 & 1 ? 1 : 0;
}

bool GetButtonSelect(void)
{
    return g_pico_ram.input.keyState.buttons >> 4 & 1 ? 1 : 0;
}

bool GetButtonStart(void)
{
    return g_pico_ram.input.keyState.buttons >> 5 & 1 ? 1 : 0;
}

bool GetButtonUp(void)
{
    return g_pico_ram.input.keyState.buttons >> 6 & 1 ? 1 : 0;
}

bool GetButtonDown(void)
{
    return g_pico_ram.input.keyState.buttons >> 7 & 1 ? 1 : 0;
}

bool GetButtonLeft(void)
{
    return g_pico_ram.input.keyState.buttons >> 8 & 1 ? 1 : 0;
}

bool GetButtonRight(void)
{
    return g_pico_ram.input.keyState.buttons >> 9 & 1 ? 1 : 0;
}

bool GetButtonJSClick(void)
{
    return g_pico_ram.input.keyState.buttons >> 10 & 1 ? 1 : 0;
}

bool GetButtonDPClick(void)
{
    return g_pico_ram.input.keyState.buttons >> 11 & 1 ? 1 : 0;
}

/**********************************************************************************************************************/
/**  Loop polls for input
 *  if input found update button state
 *  pass to main state control switch
**********************************************************************************************************************/
State HandleInput(State state)
{
    while (1)
    {
        const uint16_t tmp = SN74HC165N_BitBangShiftRegister();
        if (tmp == g_pico_ram.input.keyState.buttons)
        {
            DEBUG("Checking for input");
            sleep_ms(g_pico_ram.input.sleepTime); //sleep between key queries
            continue;
        }

        g_pico_ram.input.keyState.buttons = tmp;
        char binary[16 + 1];
        PrintInput(g_pico_ram.input.keyState.buttons, binary);
        DEBUG("Buttons: 0b%s", binary);
        DEBUG("button A : %d", GetButtonA());

        g_pico_ram.input.sleepTime = g_pico_ram.input.defaultSleepTime;
        g_pico_ram.input.keyState.d.x = 0;
        g_pico_ram.input.keyState.d.y = 0;
        break;
    }

    State prevState = state;
    DEBUG("UpdateGameState");
    state = UpdateGameState(state);
    DEBUG("UpdateGameState done");


    if (state.inputState == REBOOT)
    {
        // if (Pico_CheckBtn(REDBTN, 1000))
        //     Pico_Reset();
        // else
        //     state.inputState = MOVING;
    }

    DEBUG("FullRedraw");

    // if ((prevState.inputState == MENU || prevState.inputState == BATTLE) && state.inputState == MOVING)
    //     FullRedraw();

    if (state.inputState == MENU)
        g_pico_ram.input.sleepTime = g_pico_ram.input.menuSleepTime;

    DEBUG("input Done");
    return state;
}
