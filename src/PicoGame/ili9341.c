#include "ili9341.h"

#include "pico/stdlib.h"
#include "hardware/spi.h"

#include "lib_debugging.h"
#include "lib_types.h"
#include "pico_constants.h"
#include "memory_ram.h"


uint16_t* GetFrameBufferFront(void);
uint16_t* GetFrameBufferBack(void);
uint8_t* GetFrameBuffer1byte(void);
uint16_t* GetFrameBuffer2bytes(void);
uint16_t GetBufferWidth(void);
uint16_t GetBufferHeight(void);

void ClearBuffer(void);
void DrawBuffer(const FrameBuffer f);
void SetFrameBuffer(uint16_t rgb565);
void SetFrameBufferColor(Color rgb565);
void DrawSprite(FrameBuffer f, const uint8_t* sprite);
void DrawToBuffer(const FrameBuffer frameBuffer, const uint16_t* pixels, const Rect_16 rect);
void FillRect(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t rgb565);
void FillRectColor(uint16_t x, uint16_t y, uint16_t w, uint16_t h, Color rgb565);
void FillScreen(uint16_t rgb565);
void FillScreenColor(Color rgb565);
void DrawTileKeyed(uint16_t x, uint16_t y, uint16_t w, uint16_t h, const uint16_t* data);
void Draw(uint16_t x, uint16_t y, uint16_t w, uint16_t h, const uint8_t* data);
void SetBuffer(uint16_t length, uint16_t* p, uint16_t rgb565);
void SetBufferColor(uint16_t length, uint16_t* p, Color rgb565);

void TestAnimation(FrameBuffer f, Rect_16 r, Color color1);

GraphicsInterface GetGraphicsInterface()
{
    GraphicsInterface graphics = {
        .GetFrameBufferFront = GetFrameBufferFront,
        .GetFrameBufferBack = GetFrameBufferBack,
        .GetFrameBuffer1byte = GetFrameBuffer1byte,
        .GetFrameBuffer2bytes = GetFrameBuffer2bytes,
        .GetBufferWidth = GetBufferWidth,
        .GetBufferHeight = GetBufferHeight,
        .ClearBuffer = ClearBuffer,
        .DrawBuffer = DrawBuffer,
        .DrawSprite = DrawSprite,
        .DrawToBuffer = DrawToBuffer,
        .DrawTileKeyed = DrawTileKeyed,
        .Draw = Draw,
        .SetBuffer = SetBuffer,
        .SetBufferColor = SetBufferColor,
        .SetFrameBuffer = SetFrameBuffer,
        .SetFrameBufferColor = SetFrameBufferColor,
        .FillRect = FillRect,
        .FillRectColor = FillRectColor,
        .FillScreen = FillScreen,
        .FillScreenColor = FillScreenColor,
        .TestAnimation = TestAnimation,
    };

    return graphics;
}

/**********************************************************************************************************************/
/*
**********************************************************************************************************************/
uint16_t* GetFrameBufferFront(void)
{
    return g_pico_ram.frameBuffer.front;
}

uint16_t* GetFrameBufferBack(void)
{
    return g_pico_ram.frameBuffer.back;
}

uint16_t* GetFrameBuffer2bytes(void)
{
    return g_pico_ram.frameBuffer.frameBuffer;
}

uint8_t* GetFrameBuffer1byte(void)
{
    return g_pico_ram.frameBuffer.frameBuffer1byte;
}

uint16_t GetBufferWidth(void)
{
    return BUFFER_W;
}

uint16_t GetBufferHeight(void)
{
    return BUFFER_H;
}

/**********************************************************************************************************************/
/*
**********************************************************************************************************************/
typedef struct
{
    spi_inst_t* spi;
    uint sck, mosi, miso;
    uint cs, dc, rst;
} ili9341_t;

#define TFT_WIDTH  320
#define TFT_HEIGHT 240

ili9341_t lcd;

