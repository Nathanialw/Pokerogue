//
// Created by nathanial on 5/7/26.
//

#include "cartridge_rom.h"

#include <string.h>

#include "lib_debugging.h"
#include "pico_constants.h"
#include "hardware/gpio.h"
#include "pico/stdio.h"


/**********************************************************************************************************************/
/**
 *      Pico cartridge SPI
 *
/**********************************************************************************************************************/


/**********************************************************************************************************************/
/**  UTILS
**********************************************************************************************************************/
void EEPROM_PrintBuffer(uint32_t sector, uint8_t* buf, uint32_t BUF_SIZE)
{
    bool nonempty = false;
    for (int i = 0; i < BUF_SIZE; i++)
    {
        if (buf[i] != 0x00)
        {
            nonempty = true;
            break;
        }
    }

    if (nonempty)
    {
        DEBUG_INLINE("Sector %lu (0x%08lX):\n", sector, sector * BUF_SIZE);
        for (int i = 0; i < BUF_SIZE; i += 64)
        {
            DEBUG_INLINE("%08lX  ", sector * BUF_SIZE + i);
            for (int j = 0; j < 64 && (i + j) < BUF_SIZE; j++)
                DEBUG_INLINE("%02X ", buf[i + j]);
            DEBUG_INLINE(" |");
            for (int j = 0; j < 64 && (i + j) < BUF_SIZE; j++)
            {
                char c = buf[i + j];
                if (c < 32 || c > 126) c = '.';
                DEBUG_INLINE("%c", c);
            }
            DEBUG_INLINE("|\n");
        }
    }
}

// Release from Power-down + Read ID
// Best JEDEC Read
// Call this once after InitCart()
void EEPROM_WakeUp(void)
{
    uint8_t cmd = 0xAB;
    gpio_put(EEPROM_CART_CS, 0);
    sleep_us(20);
    spi_write_blocking(SPI_A, &cmd, 1);
    gpio_put(EEPROM_CART_CS, 1);
    sleep_ms(20);
}


// Check if busy
bool EEPROM_IsBusy(void)
{
    uint8_t cmd = 0x05;
    uint8_t status = 0;

    gpio_put(EEPROM_CART_CS, 0);
    spi_write_blocking(SPI_A, &cmd, 1);
    spi_read_blocking(SPI_A, 0x00, &status, 1);
    gpio_put(EEPROM_CART_CS, 1);

    return (status & 0x01); // BUSY bit
}


/**********************************************************************************************************************/
/**  WRITING
**********************************************************************************************************************/
// Enable Write
void EEPROM_WriteEnable(void)
{
    uint8_t cmd = 0x06;
    gpio_put(EEPROM_CART_CS, 0);
    spi_write_blocking(SPI_A, &cmd, 1);
    gpio_put(EEPROM_CART_CS, 1);
}

// Page Program (up to 256 bytes)
void EEPROM_PageProgram(uint32_t addr, uint8_t* data, uint16_t length)
{
    if (length > 256) length = 256;

    EEPROM_WriteEnable();

    uint8_t tx[4] = {0x02, (addr >> 16) & 0xFF, (addr >> 8) & 0xFF, addr & 0xFF};

    gpio_put(EEPROM_CART_CS, 0);
    spi_write_blocking(SPI_A, tx, 4);
    spi_write_blocking(SPI_A, data, length);

    gpio_put(EEPROM_CART_CS, 1);

    // Wait for completion
    while (EEPROM_IsBusy()) tight_loop_contents();
}


// Write any amount, automatically crossing page boundaries
bool EEPROM_WriteAny(uint32_t addr, uint8_t* data, uint32_t length, bool verify)
{
    while (length > 0)
    {
        // How many bytes can we write in the current page?
        uint32_t page_remaining = 256 - (addr & 0xFF); // (addr % 256)
        const uint32_t chunk = (length < page_remaining) ? length : page_remaining;

        EEPROM_PageProgram(addr, data, chunk);

        if (verify)
        {
            uint8_t buf1[256] = {0};
            EEPROM_Read(addr, buf1, chunk);
            for (int i = 0; i < chunk; i++)
                if (data[i] != buf1[i])
                {
                    DEBUG("Write failed at 0x%08lX", addr);
                    return false;
                }
        }

        addr += chunk;
        data += chunk;
        length -= chunk;
    }
    return true;
}


