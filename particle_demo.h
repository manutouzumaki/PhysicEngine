#pragma once

#include "demo.h"

#include <glm/glm.hpp>

struct Shader;
struct Renderer;
struct Quad;
struct Camera;

struct Particle {
    glm::vec3 position;
    glm::vec3 velocity;
    glm::vec3 acceleration;
    glm::vec3 force_accum;
    f32 damping;
    f32 inv_mass;
};

Particle particle_create(glm::vec3 position,
                         glm::vec3 velocity,
                         glm::vec3 acceleration,
                         f32 damping, f32 mass);
void particle_integrate(Particle* particle, f32 duration);
void particle_add_force(Particle* particle, glm::vec3 force);
void particle_clear_forces(Particle* particle);

struct ParticleDemo : public Demo
{
    void start() override;
    void update(Input* input, f32 dt) override;
    void render() override;
    void destroy() override;

    Shader* shader;
    Renderer* renderer;
    Quad* quad;
    Camera* camera;

    Particle projectil;
};

