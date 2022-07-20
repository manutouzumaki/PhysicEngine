#pragma once

#include "defines.h"
#include <Windows.h>

struct Input;

struct Platform
{
    HWND window;
    HDC device_context;
    i32 window_width;
    i32 window_height;
    b32 *running;
    Input* current_input;
    Input* last_input;
    
    f64 inv_frequency;
    f64 last_time;
    f64 current_time;

    f32 delta_time;
};

Platform* platform_create(i32 x, i32 y, i32 width, i32 height, const char* name);
void platform_begin_frame(Platform *platform);
void platform_end_frame(Platform* platform);
void platform_destroy(Platform *platform);