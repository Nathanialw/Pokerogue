//
// Created by nathanial on 5/2/26.
//
#pragma once
#include "lib_memory.h"


/**********************************************************************************************************************/
/**
**********************************************************************************************************************/
#ifdef ENABLE_ASSERTS
#include <stdio.h>
#include <stdlib.h>
#define ASSERT(cond, fmt, ...) \
do { \
if (!(cond)) { \
fprintf(stderr, "[ASSERT] %s:%d: " fmt "\n", __FILE__, __LINE__, ##__VA_ARGS__); \
fflush(stderr); \
abort(); \
}} while(0)
#else
#define ASSERT(cond, fmt, ...) ((void)0)
#endif

#ifdef ENABLE_DEBUG_OUTPUT
#include <stdio.h>
/**********************************************************************************************************************/
/** Prints only when #ifdef ENABLE_DEBUG_OUTPUT is set
 *  Handles formatted strings with as printf
**********************************************************************************************************************/
#define DEBUG(fmt, ...) \
do { fprintf(stderr, "[DEBUG] %s:%d: " fmt "\n", __FILE__, __LINE__, ##__VA_ARGS__); fflush(stderr);} while(0)
#define DEBUG_INLINE(fmt, ...) \
do { fprintf(stderr, fmt, ##__VA_ARGS__); fflush(stderr);} while(0)
#define PRINT(fmt, ...) \
do { fprintf(stderr, fmt , __FILE__, __LINE__, ##__VA_ARGS__); fflush(stderr);} while(0)
#define PRINT_INLINE(fmt, ...) \
do { fprintf(stderr, fmt, ##__VA_ARGS__); fflush(stderr);} while(0)
#else
#define DEBUG(fmt, ...) do {} while(0) // this should be optimized out by the compiler
#define DEBUG_INLINE(fmt, ...) do {} while(0) // this should be optimized out by the compiler
#define PRINT(fmt, ...) do {} while(0) // this should be optimized out by the compiler
#define PRINT_INLINE(fmt, ...) do {} while(0) // this should be optimized out by the compiler
#endif



