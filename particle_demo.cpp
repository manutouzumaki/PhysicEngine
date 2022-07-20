#include "particle_demo.h"

#include "renderer.h"
#include "camera.h"
#include "random.h"
#include "input.h"

#include <glm/gtx/compatibility.hpp>

static ParticleForceRegistry force_registry;

void ParticleDemo::start() {
    shader = shader_create("vertex.glsl", "fragment.glsl");
    renderer = renderer_create(shader);
    quad = quad_create();

    camera = camera_create_orthogonal(0, WINDOW_WIDTH, 0, WINDOW_HEIGHT, 0.01f, 100.0f);
    //camera = camera_create_perspective(glm::radians(60.0f), (f32)WINDOW_WIDTH / (f32)WINDOW_HEIGHT, 0.01f, 100.0f);
    camera_update_proj(camera, shader);

    pendulum0 = new Particle();
    pendulum0->position = glm::vec3(WINDOW_WIDTH * 0.5f, WINDOW_HEIGHT * 0.5f, 0.0f);

    pendulum1 = new Particle();
    pendulum1->position = glm::vec3(WINDOW_WIDTH * 0.5f, WINDOW_HEIGHT * 0.5f - 100.0f, 0.0f);



    mouse_pos = glm::vec3(0.0f);

    pendulum0->damping = 0.3f;
    force_registry.add(pendulum0, new ParticleAnchoredSpring(&mouse_pos, 800.0f, 1.0f));
    force_registry.add(pendulum0, new ParticleSpring(pendulum1, 400.0f, 1.0f));

    //force_registry.add(pendulum, new ParticleGravity(glm::vec3(0, -40.81, 0)));

    pendulum1->damping = 0.3f;
    force_registry.add(pendulum1, new ParticleSpring(pendulum0, 400.0f, 1.0f));

}

void ParticleDemo::update(Input* input, f32 dt) {    
    camera_update_view(camera, shader);

    mouse_pos.x = input->mouse_x;
    mouse_pos.y = WINDOW_HEIGHT - input->mouse_y;

    force_registry.update_forces(dt);
    pendulum0->integrate(dt);
    pendulum1->integrate(dt);
}

void ParticleDemo::render() {

    quad_set_scale(quad, glm::vec3(32, 32, 0));
    renderer_set_color(renderer, glm::vec4(1, 0, 0, 1));
    quad_set_position(quad, mouse_pos);
    renderer_render_quad(renderer, quad);

    renderer_set_color(renderer, glm::vec4(0, 1, 0, 1));
    quad_set_position(quad, pendulum0->position);
    renderer_render_quad(renderer, quad);

    renderer_set_color(renderer, glm::vec4(0, 0, 1, 1));
    quad_set_position(quad, pendulum1->position);
    renderer_render_quad(renderer, quad);

    quad_set_scale(quad, glm::vec3(8, 8, 0));
    for (f32 t = 0.0f; t < 1.0f; t += 0.01f) {
        renderer_set_color(renderer, glm::vec4(1, 1, 0, 1));
        glm::vec3 position = glm::lerp(pendulum0->position, mouse_pos, t);
        quad_set_position(quad, position);
        renderer_render_quad(renderer, quad);
        position = glm::lerp(pendulum1->position, pendulum0->position, t);
        quad_set_position(quad, position);
        renderer_render_quad(renderer, quad);
    }
}

void ParticleDemo::destroy() {

    delete pendulum0;
    delete pendulum1;
    force_registry.clear();

    camera_destroy(camera);
    quad_destroy(quad);
    renderer_destroy(renderer);
    shader_destroy(shader);
}