static inline void Pico_CSSelect(void) { gpio_put(lcd.cs, 0); }
static inline void Pico_CSDeselect(void) { gpio_put(lcd.cs, 1); }

/**********************************************************************************************************************/
/**  Swap the 4 High and 4 low bits of a byte
**********************************************************************************************************************/
static inline uint16_t Pico_SetColorByte(Color c)
{
#if defined(SWAP_COLOR_BYTES)
    return (c.color >> 8) | (c.color << 8);
#else
    return x;
#endif
}

/**********************************************************************************************************************/
/**  write command to the screen
**********************************************************************************************************************/
static void Pico_WriteCmd(uint8_t cmd)
{
    gpio_put(lcd.dc, 0);
    Pico_CSSelect();
    spi_write_blocking(lcd.spi, &cmd, 1);
    Pico_CSDeselect();
}

/**********************************************************************************************************************/
/** write given pixels to the screen
**********************************************************************************************************************/
static void Pico_WriteData(const uint8_t* data, size_t len)
{
    gpio_put(lcd.dc, 1);
    Pico_CSSelect();
    spi_write_blocking(lcd.spi, data, len);
    Pico_CSDeselect();
}

/**********************************************************************************************************************/
/** write given pixels to the screen
**********************************************************************************************************************/
static void Pico_SetWindow(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1)
{
    if (x0 > x1 || y0 > y1)
    {
        DEBUG("Rejected bad window: %u,%u → %u,%u", x0, y0, x1, y1);
        return;
    }

    if (x1 >= TFT_WIDTH) x1 = TFT_WIDTH - 1;
    if (y1 >= TFT_HEIGHT) y1 = TFT_HEIGHT - 1;
    if (x0 >= TFT_WIDTH || y0 >= TFT_HEIGHT) return;

    uint8_t buf[4];

    Pico_WriteCmd(0x2A); // CASET
    buf[0] = x0 >> 8;
    buf[1] = x0 & 0xFF;
    buf[2] = x1 >> 8;
    buf[3] = x1 & 0xFF;
    Pico_WriteData(buf, 4);

    Pico_WriteCmd(0x2B); // PASET
    buf[0] = y0 >> 8;
    buf[1] = y0 & 0xFF;
    buf[2] = y1 >> 8;
    buf[3] = y1 & 0xFF;
    Pico_WriteData(buf, 4);

    Pico_WriteCmd(0x2C); // RAMWR
}

/**********************************************************************************************************************/
/**  Init the LCD display for a landscape view
**********************************************************************************************************************/

