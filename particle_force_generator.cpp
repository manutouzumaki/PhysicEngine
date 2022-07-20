#include "particle_force_generator.h"
#include "particle.h"
#include "util.h"

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
    f32 drag_cof = vec3_length(force);
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
    f32 magnitude = vec3_length(force);
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
    f32 magnitude = vec3_length(force);
    magnitude = fabsf(magnitude - rest_length);
    magnitude *= spring_constant;

    // calculate the final force and apply it.
    force = glm::normalize(force);
    force *= -magnitude;
    particle->add_force(force);
}

ParticleBungee::ParticleBungee(Particle* other, f32 spring_constant, f32 rest_length)
{
    this->other = other;
    this->spring_constant = spring_constant;
    this->rest_length = rest_length;
}

void ParticleBungee::update_force(Particle* particle, f32 duration)
{
    glm::vec3 force = particle->position;
    force -= other->position;

    // chack if bungee is compressed
    f32 magnitude = vec3_length(force);
    if (magnitude <= rest_length) return;

    // calculate the magnitude of the force
    magnitude = spring_constant * (rest_length - magnitude);

    // calculate the final force and apply it.
    force = glm::normalize(force);
    force *= -magnitude;
    particle->add_force(force);
}

ParticleAnchorBungee::ParticleAnchorBungee(glm::vec3 *anchor, f32 spring_constant, f32 rest_length)
{
    this->anchor = anchor;
    this->spring_constant = spring_constant;
    this->rest_length = rest_length;
}

void ParticleAnchorBungee::update_force(Particle* particle, f32 duration)
{
    // TODO: check this ...
    glm::vec3 force = particle->position;
    force -= *anchor;
    // chack if bungee is compressed
    f32 magnitude = vec3_length(force);
    if (magnitude <= rest_length) return;

    // calculate the magnitude of the force
    magnitude = spring_constant * (rest_length - magnitude);

    // calculate the final force and apply it.
    force = glm::normalize(force);
    force *= magnitude;
    particle->add_force(force);
    
}
