#pragma once

#include "defines.h"
#include <stdlib.h>
#include <glm/glm.hpp>

f32 randf()
{
    f32 result = (f32)rand() / (f32)RAND_MAX;
    return result;
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