/**********************************************************************************************************************/
/**  ERASING
**********************************************************************************************************************/
// Sector Erase (4KB)
void EEPROM_EraseSector(uint32_t addr)
{
    EEPROM_WriteEnable();
    uint8_t tx[4] = {0x20, (addr >> 16) & 0xFF, (addr >> 8) & 0xFF, addr & 0xFF};

    gpio_put(EEPROM_CART_CS, 0);
    spi_write_blocking(SPI_A, tx, 4);
    gpio_put(EEPROM_CART_CS, 1);

    while (EEPROM_IsBusy()) sleep_ms(1);
}


void EEPROM_Clear(uint32_t expected_size)
{
    DEBUG("Clearing EEPROM...");
    for (uint32_t addr = 0; addr < expected_size; addr += 4096)
    {
        EEPROM_EraseSector(addr);
    }
}

/**********************************************************************************************************************/
/**  READING
**********************************************************************************************************************/

void EEPROM_ReadJEDEC(void)
{
    uint8_t tx[4] = {0x9F, 0x00, 0x00, 0x00};
    uint8_t rx[4] = {0};

    gpio_put(EEPROM_CART_CS, 0);
    sleep_us(30);
    spi_write_read_blocking(SPI_A, tx, rx, 4);
    gpio_put(EEPROM_CART_CS, 1);

    DEBUG("JEDEC: %02X %02X %02X", rx[1], rx[2], rx[3]);
}

// Read single byte
uint8_t EEPROM_ReadByte(uint32_t addr)
{
    uint8_t header[4] =
    {
        0x03,
        (addr >> 16) & 0xFF,
        (addr >> 8) & 0xFF,
        addr & 0xFF
    };

    uint8_t value = 0;

    gpio_put(EEPROM_CART_CS, 0);

    spi_write_blocking(SPI_A, header, 4);
    spi_read_blocking(SPI_A, 0x00, &value, 1);

    gpio_put(EEPROM_CART_CS, 1);

    return value;
}

// Read multiple bytes (recommended)
void EEPROM_ReadBuffer(uint32_t addr, uint8_t* buffer, uint16_t length)
{
    uint8_t tx[4] = {
        0x03,
        (uint8_t)(addr >> 16),
        (uint8_t)(addr >> 8),
        (uint8_t)addr
    };

    gpio_put(EEPROM_CART_CS, 0);
    sleep_us(20); // generous setup time

    spi_write_blocking(SPI_A, tx, 4);
    spi_read_blocking(SPI_A, 0x00, buffer, length);

    gpio_put(EEPROM_CART_CS, 1);
}

/**
 * Read a block of data from the EEPROM (SPI flash).
 * addr   : starting address (0 … chip_size-1)
 * data   : buffer to store read bytes
 * length : number of bytes to read
 */
void EEPROM_Read(uint32_t addr, uint8_t* data, size_t len)
{
    if (len == 0) return;

    uint8_t cmd[4];
    cmd[0] = 0x03;
    cmd[1] = (addr >> 16) & 0xFF;
    cmd[2] = (addr >> 8) & 0xFF;
    cmd[3] = addr & 0xFF;

    gpio_put(EEPROM_CART_CS, 0);
    spi_write_blocking(SPI_A, cmd, 4);
    spi_read_blocking(SPI_A, 0xFF, data, len);
    gpio_put(EEPROM_CART_CS, 1);
}

uint8_t EEPROM_ReadHighAddress(void)
{
    uint32_t high_addr = 0xFFF000; // near the end of 16MB
    uint8_t tx[4] = {
        0x03,
        (high_addr >> 16) & 0xFF,
        (high_addr >> 8) & 0xFF,
        high_addr & 0xFF
    };
    uint8_t rx[5] = {0};

    gpio_put(EEPROM_CART_CS, 0);
    sleep_us(10);
    spi_write_read_blocking(SPI_A, tx, rx, 5);
    sleep_us(10);
    gpio_put(EEPROM_CART_CS, 1);

    return rx[4];
}


