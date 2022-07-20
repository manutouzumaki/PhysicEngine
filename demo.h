#pragma once

#include "defines.h"

struct Input;

struct Demo
{
    virtual void start() = 0;
    virtual void update(Input* input, f32 dt) = 0;
    virtual void render() = 0;
    virtual void destroy() = 0;
};

