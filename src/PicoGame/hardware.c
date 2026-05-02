//
// Created by nathanial on 3/26/26.
//

#include "hardware.h"
#include "pico/stdlib.h"
#include "hardware/adc.h"
#include "hardware/i2c.h"
#include "hardware/pwm.h"

// #include "memory_ram.h"
#include "pico_constants.h"

/**********************************************************************************************************************/
/*
 *  BATTERY
 *
**********************************************************************************************************************/

#define MAX_COUNTER 10
#define DIVIDER_RATIO 2.0f   // 10k ohm + 10k ohm divider
#define BATTERY_STATUS

uint8_t counter = MAX_COUNTER;
uint8_t volts = 0;

/**********************************************************************************************************************/
/**  Reads the pin input on the pico
**********************************************************************************************************************/
uint16_t Pico_ReadBatteryADC(void)
{
    adc_select_input(2); // battery
    sleep_us(5); // small settle time
    return adc_read();
}

/**********************************************************************************************************************/
/**  Get the pin input for the battery status
 *  convert from int to char array
 *  save into cache
 *  if volts < threshold -> turn on board LED
**********************************************************************************************************************/
void Pico_BatteryStatus(void)
{
    // DEBUG("BatteryStatus. %d", counter);
    //
    // //commented out for debugging
    // // counter++;
    // // if (counter >= MAX_COUNTER)
    // // {
    //
    // uint16_t raw = Pico_ReadBatteryADC();
    // float voltage = raw * 3.3f / 4095.0f;
    // float battery_voltage = voltage * DIVIDER_RATIO * 100.0f;
    //
    // GetAsChars_uint16((uint16_t)battery_voltage, &g_run.hardware.volts);
    // g_run.hardware.volts[0] = g_run.hardware.volts[2];
    // g_run.hardware.volts[1] = '.';
    // g_run.hardware.volts[2] = g_run.hardware.volts[3];
    // g_run.hardware.volts[3] = g_run.hardware.volts[4];
    // g_run.hardware.volts[4] = 'v';
    //
    //
    // if (voltage * DIVIDER_RATIO < 3.3f)
    // {
    //     // LOW BATTERY ACTIONS
    //     // Example:
    //     // reduce volume
    //     // set a global flag
    //     // trigger UI warning
    //
    //     gpio_put(LED, 1); // simple indicator
    // }
    // else
    // {
    //     gpio_put(LED, 0);
    // }
    // }
}

/**********************************************************************************************************************/
/*
*   ADS1115 control
*
**********************************************************************************************************************/

#define ADS_ADDR 0x48
#define ADS1115_VOLUME_0    0b100
#define ADS1115_DIMMER_1    0b101
// #define ADS1115_UNUSED_2    0b110
// #define ADS1115_UNUSED_3    0b111

bool Pico_ADS1115_Init(void)
{
    i2c_init(I2C_PORT, 100 * 1000);
    gpio_set_function(ADS_SDA, GPIO_FUNC_I2C);
    gpio_set_function(ADS_SCL, GPIO_FUNC_I2C);
    gpio_pull_up(ADS_SDA);
    gpio_pull_up(ADS_SCL);
    return true;
}


static int16_t Pico_ReadADS1115(uint8_t mux_bits)
{
    uint8_t config[3];
    uint8_t reg = 0x00;
    uint8_t data[2];
    int rc;

    config[0] = 0x01; // config register
    config[1] =
        (1 << 7) | // OS = start single conversion
        ((mux_bits & 0x07) << 4) |
        (0b001 << 1); // PGA = ±4.096V

    config[2] =
        (1 << 7) | // MODE = single-shot
        (0b111 << 5) | // 860 SPS
        (0b11); // comparator disabled

    rc = i2c_write_timeout_us(I2C_PORT, ADS_ADDR, config, 3, false, 5000);
    sleep_ms(2); // enough for 860 SPS
    if (rc < 0)
    {
        // DEBUG("Pico_ReadADS1115 1");
        return -1;
    }

    rc = i2c_write_timeout_us(I2C_PORT, ADS_ADDR, &reg, 1, true, 5000);
    if (rc < 0)
    {
        // DEBUG("Pico_ReadADS1115 2");
        return -1;
    }

    rc = i2c_read_timeout_us(I2C_PORT, ADS_ADDR, data, 2, false, 5000);
    if (rc < 0)
    {
        // DEBUG("Pico_ReadADS1115 3");
        return -1;
    }

    return (int16_t)((data[0] << 8) | data[1]);
}

void I2C_BusClear(void)
{
    gpio_set_function(ADS_SDA, GPIO_FUNC_SIO);
    gpio_set_function(ADS_SCL, GPIO_FUNC_SIO);

    gpio_set_dir(ADS_SDA, GPIO_IN);
    gpio_pull_up(ADS_SDA);

    gpio_set_dir(ADS_SCL, GPIO_OUT);
    gpio_put(ADS_SCL, 1);
    sleep_us(5);

    for (int i = 0; i < 9; i++)
    {
        gpio_put(ADS_SCL, 0);
        sleep_us(5);
        gpio_put(ADS_SCL, 1);
        sleep_us(5);
    }

    // force STOP
    gpio_set_dir(ADS_SDA, GPIO_OUT);
    gpio_put(ADS_SDA, 0);
    sleep_us(5);
    gpio_put(ADS_SCL, 1);
    sleep_us(5);
    gpio_put(ADS_SDA, 1);
    sleep_us(5);
}

void RecoverADS1115(void)
{
    // DEBUG("Recovering ADS");
    i2c_deinit(I2C_PORT);
    I2C_BusClear();
    sleep_ms(1);
    Pico_ADS1115_Init();
}


/**********************************************************************************************************************/
/*
*   Volume control
*
**********************************************************************************************************************/
int16_t Pico_VolumeControl(void)
{
    return Pico_ReadADS1115(ADS1115_VOLUME_0); // AIN0 vs GND
}


/**********************************************************************************************************************/
/*
*   Backlight control
*
**********************************************************************************************************************/


int16_t Pico_DimmerControl(void)
{
    return Pico_ReadADS1115(ADS1115_DIMMER_1); // AIN1 vs GND
}

void Backlight_Init(void)
{
    gpio_set_function(BL_PWM, GPIO_FUNC_PWM);

    uint slice = pwm_gpio_to_slice_num(BL_PWM);

    pwm_set_wrap(slice, 255); // 8-bit should be ok
    pwm_set_enabled(slice, true);
}

void UpdateBacklight(void)
{
    int16_t raw = Pico_DimmerControl();
    if (raw < 0) return;

    uint8_t duty = raw >> 8;

    uint slice = pwm_gpio_to_slice_num(BL_PWM);
    pwm_set_chan_level(slice, pwm_gpio_to_channel(BL_PWM), duty);
}


/**********************************************************************************************************************/
/*
*   SD CARD
*
**********************************************************************************************************************/

// TODO: still waiting for SD card reader module to come in the mail