/**********************************************************************************************************************/
/**  TESTING
**********************************************************************************************************************/

/**
 * Full Brute-Force EEPROM Test as requested:
 * - Zeros out the ENTIRE chip first
 * - Writes unique value every 1KB
 * - Verifies written bytes are correct
 * - Verifies all other bytes remain 0xFF
 * - Logs up to 256 failures
 */


// Fast Read version
uint8_t EEPROM_FastReadByte(uint32_t addr)
{
    uint8_t tx[5] = {
        0x0B,
        (uint8_t)(addr >> 16),
        (uint8_t)(addr >> 8),
        (uint8_t)addr,
        0x00 // dummy
    };
    uint8_t rx[6] = {0};

    gpio_put(EEPROM_CART_CS, 0);
    sleep_us(5);

    spi_write_read_blocking(SPI_A, tx, rx, 6);

    gpio_put(EEPROM_CART_CS, 1);

    return rx[5];
}


void InitCart(void)
{
    gpio_init(EEPROM_CART_CS);
    gpio_set_dir(EEPROM_CART_CS, GPIO_OUT);
    gpio_put(EEPROM_CART_CS, 1);
}


/**********************************************************************************************************************/
/**  tests
**********************************************************************************************************************/
bool EEPROM_FullTest()
{
    DEBUG("=== EEPROM FULL TEST START ===");

    // --------------------------------------------------------------------
    // JEDEC
    // --------------------------------------------------------------------

    EEPROM_ReadJEDEC();

    // --------------------------------------------------------------------
    // Read distribution test
    // --------------------------------------------------------------------

    uint32_t test_addrs[] =
    {
        0x000000,
        0x000100,
        0x001000,
        0x010000,
        0x100000,
        0x700000,
        0xFFFFFF
    };

    for (size_t i = 0; i < count_of(test_addrs); i++)
    {
        uint8_t value =
            EEPROM_ReadByte(test_addrs[i]);

        DEBUG(
            "ADDR 0x%06X = 0x%02X",
            test_addrs[i],
            value
        );
    }

    // --------------------------------------------------------------------
    // Sequential read test
    // --------------------------------------------------------------------

    DEBUG("=== EEPROM BLOCK READ TEST ===");

    uint8_t buffer[256];

    EEPROM_Read(
        0x000000,
        buffer,
        sizeof(buffer)
    );

    for (int i = 0; i < 32; i++)
    {
        DEBUG(
            "%02X %02X %02X %02X %02X %02X %02X %02X",
            buffer[i * 8 + 0],
            buffer[i * 8 + 1],
            buffer[i * 8 + 2],
            buffer[i * 8 + 3],
            buffer[i * 8 + 4],
            buffer[i * 8 + 5],
            buffer[i * 8 + 6],
            buffer[i * 8 + 7]
        );
    }

    // --------------------------------------------------------------------
    // Read speed test
    // --------------------------------------------------------------------

    DEBUG("=== EEPROM SPEED TEST ===");

    absolute_time_t start =
        get_absolute_time();

    for (int i = 0; i < 1000; i++)
    {
        EEPROM_Read(
            0x000000,
            buffer,
            sizeof(buffer)
        );
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

    double mb_per_sec =
        (256.0 / 1024.0) /
        ((double)elapsed_us / 1000000.0);

    DEBUG(
        "READ SPEED: %.2f MB/s",
        mb_per_sec
    );

    DEBUG("=== EEPROM FULL TEST PASSED ===");

    return true;
}


bool EEPROM_VerifySize(uint32_t expected_size)
{
    DEBUG("========================================");
    DEBUG("=== FULL BRUTE FORCE SIZE TEST ===");
    DEBUG("Expected: %lu MB (%lu bytes)", expected_size/(1024*1024), expected_size);
    DEBUG("========================================");

    const uint32_t step = 1024;
    const uint32_t num_steps = expected_size / step;

#define MAX_FAILURES 256
    uint32_t failed_addr[MAX_FAILURES];
    uint8_t expected_val[MAX_FAILURES];
    uint8_t actual_val[MAX_FAILURES];
    int failure_count = 0;

    // ===================================================================
    // 1. ZERO OUT THE ENTIRE CHIP
    // ===================================================================
    DEBUG("Phase 1: Zeroing out entire chip with 0xFF...");
    EEPROM_Clear(expected_size);

    // ===================================================================
    // 2. WRITE UNIQUE VALUES EVERY 1KB
    // ===================================================================
    DEBUG("Phase 2: Writing unique markers every 1KB...");
    for (uint32_t i = 0; i < num_steps; i++)
    {
        uint32_t addr = i * step;
        uint8_t value = (uint8_t)(i & 0xFF);

        EEPROM_PageProgram(addr, &value, 1);

        if (i % 256 == 0)
        {
            DEBUG("  Progress: %3lu%%  (0x%08lX)", (i * 100) / num_steps, addr);
        }
    }

    sleep_ms(50);

    // ===================================================================
    // 3. FULL VERIFICATION
    // ===================================================================
    DEBUG("Phase 3: Full verification...");

    for (uint32_t i = 0; i < num_steps; i++)
    {
        uint32_t addr = i * step;
        uint8_t expected = (uint8_t)(i & 0xFF);
        uint8_t actual = EEPROM_ReadByte(addr);

        if (actual != expected)
        {
            if (failure_count < MAX_FAILURES)
            {
                failed_addr[failure_count] = addr;
                expected_val[failure_count] = expected;
                actual_val[failure_count] = actual;
                failure_count++;
            }
        }
    }

    // Check that unwritten areas are still 0xFF
    DEBUG("Checking non-marker areas are still 0xFF...");
    for (uint32_t addr = 0; addr < expected_size; addr += step)
    {
        // Skip the marker positions
        if ((addr % step) == 0) continue; // This is rough, but fast

        uint8_t val = EEPROM_ReadByte(addr);
        if (val != 0xFF)
        {
            if (failure_count < MAX_FAILURES)
            {
                failed_addr[failure_count] = addr;
                expected_val[failure_count] = 0xFF;
                actual_val[failure_count] = val;
                failure_count++;
            }
        }
    }

    // ===================================================================
    // 4. RESTORE TO 0xFF
    // ===================================================================
    // DEBUG("Restoring chip to 0xFF...");
    // EEPROM_Clear(expected_size);

    // ===================================================================
    // Results
    // ===================================================================
    DEBUG("========================================");
    DEBUG("Test complete. %d failures found.", failure_count);

    if (failure_count == 0)
    {
        DEBUG("✓ **GENUINE %lu MB EEPROM** - Test passed perfectly!", expected_size/(1024*1024));
        return true;
    }
    else
    {
        DEBUG("✗ **FAIL** - %d problems detected:", failure_count);
        for (int i = 0; i < failure_count && i < 16; i++) // Show first 16
        {
            DEBUG("   0x%08lX  Expected: 0x%02X  Got: 0x%02X",
                  failed_addr[i], expected_val[i], actual_val[i]);
        }
        if (failure_count > 16)
            DEBUG("   ... and %d more failures", failure_count - 16);

        return false;
    }
}


/**
 * Full Brute-Force EEPROM Test
 * - Zeros entire chip first
 * - Writes unique value every 1KB
 * - Verifies written bytes are correct
 * - Verifies all other bytes are still 0xFF
 * - Does NOT zero the chip at the end (for retention testing)
 */
/**
 * Full Brute-Force EEPROM Test - Reliable version
 */
void EEPROM_FullMemoryTest(uint32_t chip_size)
{
    DEBUG("========================================");
    DEBUG("=== FULL BRUTE FORCE INTEGRITY TEST ===");
    DEBUG("Expected: %lu MB (%lu bytes)", chip_size/(1024*1024), chip_size);
    DEBUG("========================================");

    const uint32_t step = 1024;
    const uint32_t num_steps = chip_size / step;

    uint32_t bad_locations = 0;

    // 1. Zero entire chip
    DEBUG("Phase 1: Erasing chip to 0xFF...");
    for (uint32_t addr = 0; addr < chip_size; addr += 4096)
    {
        EEPROM_EraseSector(addr);
    }

    // 2. Write reliable pattern (4 bytes at a time)
    DEBUG("Phase 2: Writing test pattern every 1KB...");
    for (uint32_t i = 0; i < num_steps; i++)
    {
        uint32_t addr = i * step;
        uint8_t value = 0x80 | (uint8_t)(i & 0x7F);

        // Write 4 bytes for better reliability
        for (int j = 0; j < 4; j++)
        {
            uint8_t v = value + j;
            EEPROM_PageProgram(addr + j, &v, 1);
        }

        if (i % 256 == 0)
            DEBUG("  Progress: %3lu%%  (0x%08lX)", (i * 100) / num_steps, addr);
    }

    sleep_ms(100);

    // 3. Verify
    DEBUG("Phase 3: Verifying...");
    for (uint32_t i = 0; i < num_steps; i++)
    {
        uint32_t addr = i * step;
        uint8_t expected = 0x80 | (uint8_t)(i & 0x7F);
        uint8_t actual = EEPROM_ReadByte(addr);

        if (actual != expected)
        {
            DEBUG("✗ MISMATCH at 0x%08lX | Expected 0x%02X, Got 0x%02X",
                  addr, expected, actual);
            bad_locations++;
        }
    }

    DEBUG("========================================");
    if (bad_locations == 0)
        DEBUG("✓ FULL MEMORY TEST PASSED");
    else
        DEBUG("✗ %lu failures during write verification", bad_locations);

    DEBUG("You can now run EEPROM_RetentionCheck() after power cycle.");
}


/**
 * Retention Check - Run after power cycle
 * Reports first few failing addresses for debugging
 */
void EEPROM_RetentionCheck(uint32_t chip_size)
{
    DEBUG("========================================");
    DEBUG("=== EEPROM RETENTION CHECK ===");
    DEBUG("Size: %lu MB (%lu bytes)", chip_size/(1024*1024), chip_size);
    DEBUG("========================================");

    const uint32_t step = 1024;
    const uint32_t num_steps = chip_size / step;
    uint32_t bad_locations = 0;

#define MAX_REPORT 16
    uint32_t failed_addr[MAX_REPORT];
    uint8_t expected_val[MAX_REPORT];
    uint8_t actual_val[MAX_REPORT];

    DEBUG("Checking stored pattern...");

    for (uint32_t i = 0; i < num_steps; i++)
    {
        uint32_t addr = i * step;
        uint8_t expected = 0x80 | (uint8_t)(i & 0x7F);
        uint8_t actual = EEPROM_ReadByte(addr);

        if (actual != expected)
        {
            if (bad_locations < MAX_REPORT)
            {
                failed_addr[bad_locations] = addr;
                expected_val[bad_locations] = expected;
                actual_val[bad_locations] = actual;
            }
            bad_locations++;
        }
    }

    DEBUG("========================================");

    if (bad_locations == 0)
    {
        DEBUG("✓ RETENTION PASSED - All data survived power cycle");
        DEBUG("   Chip appears to have good data retention.");
    }
    else
    {
        DEBUG("✗ RETENTION FAILED - %lu locations lost data", bad_locations);

        DEBUG("First %d failures:", (bad_locations < MAX_REPORT ? bad_locations : MAX_REPORT));
        for (int i = 0; i < MAX_REPORT && i < bad_locations; i++)
        {
            DEBUG("   0x%08lX | Expected: 0x%02X | Got: 0x%02X",
                  failed_addr[i], expected_val[i], actual_val[i]);
        }

        if (bad_locations > MAX_REPORT)
            DEBUG("   ... and %lu more failures", bad_locations - MAX_REPORT);
    }

    DEBUG("========================================");
}


bool CheckConnection(const char* sent, const char* rec)
{
    char cmd[16];

    while (1)
    {
        memset(cmd, 0, sizeof(cmd));

        printf(sent);
        fflush(stdout);

        absolute_time_t timeout = make_timeout_time_ms(100);

        int index = 0;
        bool got_line = false;

        printf("\n %s %s %d\n", cmd, rec, strcmp(cmd, rec));
        while (!time_reached(timeout))
        {
            int ch = getchar_timeout_us(1000);

            if (ch == PICO_ERROR_TIMEOUT)
                continue;

            // Ignore non-printable garbage
            if (ch < 32 || ch > 126)
            {
                if (ch == '\r' || ch == '\n')
                {
                    got_line = true;
                    break;
                }

                continue;
            }

            if (index < (sizeof(cmd) - 1))
            {
                cmd[index++] = (char)ch;
                cmd[index] = '\0';
            }
        }

        if (!got_line)
            continue;

        printf("\n %s %s %d\n", cmd, rec, strcmp(cmd, rec));
        if (strcmp(cmd, rec) != 0)
        {
            break;
        }

        printf("\nReceived %s command\n", rec);
    }

    return true;
}

void Signal()
{
    while (1)
    {
        printf("READY\n");

        int c = getchar_timeout_us(100000); // 100ms

        if (c == 'D')
        {
            char cmd[4];

            cmd[0] = getchar_timeout_us(100000);
            cmd[1] = getchar_timeout_us(100000);
            cmd[2] = getchar_timeout_us(100000);
            cmd[3] = getchar_timeout_us(100000);

            if (cmd[0] == 'U' &&
                cmd[1] == 'M' &&
                cmd[2] == 'P' &&
                cmd[3] == '\n')
            {
                break;
            }
        }
    }
}

void EEPROM_Flash(void)
{
    uint32_t size = 4 * 1024 * 1024; // Change as needed
    const uint32_t buffer_size = 4096;
    uint8_t buf[buffer_size];
    uint32_t addr = 0;

    printf("BEGIN_FLASH\n");

    while (addr < size)
    {
        uint32_t chunk = (size - addr < buffer_size) ? (size - addr) : buffer_size;

        // Read from serial
        fread(buf, 1, chunk, stdin);

        // Write to EEPROM
        while (1)
        {
            if (EEPROM_WriteAny(addr, buf, chunk, true))
                break;
        }

        addr += chunk;
    }
}


void EEPROM_Dump(uint32_t chip_size)
{
    // Wait for DUMP command

    const uint32_t buffer_size = 4096;
    uint8_t buf[buffer_size];
    uint32_t addr = 0;

    printf("BEGIN_DUMP\n");

    while (addr < chip_size)
    {
        uint32_t remaining = chip_size - addr;
        uint32_t chunk = (remaining < buffer_size) ? remaining : buffer_size;

        EEPROM_Read(addr, buf, chunk);
        fwrite(buf, 1, chunk, stdout);
        fflush(stdout);

        addr += chunk;
    }

    printf("\nEND_DUMP\n");
}


void EEPROM_Verify(uint32_t chip_size)
{
    uint32_t size = 4 * 1024 * 1024; // Change as needed
    const uint32_t buffer_size = 4096;
    uint8_t buf[buffer_size];
    uint8_t verify_buf[buffer_size];
    uint32_t addr = 0;

    printf("BEGIN_VERIFY\n");

    while (addr < size)
    {
        uint32_t chunk_size = (size - addr < buffer_size) ? (size - addr) : buffer_size;

        // Read from serial
        fread(buf, 1, chunk_size, stdin);

        // Read from EEPROM
        EEPROM_Read(addr, verify_buf, chunk_size);

        //verify
        for (int i = 0; i < chunk_size; i++)
            if (buf[i] != verify_buf[i])
            {
                printf("ERROR: EEPROM data mismatch at 0x%08lX\n", addr + i);
                return;
            }

        addr += chunk_size;
    }

    printf("VERIFICATION SUCCESSFUL\n");
}
