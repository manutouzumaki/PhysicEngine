#pragma once

#include <glm/glm.hpp>
#include "defines.h"

struct Shader;

struct Camera
{
    glm::vec3 up;
    glm::vec3 position;
    glm::vec3 target;

    glm::mat4 proj;
    glm::mat4 view;
};

Camera* camera_create_orthogonal(f32 l, f32 r, f32 b, f32 t, f32 n, f32 f);
Camera* camera_create_perspective(f32 fov, f32 aspect, f32 n, f32 f);
void camera_update_proj(Camera* camera, Shader* shader);
void camera_update_view(Camera *camera, Shader *shader);
void camera_destroy(Camera* camera);
