#pragma once

#include "demo.h"
#include "particle.h"
#include "particle_force_generator.h"
#include <glm/glm.hpp>

struct Shader;
struct Renderer;
struct Quad;
struct Camera;

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

    Particle* pendulum0;
    Particle* pendulum1;
    glm::vec3 mouse_pos;

};

