#include "renderer.h"
#include "input.h"

#include <glad/glad.h>
#include <malloc.h>
#include <stdio.h>
#include <glm/gtc/matrix_transform.hpp>


Shader* shader_create(const char* vertex_filepath, const char* fragment_filepath) {
    Shader* shader = (Shader *)malloc(sizeof(Shader));

    FileResult vertex_result = read_file(vertex_filepath);
    FileResult fragment_result = read_file(fragment_filepath);
    const char* vertex_src = (const char*)vertex_result.data;
    const char* fragment_src = (const char*)fragment_result.data;
    i32 succes;
    char info_log[512];

    i32 vertex_id = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex_id, 1, &vertex_src, 0);
    glCompileShader(vertex_id);
    glGetShaderiv(vertex_id, GL_COMPILE_STATUS, &succes);
    if (!succes) {
        printf("Error Compiling: %s\n", vertex_filepath);
        glGetShaderInfoLog(vertex_id, 512, 0, info_log);
        printf("%s\n", info_log);
        Assert(!"Error Vertex Shader");
    }

    i32 fragment_id = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment_id, 1, &fragment_src, 0);
    glCompileShader(fragment_id);
    glGetShaderiv(fragment_id, GL_COMPILE_STATUS, &succes);
    if (!succes) {
        printf("Error Compiling: %s\n", fragment_filepath);
        glGetShaderInfoLog(fragment_id, 512, 0, info_log);
        printf("%s\n", info_log);
        Assert(!"Error Fragment Shader");
    }

    shader->id = glCreateProgram();
    glAttachShader(shader->id, vertex_id);
    glAttachShader(shader->id, fragment_id);
    glLinkProgram(shader->id);
    glGetProgramiv(shader->id, GL_LINK_STATUS, &succes);
    if (!succes) {
        printf("Error Linking Shader Program\n");
        glGetProgramInfoLog(shader->id, 512, 0, info_log);
        printf("%s\n", info_log);
    }

    glDeleteShader(vertex_id);
    glDeleteShader(fragment_id);

    free(vertex_result.data);
    free(fragment_result.data);

    return shader;
}


void shader_destroy(Shader* shader) {
    glDeleteProgram(shader->id);
    free(shader);
}

void shader_bind(Shader* shader) {
    glUseProgram(shader->id);
}

void shader_unbind() {
    glUseProgram(0);
}

void shader_update_V3(Shader* shader, const char* var_name, glm::vec3 vector) {
    i32 var_loc = glGetUniformLocation(shader->id, var_name);
    shader_bind(shader);
    glUniform3fv(var_loc, 1, &vector[0]);
}

void shader_update_V4(Shader* shader, const char* var_name, glm::vec4 vector) {
    i32 var_loc = glGetUniformLocation(shader->id, var_name);
    shader_bind(shader);
    glUniform4fv(var_loc, 1, &vector[0]);
}

void shader_update_M4(Shader* shader, const char* var_name, glm::mat4 matrix) {
    i32 var_loc = glGetUniformLocation(shader->id, var_name);
    shader_bind(shader);
    glUniformMatrix4fv(var_loc, 1, false, &matrix[0][0]);
}

void shader_update_int(Shader* shader, const char* var_name, i32 value) {
    i32 var_loc = glGetUniformLocation(shader->id, var_name);
    shader_bind(shader);
    glUniform1i(var_loc, value);
}

void shader_update_int_array(Shader* shader, const char* var_name, i32 size, i32* array) {
    i32 var_loc = glGetUniformLocation(shader->id, var_name);
    shader_bind(shader);
    glUniform1iv(var_loc, size, array);
}

Quad *quad_create()
{
    Vertex vertices[6] = {
        {{-0.5f, -0.5f, 0.0f}},
        {{ 0.5f, -0.5f, 0.0f}},
        {{ 0.5f,  0.5f, 0.0f}},
        {{ 0.5f,  0.5f, 0.0f}},
        {{-0.5f,  0.5f, 0.0f}},
        {{-0.5f, -0.5f, 0.0f}}
    };
    Quad *quad = (Quad *)malloc(sizeof(Quad));
    glGenVertexArrays(1, &quad->vao);
    glBindVertexArray(quad->vao);
    glGenBuffers(1, &quad->vbo);
    glBindBuffer(GL_ARRAY_BUFFER, quad->vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex)*6, vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)0);
    glEnableVertexAttribArray(0);
    return quad;
}

void quad_set_position(Quad* quad, glm::vec3 position)
{
    quad->position = position;
    quad_update_model(quad);
}

void quad_set_scale(Quad* quad, glm::vec3 scale)
{
    quad->scale = scale;
    quad_update_model(quad);
}

void quad_update_model(Quad* quad) {
    glm::mat4 trans = glm::translate(glm::mat4(1.0f), quad->position);
    glm::mat4 scale = glm::scale(glm::mat4(1.0f), quad->scale);
    quad->model = trans * scale;
}

void quad_destroy(Quad* quad) {
    glDeleteVertexArrays(1, &quad->vao);
    glDeleteBuffers(1, &quad->vbo);
    free(quad);
}

Renderer* renderer_create(Shader* shader)
{
    Renderer* renderer = (Renderer*)malloc(sizeof(Renderer));
    renderer->shader = shader;
    shader_update_V4(renderer->shader, "uColor", glm::vec4(1, 1, 1, 1));
    return renderer;
}

void renderer_destroy(Renderer* renderer) {
    free(renderer);
}

void renderer_set_shader(Renderer* renderer, Shader* shader)
{
    renderer->shader = shader;
}

void renderer_set_color(Renderer* renderer, glm::vec4 color) {
    shader_update_V4(renderer->shader, "uColor", color);
}

void renderer_render_quad(Renderer* renderer, Quad* quad)
{
    shader_bind(renderer->shader);
    shader_update_M4(renderer->shader, "uModel", quad->model);
    glBindVertexArray(quad->vao);
    glDrawArrays(GL_TRIANGLES, 0, 6);
}
