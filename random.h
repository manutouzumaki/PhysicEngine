#pragma once

#include "defines.h"
#include <stdlib.h>
#include <glm/glm.hpp>

f32 randf()
{
    u32 bits = rand();
    union {
        f32 value;
        u32 word;
    } convert;

    convert.word = (bits >> 9) | 0x3f800000;
    return convert.value - 1.0f;
}

f32 randf(f32 min, f32 max)
{
    return randf() * (max - min) + min;
}

glm::vec3 rand_v3(glm::vec3 min, glm::vec3 max) {
    glm::vec3 result = glm::vec3(
        randf(min.x, max.x),
        randf(min.y, max.y),
        randf(min.z, max.z)
    );
    return result;
}