void Pico_ili9341_Init(void)
{
    // init LCD
    lcd.spi = SPI_B;
    lcd.sck = SPIB_SCK;
    lcd.mosi = SPIB_MOSI;
    lcd.cs = CS;
    lcd.dc = DC;
    lcd.rst = RST;


    // GPIO init
    gpio_init(lcd.cs);
    gpio_set_dir(lcd.cs, GPIO_OUT);
    gpio_put(lcd.cs, 0);
    // gpio_put(lcd.cs, 1);
    gpio_init(lcd.dc);
    gpio_set_dir(lcd.dc, GPIO_OUT);
    gpio_put(lcd.dc, 1);
    gpio_init(lcd.rst);
    gpio_set_dir(lcd.rst, GPIO_OUT);

    // SPI pins
    spi_init(lcd.spi, 20 * 1000 * 1000); //testing higher,  40 MHz is often OK; if flaky, drop to 20MHz.
    gpio_set_function(lcd.sck, GPIO_FUNC_SPI);
    gpio_set_function(lcd.mosi, GPIO_FUNC_SPI);

    // Reset
    gpio_put(lcd.rst, 0);
    sleep_ms(50);
    gpio_put(lcd.rst, 1);
    sleep_ms(120);

#if defined(ILI9341)
    Pico_WriteCmd(0x01);
    sleep_ms(10); // Software reset
    Pico_WriteCmd(0x28); // Display OFF

    // Pixel format: 16-bit
    Pico_WriteCmd(0x3A);
    uint8_t pf = 0x55; // 16bpp
    Pico_WriteData(&pf, 1);

    Pico_WriteCmd(0x36);
    uint8_t madctl =
        (1 << 7) | //  Row order
        (0 << 6) | // Column order
        (1 << 5) | // Row/column exchange (set landscape)
        (0 << 4) | //  vertical refresh order
        (0 << 3) | // BGR: BGR order
        (0 << 2); // horizontal refresh order

    Pico_WriteData(&madctl, 1);

    Pico_WriteCmd(0x11);
    sleep_ms(120); // Sleep Out
    Pico_WriteCmd(0x29);
    sleep_ms(20); // Display ON
#elif defined(ST7796S)
    Pico_WriteCmd(0x01);
    sleep_ms(10); // Software reset
    Pico_WriteCmd(0x11);
    sleep_ms(120); // Sleep out

    Pico_WriteCmd(0x36); // MADCTL
    uint8_t madctl = 0xE8; // Example: BGR, swap XY for landscape? Adjust as needed

    Pico_WriteData(&madctl, 1);

    Pico_WriteCmd(0x3A); // Pixel format

    uint8_t pf = 0x55; // 16bpp
    Pico_WriteData(&pf, 1);

    Pico_WriteCmd(0x29);
    sleep_ms(20); // Display on
#else
#error "Define ILI9341 or ST7796S"
#endif
}

/**********************************************************************************************************************/
/**  Draws a rect to the screen of the given position, dimensions and colour
 *  recursively splits the rect size to fit the partial frame buff
**********************************************************************************************************************/
void Pico_ili9341_DrawRect(uint16_t x, uint16_t y, uint16_t w, uint16_t h, Color rgb565)
{
    // if ((w * h) < (BUFFER_SIZE * 2))
    if ((w * h) < BUFFER_SIZE_2BYTES)
    {
        SetFrameBufferColor(rgb565);
        Draw(x, y, w, h, g_pico_ram.frameBuffer.frameBuffer1byte);
    }
    else
    {
        if (w > h)
        {
            w = w / 2;
            FillRectColor(x, y, w, h, rgb565);
            x += w;
            FillRectColor(x, y, w, h, rgb565);
        }
        else
        {
            h = h / 2;
            FillRectColor(x, y, w, h, rgb565);
            y += h;
            FillRectColor(x, y, w, h, rgb565);
        }
    }
}

/**********************************************************************************************************************/
/**  Validates the rect given rect
 *  Calls the Draw rect function to the screen of the given position, dimensions and colour
**********************************************************************************************************************/
void FillRect(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t rgb565)
{
    if ((w == 0 || h == 0) || (x >= TFT_W || y >= TFT_H))
        return;

    if (x + w > TFT_W) w = TFT_W - x;
    if (y + h > TFT_H) h = TFT_H - y;

    Color c = {.color = rgb565};
    Pico_ili9341_DrawRect(x, y, w, h, c);
}


/**********************************************************************************************************************/
/**  Validates the rect given rect
 *  Calls the Draw rect function to the screen of the given position, dimensions and colour
**********************************************************************************************************************/
void FillRectColor(uint16_t x, uint16_t y, uint16_t w, uint16_t h, Color rgb565)
{
    if ((w == 0 || h == 0) || (x >= TFT_W || y >= TFT_H))
        return;

    if (x + w > TFT_W) w = TFT_W - x;
    if (y + h > TFT_H) h = TFT_H - y;

    Pico_ili9341_DrawRect(x, y, w, h, rgb565);
}

/**********************************************************************************************************************/
/**  Fills the screen with the given colour
**********************************************************************************************************************/
void FillScreen(uint16_t rgb565)
{
    Color c = {.color = rgb565};
    FillRectColor(0, 0, TFT_W, TFT_H, c);
}


