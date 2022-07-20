#include "particle.h"

Particle::Particle()
{
    position = glm::vec3(0.0f);
    velocity = glm::vec3(0.0f);
    acceleration = glm::vec3(0.0f);
    force_accum = glm::vec3(0.0f);
    damping = 0.95f;
    inv_mass = 1.0f;
}

void Particle::integrate(f32 duration)
{
    Assert(duration > 0.0f);

    // update linear position
    position += velocity * duration;
    // work out the acceleration from the force

    glm::vec3 resulting_acc = acceleration;
    resulting_acc += force_accum * inv_mass;
    // update linear velocity from acceleration
    velocity += resulting_acc * duration;
    // impose drag
    velocity *= powf(damping, duration);

    clear_forces();
}

void Particle::add_force(glm::vec3 force)
{
    force_accum += force;
}

void Particle::clear_forces()
{
    force_accum = glm::vec3(0.0f);
}

bool Particle::has_finite_mass()
{
    return inv_mass >= 0.0f;
}
