#pragma once

#include "defines.h"
#include <vector>
#include <glm/glm.hpp>

struct Particle;

class ParticleForceGenerator
{
public:
    virtual void update_force(Particle* particle, f32 duration) = 0;
};

class ParticleForceRegistry {
    struct ParticleForceRegistration {
        Particle* particle;
        ParticleForceGenerator* fg;
    };
protected:
    typedef std::vector<ParticleForceRegistration> Registry;
    Registry registrations;
public:
    void add(Particle* particle, ParticleForceGenerator* fg);
    void update_forces(f32 duration);
    void clear();
};

class ParticleGravity : public ParticleForceGenerator {
    glm::vec3 gravity;
public:
    ParticleGravity(glm::vec3 gravity);
    virtual void update_force(Particle* particle, f32 duration);
};

class ParticleDrag : public ParticleForceGenerator {
    f32 k1, k2;
public:
    ParticleDrag(f32 k1, f32 k2);
    virtual void update_force(Particle* particle, f32 duration);
};

class ParticleSpring : public ParticleForceGenerator {
    Particle* other;
    f32 spring_constant;
    f32 rest_length;
public:
    ParticleSpring(Particle* other, f32 spring_constant, f32 rest_length);
    virtual void update_force(Particle* particle, f32 duration);
};

class ParticleAnchoredSpring : public ParticleForceGenerator {
    glm::vec3* anchor;
    f32 spring_constant;
    f32 rest_length;
public:
    ParticleAnchoredSpring(glm::vec3 *anchor, f32 spring_constant, f32 rest_length);
    virtual void update_force(Particle* particle, f32 duration);
};

