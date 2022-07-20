#pragma once

#include "defines.h"
#include <glm/glm.hpp>

struct Shader {
    u32 id;
};

Shader* shader_create(const char* vertex_filepath, const char* fragment_filepath);
void shader_destroy(Shader* shader);
void shader_bind(Shader* shader);
void shader_unbind();

void shader_update_V3(Shader* shader, const char* var_name, glm::vec3 vector);
void shader_update_V4(Shader* shader, const char* var_name, glm::vec4 vector);
void shader_update_M4(Shader* shader, const char* var_name, glm::mat4 matrix);
void shader_update_int(Shader* shader, const char* var_name, i32 value);
void shader_update_int_array(Shader* shader, const char* var_name, i32 size, i32* array);

struct Quad {
    u32 vao;
    u32 vbo;
    glm::vec3 position;
    glm::vec3 scale;
    glm::mat4 model;
};

Quad *quad_create();
void quad_set_position(Quad* quad, glm::vec3 position);
void quad_set_scale(Quad* quad, glm::vec3 scale);
void quad_update_quad_model(Quad* quad);
void quad_destroy(Quad* quad);

struct Vertex {
    glm::vec3 position;
};

struct Renderer {
    Shader* shader;
};

Renderer* renderer_create(Shader* shader);
void renderer_destroy(Renderer* renderer);
void renderer_set_shader(Renderer *renderer, Shader* shader);
void renderer_set_color(Renderer* renderer, glm::vec4 color);
void renderer_render_quad(Renderer *renderer, Quad* quad);