#pragma once

#include <stdint.h>

typedef uint64_t u64;
typedef uint32_t u32;
typedef uint16_t u16;
typedef uint8_t  u8;

typedef int64_t i64;
typedef int32_t i32;
typedef int16_t i16;
typedef int8_t  i8;

typedef float  f32;
typedef double f64;

typedef uint32_t b32;
typedef bool b8;

#define Kilobyte(value) (value * 1024LL)
#define Megabyte(value) (value * 1024LL * 1024LL)
#define Gigabyte(value) (value * 1024LL * 1024LL * 1024LL)
#define Terabyte(value) (value * 1024LL * 1024LL * 1024LL * 1024LL)

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600

#define Assert(condition) if(!(condition)) { *(u32*)0 = 0;}
#define ArrayCount(array) (sizeof(array)/sizeof((array)[0]))

#define EXPORT extern "C" __declspec(dllexport)

#define FPS 60.0f
#define TARGET_SECONDS_PER_FRAME (1.0f / FPS)

#define PI 3.14159265359f

// SSE2
#define M(a, i) ((f32 *)&(a))[i]
#define Mi(a, i) ((i32 *)&(a))[i]
#define Mu(a, i) ((u32 *)&(a))[i]