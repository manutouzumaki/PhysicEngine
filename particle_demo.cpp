#include "particle_demo.h"

#include "renderer.h"
#include "camera.h"
#include "random.h"

Particle particle_create(glm::vec3 position,
    glm::vec3 velocity,
    glm::vec3 acceleration,
    f32 damping, f32 mass)
{
    Particle particle = {};
    particle.position = position;
    particle.velocity = velocity;
    particle.acceleration = acceleration;
    particle.damping = damping;
    particle.inv_mass = 1.0f / mass;
    return particle;
}

void particle_integrate(Particle* particle, f32 duration)
{
    Assert(duration > 0.0f);

    // update linear position
    particle->position += particle->velocity * duration;
    // work out the acceleration from the force

    glm::vec3 resulting_acc = particle->acceleration;
    resulting_acc += particle->force_accum * particle->inv_mass;
    // update linear velocity from acceleration
    particle->velocity += resulting_acc * duration;
    // impose drag
    particle->velocity *= powf(particle->damping, duration);

    particle_clear_forces(particle);
}

void particle_add_force(Particle* particle, glm::vec3 force)
{
    particle->force_accum += force;
}

void particle_clear_forces(Particle* particle)
{
    particle->force_accum = glm::vec3(0.0f);
}

void ParticleDemo::start() {
    shader = shader_create("vertex.glsl", "fragment.glsl");
    renderer = renderer_create(shader);
    quad = quad_create();

    camera = camera_create_orthogonal(0, WINDOW_WIDTH, 0, WINDOW_HEIGHT, 0.01f, 100.0f);
    //camera = camera_create_perspective(glm::radians(60.0f), (f32)WINDOW_WIDTH / (f32)WINDOW_HEIGHT, 0.01f, 100.0f);
    camera_update_proj(camera, shader);

    quad_set_scale(quad, glm::vec3(8, 8, 0));
    projectil = particle_create(glm::vec3(8, WINDOW_HEIGHT*0.5f, 0.0f),
                                glm::vec3(10.0f, 0.0f, 0.0f),
                                glm::vec3(0.0f, -1.0f, 0.0f),
                                0.99f, 1.0f);
}

void ParticleDemo::update(Input* input, f32 dt) {
    camera_update_view(camera, shader);
    particle_integrate(&projectil, dt);
}

void ParticleDemo::render() {
    quad_set_position(quad, projectil.position);
    renderer_set_color(renderer, glm::vec4(0, 0, 1, 1));
    renderer_render_quad(renderer, quad);
}

void ParticleDemo::destroy() {
    camera_destroy(camera);
    quad_destroy(quad);
    renderer_destroy(renderer);
    shader_destroy(shader);
}




