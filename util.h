#pragma once

#include "defines.h"
#include <glm/glm.hpp>
#include <math.h>

inline f32 vec3_length(glm::vec3 v) {
    return sqrtf(v.x*v.x + v.y*v.y + v.z*v.z);
}
