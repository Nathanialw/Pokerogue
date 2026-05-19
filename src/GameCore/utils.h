//
// Created by nathanial on 3/8/26.
//
#pragma once

#include "utils.h"

#include "lib_types.h"
#include "types.h"


/**********************************************************************************************************************/
/** TYPES
**********************************************************************************************************************/
typedef char CharStr_uint8[4];
typedef char CharStr_max999[4];
typedef char CharStr_max99[4];
typedef char CharStr_99[3];
typedef char CharStr_uint16[6];


/**********************************************************************************************************************/
/** FUNCTIONS
**********************************************************************************************************************/

static inline void GetAsChars_uint8(uint8_t n, CharStr_uint8* out, bool prependZeros);
static inline void GetAsChars_uint16(uint16_t n, CharStr_uint16* out);
static inline void GetAsChars_Max999(uint16_t n, CharStr_max999* out);

static inline uint8_t GetBottomByte(uint8_t originalByte);
static inline uint8_t GetTopByte(uint8_t originalByte);
static inline void GetAsChars_99(Int99 n, CharStr_99* out, bool prependZeros);

static inline uint16_t Int999GetCurrent(const IntMax999* hp);
static inline uint16_t Int999GetMax(const IntMax999* hp);
static inline void Int999SetMax(IntMax999* hp, uint16_t value);
static inline void Int999SetCurrent(IntMax999* hp, uint16_t value);
static inline void Int999ApplyValue(HardwareInterface hardware, IntMax999* hp, int16_t value);

static inline bool WithinRange(uint8_t x, uint8_t y, uint8_t tx, uint8_t ty, uint8_t range);
static inline bool SortEntityArray(EntityId* sorted, EntityId* unsorted, uint8_t n);

static inline void* memset(void* dst, int value, size_t len);
static inline void* memcpy(void* dst, const void* src, size_t len);

/**********************************************************************************************************************/
/** BIT ARRAY MANIPULATION
**********************************************************************************************************************/
static inline bool SetBit(uint8_t* bitField, uint8_t idx, bool set);
static inline bool GetBit(const uint8_t* bitField, uint8_t idx);
static inline bool ToggleBit(uint8_t* bitField, uint8_t idx);

/**********************************************************************************************************************/
/** STRING MANIPULATION
**********************************************************************************************************************/
static inline bool IsHyphenationPoint(HardwareInterface hardware, const char* word, uint8_t pos, uint8_t word_len);

/**********************************************************************************************************************/
/** IMPLEMENTATION DEFINED
**********************************************************************************************************************/
#define CHAR_OFFSET 48

/**********************************************************************************************************************/
/** takes a char array and a uin8_t and writes the value og the integer as chars to the array
**********************************************************************************************************************/
SET_MEMORY(".core")
static inline void GetAsChars_uint8(uint8_t n, CharStr_uint8* out, bool prependZeros)
{
    uint8_t hundreds = ((n / 100) % 10);
    uint8_t tens = ((n / 10) % 10);
    uint8_t ones = (n % 10);

    if (!prependZeros && CHAR_OFFSET + hundreds == '0')
        (*out)[0] = ' ';
    else
        (*out)[0] = CHAR_OFFSET + hundreds;

    if (!prependZeros && CHAR_OFFSET + tens == '0')
        (*out)[1] = ' ';
    else
        (*out)[1] = CHAR_OFFSET + tens;

    (*out)[2] = CHAR_OFFSET + ones;
    (*out)[3] = '\0';
}

/**********************************************************************************************************************/
/** takes a char array and a uin16_t and writes the value og the integer as chars to the array
**********************************************************************************************************************/
SET_MEMORY(".core")
static inline void GetAsChars_uint16(uint16_t n, CharStr_uint16* out)
{
    uint8_t ten_thousands = (n / 10000) % 10;
    uint8_t thousands = (n / 1000) % 10;
    uint8_t hundreds = (n / 100) % 10;
    uint8_t tens = (n / 10) % 10;
    uint8_t ones = n % 10;

    (*out)[0] = CHAR_OFFSET + ten_thousands;
    (*out)[1] = CHAR_OFFSET + thousands;
    (*out)[2] = CHAR_OFFSET + hundreds;
    (*out)[3] = CHAR_OFFSET + tens;
    (*out)[4] = CHAR_OFFSET + ones;
    (*out)[5] = '\0';
}

/**********************************************************************************************************************/
/** takes a char array and a uint16_t and writes the value og the integer as chars to the array
**********************************************************************************************************************/
SET_MEMORY(".core")
static inline void GetAsChars_Max999(uint16_t n, CharStr_max999* out)
{
    uint8_t hundreds = (n / 100) % 10;
    uint8_t tens = (n / 10) % 10;
    uint8_t ones = n % 10;

    (*out)[0] = CHAR_OFFSET + hundreds;
    (*out)[1] = CHAR_OFFSET + tens;
    (*out)[2] = CHAR_OFFSET + ones;
    (*out)[3] = '\0';
}

