#include "particle_force_generator.h"
#include "particle.h"

void ParticleForceRegistry::add(Particle* particle, ParticleForceGenerator* fg)
{
    registrations.push_back({ particle, fg });
}

void ParticleForceRegistry::update_forces(f32 duration)
{
    Registry::iterator i = registrations.begin();
    for (; i != registrations.end(); i++) {
        i->fg->update_force(i->particle, duration);
    }
}

void ParticleForceRegistry::clear()
{
    Registry::iterator i = registrations.begin();
    for (; i != registrations.end(); i++) {
        delete i->fg;
    }
}

ParticleGravity::ParticleGravity(glm::vec3 gravity)
{
    this->gravity = gravity;
}

void ParticleGravity::update_force(Particle* particle, f32 duration)
{
    if (!particle->has_finite_mass()) return;

    f32 mass = (1.0f / particle->inv_mass);
    particle->add_force(gravity * mass);
}

ParticleDrag::ParticleDrag(f32 k1, f32 k2)
{
    this->k1 = k1;
    this->k2 = k2;
}

void ParticleDrag::update_force(Particle* particle, f32 duration)
{
    glm::vec3 force = particle->velocity;
    // calculate the total drag coefficient
    f32 drag_cof = force.length();
    drag_cof = k1* drag_cof + k2 * drag_cof * drag_cof;
    force = glm::normalize(force);
    force *= -drag_cof;
    particle->add_force(force);
}

ParticleSpring::ParticleSpring(Particle* other, f32 spring_constant, f32 rest_length)
{
    this->other = other;
    this->spring_constant = spring_constant;
    this->rest_length = rest_length;
}

void ParticleSpring::update_force(Particle* particle, f32 duration)
{
    // calculate the vector of the spring
    glm::vec3 force = particle->position;
    force -= other->position;

    // calculate the magnitude of the force
    f32 magnitude = force.length();
    magnitude = fabsf(magnitude - rest_length);
    magnitude *= spring_constant;

    // calculate the final force and apply it.
    force = glm::normalize(force);
    force *= -magnitude;
    particle->add_force(force);
}

ParticleAnchoredSpring::ParticleAnchoredSpring(glm::vec3* anchor, f32 spring_constant, f32 rest_length)
{
    this->anchor = anchor;
    this->spring_constant = spring_constant;
    this->rest_length = rest_length;
}

void ParticleAnchoredSpring::update_force(Particle* particle, f32 duration)
{
    // calculate the vector of the spring
    glm::vec3 force = particle->position;
    force -= *anchor;

    // calculate the magnitude of the force
    f32 magnitude = force.length();
    magnitude = fabsf(magnitude - rest_length);
    magnitude *= spring_constant;

    // calculate the final force and apply it.
    force = glm::normalize(force);
    force *= -magnitude;
    particle->add_force(force);
}