/**********************************************************************************************************************/
/**  Fills the screen with the given colour
**********************************************************************************************************************/
void FillScreenColor(Color rgb565)
{
    FillRectColor(0, 0, TFT_W, TFT_H, rgb565);
}

/**********************************************************************************************************************/
/**  Blits the buffer to the screen with the given position and dimensions
**********************************************************************************************************************/
void Draw(uint16_t x, uint16_t y, uint16_t w, uint16_t h, const uint8_t* data)
{
    if ((w == 0 || h == 0) || (x >= TFT_W || y >= TFT_H))
        return;
    if (x + w > TFT_W) w = TFT_W - x;
    if (y + h > TFT_H) h = TFT_H - y;

    Pico_SetWindow(x, y, x + w - 1, y + h - 1);
    gpio_put(lcd.dc, 1);
    Pico_CSSelect();
    spi_write_blocking(lcd.spi, data, w * h * sizeof(uint16_t));
    Pico_CSDeselect();
}

/**********************************************************************************************************************/
/**  writes the uint16_t pixel values into the frame buffer at the given rect posiotion and dimensions
**********************************************************************************************************************/
void DrawToBuffer(const FrameBuffer frameBuffer, const uint16_t* pixels, const Rect_16 rect)
{
    uint16_t width = frameBuffer.w;
    // = g_gameFlash.GetColor[PAL_KEY];

    uint16_t clip_x = 0;
    uint16_t clip_y = 0;
    uint16_t dest_x = rect.x;
    uint16_t dest_y = rect.y;

    // handle out of bounds drawing
    if (rect.x < 0)
    {
        clip_x = -rect.x;
        dest_x = 0;
    }
    if (rect.y < 0)
    {
        clip_y = -rect.y;
        dest_y = 0;
    }

    for (uint16_t y = clip_y; y < rect.h; y++)
    {
        for (uint16_t x = clip_x; x < rect.w; x++)
        {
            Color color;
            color.color = pixels[(y * rect.w) + x];
            if (color.color == TRANSPARENCY) continue;

            uint16_t screen_x = dest_x + x;
            uint16_t screen_y = dest_y + y;
            uint16_t index = (screen_y * width) + screen_x;

            // g_run.tileCache.frameBuffer.frameBuffer[index] = Pico_SwapBytes(color);
            g_pico_ram.frameBuffer.frameBuffer[index] = Pico_SetColorByte(color);
        }
    }
}

/**********************************************************************************************************************/
/**  fills the given buffer with the given colour value
**********************************************************************************************************************/
void Pico_ili9341_SetRectColor(uint16_t length, uint16_t* p, Color rgb565)
{
    for (uint16_t i = 0; i < length; i++)
        p[i] = Pico_SetColorByte(rgb565);
}

/**********************************************************************************************************************/
/**  fills the frame buffer with the given colour value
**********************************************************************************************************************/
void SetFrameBufferColor(Color rgb565)
{
    Pico_ili9341_SetRectColor(BUFFER_SIZE_2BYTES, g_pico_ram.frameBuffer.frameBuffer, rgb565);
    // Pico_ili9341_SetRectColor(BUFFER_SIZE * 2, g_run.tileCache.frameBuffer.frameBuffer, rgb565);
}

/**********************************************************************************************************************/
/**  fills the frame buffer with the given colour value
**********************************************************************************************************************/
void SetFrameBuffer(uint16_t rgb565)
{
    Color c = {.color = rgb565};
    Pico_ili9341_SetRectColor(BUFFER_SIZE_2BYTES, g_pico_ram.frameBuffer.frameBuffer, c);
    // Pico_ili9341_SetRectColor(BUFFER_SIZE * 2, g_run.tileCache.frameBuffer.frameBuffer, rgb565);
}