/**********************************************************************************************************************/
/** takes a char array and a uint8_t and writes the value og the integer as chars to the array
**********************************************************************************************************************/
SET_MEMORY(".core")
static inline void GetAsChars_Max99(uint8_t n, CharStr_max99* out)
{
    uint8_t hundreds = (n / 100) % 10;
    uint8_t tens = (n / 10) % 10;
    uint8_t ones = n % 10;

    (*out)[0] = CHAR_OFFSET + hundreds;
    (*out)[1] = CHAR_OFFSET + tens;
    (*out)[2] = CHAR_OFFSET + ones;
    (*out)[3] = '\0';
}

/**********************************************************************************************************************/
/** takes a char array and a uint8_t and writes the value og the integer as chars to the array
**********************************************************************************************************************/
SET_MEMORY(".core")
static inline void GetAsChars_99(Int99 n, CharStr_99* out, bool prependZeros)
{
    uint8_t tens = (n.value / 10) % 10;
    uint8_t ones = n.value % 10;

    if (!prependZeros && CHAR_OFFSET + tens == '0')
        (*out)[0] = ' ';
    else
        (*out)[0] = CHAR_OFFSET + tens;
    (*out)[1] = CHAR_OFFSET + ones;
    (*out)[2] = '\0';
}

/**********************************************************************************************************************/
/** Returns the top 4 bits of the given byte
**********************************************************************************************************************/
SET_MEMORY(".core")
static inline uint8_t GetTopByte(uint8_t originalByte)
{
    return originalByte >> 4;
}

/**********************************************************************************************************************/
/** Returns the bottom 4 bits of the given byte
**********************************************************************************************************************/
SET_MEMORY(".core")
static inline uint8_t GetBottomByte(uint8_t originalByte)
{
    return originalByte & 0x0F;
}

/**********************************************************************************************************************/
/**
**********************************************************************************************************************/
SET_MEMORY(".core")
static inline uint8_t GetNibble(uint8_t* bitField, uint8_t id)
{
    if (id % 2 == 0)
        return GetTopByte(bitField[id / 2]);
    return GetBottomByte(bitField[id / 2]);
}

/**********************************************************************************************************************/
/** Returns the current value of the given IntMax999
**********************************************************************************************************************/
SET_MEMORY(".core")
static inline uint16_t Int999GetCurrent(const IntMax999* hp)
{
    return (hp->current_high << 8) | hp->current_low;
}

/**********************************************************************************************************************/
/** Returns the max value of the given IntMax999
**********************************************************************************************************************/
SET_MEMORY(".core")
static inline uint16_t Int999GetMax(const IntMax999* hp)
{
    return (hp->max_high << 8) | hp->max_low;
}

/**********************************************************************************************************************/
/** Sets the current value of the given IntMax999 to the given value
**********************************************************************************************************************/
SET_MEMORY(".core")
static inline void Int999SetCurrent(IntMax999* hp, uint16_t value)
{
    hp->current_low = value & 0xFF;
    hp->current_high = (value >> 8) & 0x03;
}

/**********************************************************************************************************************/
/** Sets the max value of the given IntMax999 to the given value
**********************************************************************************************************************/
SET_MEMORY(".core")
static inline void Int999SetMax(IntMax999* hp, uint16_t value)
{
    hp->max_low = value & 0xFF;
    hp->max_high = (value >> 8) & 0x03;
}

SET_MEMORY(".core")
static inline void Int999ApplyValue(HardwareInterface hardware, IntMax999* hp, int16_t value)
{
    uint16_t n = Int999GetCurrent(hp);
    if (n >= hardware.Abs(value))
    {
        n += value;
        Int999SetCurrent(hp, n);
    }
    else
    {
        Int999SetCurrent(hp, 0);
    }
}


/**********************************************************************************************************************/
/** Checks whether tx, ty is with the range of x,y
 *  ON SUCCESS - returns true
 *  ON FAIL - returns false
**********************************************************************************************************************/
SET_MEMORY(".map")
static inline bool WithinRange(uint8_t x, uint8_t y, uint8_t tx, uint8_t ty, uint8_t range)
{
    if (tx >= x - range && tx < (x + range) && ty >= y - range && ty < (y + range))
        return true;
    return false;
}


