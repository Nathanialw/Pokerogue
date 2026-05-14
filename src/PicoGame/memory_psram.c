//
// Created by nathanial on 5/8/26.
//

#include "memory_psram.h"

#include "lib_debugging.h"
#include "pico_constants.h"
#include "hardware/gpio.h"

void InitPSram()
{
    gpio_init(ESP_PSRAM64_CS);
    gpio_set_dir(ESP_PSRAM64_CS, GPIO_OUT);
    gpio_put(ESP_PSRAM64_CS, 1);
}

void PSRAM_Reset()
{
    uint8_t cmd;
    gpio_put(ESP_PSRAM64_CS, 0);

    cmd = 0x66;
    spi_write_blocking(SPI_A, &cmd, 1);
    gpio_put(ESP_PSRAM64_CS, 1);
    sleep_us(10);
    gpio_put(ESP_PSRAM64_CS, 0);

    cmd = 0x99;
    spi_write_blocking(SPI_A, &cmd, 1);
    gpio_put(ESP_PSRAM64_CS, 1);
    sleep_ms(1);
}

void PSRAM_ReadID()
{
    uint8_t tx[4] = {0x9F, 0, 0, 0};
    uint8_t rx[4] = {0};

    gpio_put(ESP_PSRAM64_CS, 0);
    spi_write_read_blocking(SPI_A, tx, rx, 4);
    gpio_put(ESP_PSRAM64_CS, 1);

    DEBUG("PSRAM ID: %02X %02X %02X",
          rx[1], rx[2], rx[3]);
}

void PSRAM_WriteByte(uint32_t addr, uint8_t value)
{
    uint8_t tx[5];

    tx[0] = 0x02;

    tx[1] = (addr >> 16) & 0xFF;
    tx[2] = (addr >> 8) & 0xFF;
    tx[3] = addr & 0xFF;

    tx[4] = value;

    gpio_put(ESP_PSRAM64_CS, 0);
    spi_write_blocking(SPI_A, tx, 5);
    gpio_put(ESP_PSRAM64_CS, 1);
}

uint8_t PSRAM_ReadByte(uint32_t addr)
{
    uint8_t tx[5];
    uint8_t rx[5];

    tx[0] = 0x03;

    tx[1] = (addr >> 16) & 0xFF;
    tx[2] = (addr >> 8) & 0xFF;
    tx[3] = addr & 0xFF;
    tx[4] = 0x00;

    gpio_put(ESP_PSRAM64_CS, 0);
    spi_write_read_blocking(SPI_A, tx, rx, 5);
    gpio_put(ESP_PSRAM64_CS, 1);

    return rx[4];
}

void PSRAM_Read(
    uint32_t addr,
    uint8_t *data,
    size_t len
)
{
    uint8_t header[4];

    header[0] = 0x03;
    header[1] = (addr >> 16) & 0xFF;
    header[2] = (addr >> 8) & 0xFF;
    header[3] = addr & 0xFF;

    gpio_put(ESP_PSRAM64_CS, 0);
    spi_write_blocking(SPI_A, header, 4);
    spi_read_blocking(SPI_A, 0x00, data, len);
    gpio_put(ESP_PSRAM64_CS, 1);
}

void PSRAM_Write(
    uint32_t addr,
    const uint8_t *data,
    size_t len
)
{
    uint8_t header[4];

    header[0] = 0x02;
    header[1] = (addr >> 16) & 0xFF;
    header[2] = (addr >> 8) & 0xFF;
    header[3] = addr & 0xFF;

    gpio_put(ESP_PSRAM64_CS, 0);

    spi_write_blocking(SPI_A, header, 4);
    spi_write_blocking(SPI_A, data, len);

    gpio_put(ESP_PSRAM64_CS, 1);
}

/**********************************************************************************************************************/
/**  tests
**********************************************************************************************************************/
void PSRAM_Test()
{
    uint8_t value;

    PSRAM_Reset();

    PSRAM_ReadID();

    PSRAM_WriteByte(0x000000, 0x42);

    value = PSRAM_ReadByte(0x000000);

    DEBUG("PSRAM READBACK: %02X", value);
}

