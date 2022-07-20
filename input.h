#pragma once

#include "defines.h"

struct ButtonState {
    b8 is_down;
    b8 was_down;
};

struct Input
{
    ButtonState keys[350];
    i32 mouse_x;
    i32 mouse_y;
    union {
        struct {
            ButtonState mouse_left;
            ButtonState mouse_middle;
            ButtonState mouse_right;
        };
        ButtonState mouse_buttons[3];
    };
};

struct FileResult {
    void* data;
    size_t size;
};

FileResult read_file(const char *filepath);