/**********************************************************************************************************************/
/** handles line breaks on:
 *      -double letter pattern words
 *      -syllable
**********************************************************************************************************************/
SET_MEMORY(".core")
static inline bool IsHyphenationPoint(HardwareInterface hardware, const char* word, uint8_t pos, uint8_t word_len)
{
    // Don't hyphenate at beginning or end
    if (pos < 2 || pos > word_len - 2) return false;

    // Simple rule: hyphenate between double consonants or after prefix
    char c1 = word[pos - 1];
    char c2 = word[pos];
    char c3 = word[pos + 1];

    // Common consonant pairs
    if ((c1 == 'b' && c2 == 'b') || (c1 == 'd' && c2 == 'd') ||
        (c1 == 'l' && c2 == 'l') || (c1 == 'm' && c2 == 'm') ||
        (c1 == 'n' && c2 == 'n') || (c1 == 'p' && c2 == 'p') ||
        (c1 == 'r' && c2 == 'r') || (c1 == 's' && c2 == 's') ||
        (c1 == 't' && c2 == 't'))
    {
        return true;
    }

    // Vowel-consonant patterns (very simplified)
    char vowels[] = "aeiou";
    bool is_vowel_c1 = (hardware.StrChr(vowels, c1) != NULL);
    bool is_vowel_c2 = (hardware.StrChr(vowels, c2) != NULL);

    // Vowel followed by consonant is often a syllable break
    if (is_vowel_c1 && !is_vowel_c2 && pos < word_len - 2)
    {
        // Check if next is vowel (makes it a good break)
        bool is_vowel_c3 = (hardware.StrChr(vowels, c3) != NULL);
        if (is_vowel_c3) return true;
    }

    return false;
}

/********************************************************************************************************************************************************************************************************************************************/
/* *
 *  bit field functions
 *
/********************************************************************************************************************************************************************************************************************************************/

/**********************************************************************************************************************/
/**Takes an array on uint8_t
 * sets the bit at the given index to the set value
 * Returns the bit value at the given index
**********************************************************************************************************************/
SET_MEMORY(".core")
static inline bool SetBit(uint8_t* bitField, uint8_t idx, bool set)
{
    uint32_t byteIndex = idx >> 3; // / 8 faster on pico ARM
    uint32_t bitIndex = idx & 0x07; // % 8 faster on pico ARM
    uint8_t mask = (uint8_t)(1u << bitIndex);

    uint8_t* bytePtr = &bitField[byteIndex];
    uint8_t old = *bytePtr;

    if (set)
    {
        *bytePtr |= mask;
    }
    else
    {
        *bytePtr &= (uint8_t)~mask;
    }

    return *bytePtr != old;
}

/**********************************************************************************************************************/
/** Takes an array on uint8_t
 *  Returns the bit value at the given index
**********************************************************************************************************************/
SET_MEMORY(".core")
static inline bool GetBit(const uint8_t* bitField, uint8_t idx)
{
    uint32_t byteIndex = idx >> 3;
    uint32_t bitIndex = idx & 0x07;
    uint8_t mask = (uint8_t)(1u << bitIndex);

    return (bitField[byteIndex] & mask) != 0;
}

/**********************************************************************************************************************/
/**Takes an array on uint8_t
 * flips the bit at the given index
 * Returns the bit value at the given index
**********************************************************************************************************************/
SET_MEMORY(".core")
static inline bool ToggleBit(uint8_t* bitField, uint8_t idx)
{
    uint32_t byteIndex = idx >> 3;
    uint32_t bitIndex = idx & 0x07;
    uint8_t mask = (uint8_t)(1u << bitIndex);

    uint8_t* bytePtr = &bitField[byteIndex];
    *bytePtr ^= mask; // Toggle with XOR

    return (*bytePtr & mask) != 0; // Return new state
}

/**********************************************************************************************************************/
/*
**********************************************************************************************************************/
SET_MEMORY(".map")
static inline bool SortEntityArray(EntityId* sorted, EntityId* unsorted, uint8_t n)
{
    // Copy unsorted to sorted first
    for (uint8_t i = 0; i < n; i++)
    {
        sorted[i] = unsorted[i];
    }

    // Selection sort
    for (uint8_t i = 0; i < n - 1; i++)
    {
        uint8_t min_idx = i;
        for (uint8_t j = i + 1; j < n; j++)
        {
            if (sorted[j] < sorted[min_idx])
            {
                min_idx = j;
            }
        }

        // Swap if needed
        if (min_idx != i)
        {
            EntityId temp = sorted[i];
            sorted[i] = sorted[min_idx];
            sorted[min_idx] = temp;
        }
    }

    return true;
}



SET_MEMORY(".core")
static inline void* memcpy(void* dst, const void* src, size_t len)
{
    uint8_t* d = (uint8_t*)dst;
    const uint8_t* s = (const uint8_t*)src;

    while(len--)
    {
        *d++ = *s++;
    }

    return dst;
}

SET_MEMORY(".core")
static inline void* memset(void* dst, int value, size_t len)
{
    uint8_t* d = (uint8_t*)dst;

    while(len--)
    {
        *d++ = (uint8_t)value;
    }

    return dst;
}