bool PSRAM_FullTest()
{
    typedef struct
    {
        uint32_t addr;
        uint8_t  value;
    }
    psram_test_t;

    // Spread across full 8MB address space
    static const psram_test_t tests[] =
    {
        {0x000000, 0x11},
        {0x000001, 0x22},
        {0x000100, 0x33},
        {0x001000, 0x44},
        {0x010000, 0x55},
        {0x100000, 0x66},
        {0x200000, 0x77},
        {0x400000, 0x88},
        {0x7FFFFF, 0x99},
    };

    uint8_t readback;

    DEBUG("=== PSRAM FULL TEST START ===");

    // --------------------------------------------------------------------
    // Write test pattern
    // --------------------------------------------------------------------

    for (size_t i = 0; i < count_of(tests); i++)
    {
        PSRAM_WriteByte(
            tests[i].addr,
            tests[i].value
        );
    }

    // --------------------------------------------------------------------
    // Verify pattern
    // --------------------------------------------------------------------

    for (size_t i = 0; i < count_of(tests); i++)
    {
        readback = PSRAM_ReadByte(tests[i].addr);

        DEBUG(
            "ADDR 0x%06X : WROTE 0x%02X READ 0x%02X",
            tests[i].addr,
            tests[i].value,
            readback
        );

        if (readback != tests[i].value)
        {
            DEBUG(
                "FAIL at 0x%06X",
                tests[i].addr
            );

            return false;
        }
    }

    // --------------------------------------------------------------------
    // Aliasing test
    // --------------------------------------------------------------------
    //
    // Detects fake/smaller PSRAM mirrored into
    // larger address space.
    //
    // Write different values to far-apart addresses
    // and ensure they stay independent.
    //
    // --------------------------------------------------------------------

    DEBUG("=== ALIAS TEST ===");

    PSRAM_WriteByte(0x000000, 0xAA);
    PSRAM_WriteByte(0x400000, 0x55);

    uint8_t low  = PSRAM_ReadByte(0x000000);
    uint8_t high = PSRAM_ReadByte(0x400000);

    DEBUG(
        "LOW  = 0x%02X HIGH = 0x%02X",
        low,
        high
    );

    if (low != 0xAA || high != 0x55)
    {
        DEBUG("ALIAS TEST FAILED");
        return false;
    }

    // --------------------------------------------------------------------
    // Sequential block test
    // --------------------------------------------------------------------

    DEBUG("=== BLOCK TEST ===");

    uint8_t tx[256];
    uint8_t rx[256];

    for (int i = 0; i < 256; i++)
    {
        tx[i] = i;
        rx[i] = 0;
    }

    PSRAM_Write(0x002000, tx, sizeof(tx));
    PSRAM_Read(0x002000, rx, sizeof(rx));

    for (int i = 0; i < 256; i++)
    {
        if (rx[i] != tx[i])
        {
            DEBUG(
                "BLOCK FAIL [%d] TX=0x%02X RX=0x%02X",
                i,
                tx[i],
                rx[i]
            );

            return false;
        }
    }

    DEBUG("BLOCK TEST PASSED");

    // --------------------------------------------------------------------
    // Speed sanity test
    // --------------------------------------------------------------------

    DEBUG("=== SPEED TEST ===");

    absolute_time_t start = get_absolute_time();

    for (int i = 0; i < 1000; i++)
    {
        PSRAM_Read(0x002000, rx, sizeof(rx));
    }

    int64_t elapsed_us =
        absolute_time_diff_us(
            start,
            get_absolute_time()
        );

    DEBUG(
        "256KB READ TIME: %lld us",
        elapsed_us
    );

    double read_mb_per_sec =
    (256.0 / 1024.0) / ((double)elapsed_us / 1000000.0);

    DEBUG(
        "READ SPEED: %.2f MB/s",
        read_mb_per_sec
    );

    // --------------------------------------------------------------------
    // Write speed test
    // --------------------------------------------------------------------

    DEBUG("=== WRITE SPEED TEST ===");

    start = get_absolute_time();

    for (int i = 0; i < 1000; i++)
    {
        PSRAM_Write(0x002000, tx, sizeof(tx));
    }

    elapsed_us =
        absolute_time_diff_us(
            start,
            get_absolute_time()
        );

    DEBUG(
        "256KB WRITE TIME: %lld us",
        elapsed_us
    );

    double write_mb_per_sec =
        (256.0 / 1024.0) / ((double)elapsed_us / 1000000.0);

    DEBUG(
        "WRITE SPEED: %.2f MB/s",
        write_mb_per_sec
    );

    DEBUG("=== PSRAM FULL TEST PASSED ===");

    return true;
}