/**********************************************************************************************************************/
/**  fills the frame buffer with the given colour value
**********************************************************************************************************************/
void SetBuffer(uint16_t length, uint16_t* p, uint16_t rgb565)
{
    Color c = {.color = rgb565};
    Pico_ili9341_SetRectColor(length, p, c);
}

/**********************************************************************************************************************/
/**  fills the frame buffer with the given colour value
**********************************************************************************************************************/
void SetBufferColor(uint16_t length, uint16_t* p, Color rgb565)
{
    Pico_ili9341_SetRectColor(length, p, rgb565);
}

/**********************************************************************************************************************/
/**  clears the frame buffer pixel colour values
**********************************************************************************************************************/
void ClearBuffer()
{
    // Pico_ili9341_SetRectColor(BUFFER_SIZE * 2, partialFrameBuffer, g_gameFlash.GetColor[PAL_BRIGHT_RED]);
    Color c = {.color = 0xFFFF};
    Pico_ili9341_SetRectColor(BUFFER_SIZE_2BYTES, g_pico_ram.frameBuffer.frameBuffer, c);
}

/**********************************************************************************************************************/
/**  Blits the frame buffer to the screeen
**********************************************************************************************************************/
void DrawBuffer(const FrameBuffer f)
{
    Draw(f.x, f.y, f.w, f.h, g_pico_ram.frameBuffer.frameBuffer1byte);
}

/**********************************************************************************************************************/
/**
**********************************************************************************************************************/
void DrawSprite(const FrameBuffer f, const uint8_t* sprite)
{
    Draw(f.x, f.y, f.w, f.h, sprite);
}

/**********************************************************************************************************************/
/**  TEMP - for testing animations
 *      Draws a background and 3 color bars, useful for ensuring colours correct
 *
**********************************************************************************************************************/
void Pico_TestFrameBuffer()
{
    SetFrameBuffer(0xd6fa); // gray
    FrameBuffer f = {50, 100, 64, 80, 0xd6fa};


    Rect_16 r = {5, 20, 10, 40};
    uint16_t p[10 * 40];

    r.x = 5;
    Color c = {.color = 0x001f};
    Pico_ili9341_SetRectColor(10 * 40, p, c); //blu
    DrawToBuffer(f, p, r);

    r.x = 25;
    c.color = 0x07e0;
    Pico_ili9341_SetRectColor(10 * 40, p, c); //grn
    DrawToBuffer(f, p, r);

    r.x = 45;
    c.color = 0xf81f;
    Pico_ili9341_SetRectColor(10 * 40, p, c); //red
    DrawToBuffer(f, p, r);

    DrawBuffer(f);
}

/**********************************************************************************************************************/
/**  TEMP - for testing animations
**********************************************************************************************************************/
void TestAnimation(FrameBuffer f, Rect_16 r, Color color1)
{
    SetFrameBuffer(0xd6fa); // gray

    uint16_t size = r.w * r.h;
    uint16_t p[size];

    Pico_ili9341_SetRectColor(size, p, color1); //blu
    DrawToBuffer(f, p, r);
    DrawBuffer(f);
}


/**********************************************************************************************************************/
/**  Color bars to check proper colour
**********************************************************************************************************************/
void Pico_TestColors()
{
    uint16_t width = 40;
    FrameBuffer f = {.x = 0, .y = 0, .w = width, .h = 320};

    uint16_t colors[8] =
    {
        0xFFFF, //White
        0xFFE0, //Yellow
        0x07FF, //Cyan
        0x0400, //Green
        0xF81F, //Magenta
        0xF800, //Red
        0x001F, //Blue
        0x0000, //Black
    };


    for (uint8_t i = 0; i < 8; i++)
    {
        Color c = {.color = colors[i]};
        SetFrameBufferColor(c);
        DrawBuffer(f);
        f.x += width;
    }
}

