#pragma once
#include <glm/glm.hpp>
#include "defines.h"

class Particle {
public:
    glm::vec3 position;
    glm::vec3 velocity;
    glm::vec3 acceleration;
    glm::vec3 force_accum;
    f32 damping;
    f32 inv_mass;

    Particle();

    void integrate(f32 duration);
    void add_force(glm::vec3 force);
    void clear_forces();
    bool has_finite_mass();
};