/**********************************************************************************************************************/
/**  Test text drawn to the screen
**********************************************************************************************************************/
void Pico_TestText()
{
    uint16_t width = 40;
    FrameBuffer f = {.x = 0, .y = 0, .w = width, .h = 320};

    uint16_t colors[8] =
    {
        0xFFFF, //White
        0xFFE0, //Yellow
        0x07FF, //Cyan
        0x0400, //Green
        0xF81F, //Magenta
        0xF800, //Red
        0x001F, //Blue
        0x0000, //Black
    };
}

/**********************************************************************************************************************/
/**  blits pixels to the screen at the given position and dimensions
 *  ignores transparent keyed pixels
 *  many SPI transfers, quite slow but can draw to portions of the screen using transparency, use sparingly
**********************************************************************************************************************/
void DrawTileKeyed(uint16_t x, uint16_t y, uint16_t w, uint16_t h, const uint16_t* data)
{
    // DEBUG("data ptr %p", data);
    // DEBUG("BLIT x=%d y=%d w=%d h=%d", x, y, w, h);

    static uint8_t linebuf[64 * 2];

    // DEBUG("start DrawKeyed");
    for (uint16_t row = 0; row < h; row++)
    {
        // DEBUG("getting new");
        const uint16_t* src = data + row * w;
        // DEBUG("getting new success");

        uint16_t col = 0;

        while (col < w)
        {
            // DEBUG("inner1 %d %d", col, w);

            // Skip transparent pixels
            if (src[col] == TRANSPARENCY)
            {
                col++;
                continue;
            }

            // Start of visible span
            uint16_t start = col;

            // Advance until transparent or end
            while (col < w && src[col] != TRANSPARENCY)
            {
                col++;
            }

            uint16_t span = col - start;
            if (span > w)
                span = w;

            uint16_t remaining = span;
            uint16_t offset = 0;


            while (remaining)
            {
                uint16_t chunk = remaining > 64 ? 64 : remaining;

                for (uint16_t i = 0; i < chunk; i++)
                {
                    uint16_t idx = start + offset + i;
                    if (idx >= w)
                    {
                        // DEBUG("OOB idx=%d w=%d start=%d offset=%d i=%d", idx, w, start, offset, i);
                        break;
                    }
                    uint16_t px = src[start + offset + i];
                    linebuf[i * 2] = px >> 8;
                    linebuf[i * 2 + 1] = px & 0xFF;
                }

                uint16_t screen_left = x + start + offset;
                uint16_t screen_top = y + row;
                uint16_t screen_right = screen_left + chunk - 1;
                uint16_t screen_bottom = screen_top; // horizontal strip

                // Hard clamp — ILI9341 is usually 0..319 × 0..239 in your rotation

                if (screen_left >= TFT_WIDTH)
                {
                    // fully off right — skip this chunk
                    offset += chunk;
                    remaining -= chunk;
                    continue;
                }

                if (screen_top >= TFT_HEIGHT)
                {
                    // fully off bottom — can break early in row loop if you want
                    offset += chunk;
                    remaining -= chunk;
                    continue;
                }

                // Shrink if hanging off right/bottom
                if (screen_right >= TFT_WIDTH) screen_right = TFT_WIDTH - 1;
                if (screen_bottom >= TFT_HEIGHT) screen_bottom = TFT_HEIGHT - 1;

                // Critical: skip if window became invalid (prevents x0 > x1)
                if (screen_left > screen_right || screen_top > screen_bottom)
                {
                    // DEBUG("Invalid window skipped: L=%u R=%u T=%u B=%u", screen_left, screen_right, screen_top, screen_bottom);
                    offset += chunk;
                    remaining -= chunk;
                    continue;
                }

                Pico_SetWindow(
                    x + start + offset,
                    y + row,
                    x + start + offset + chunk - 1,
                    y + row
                );

                gpio_put(lcd.dc, 1);
                Pico_CSSelect();

                spi_write_blocking(lcd.spi, linebuf, chunk * 2);

                Pico_CSDeselect();

                offset += chunk;
                remaining -= chunk;
            }
        }
    }
}
