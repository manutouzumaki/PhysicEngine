#include "camera.h"

#include "renderer.h"
#include <glm/gtc/matrix_transform.hpp>


Camera* camera_create_orthogonal(f32 l, f32 r, f32 b, f32 t, f32 n, f32 f)
{
    Camera *camera = (Camera *)malloc(sizeof(Camera));
    camera->position = glm::vec3(0, 0, 5);
    camera->target = glm::vec3(0, 0, -1);
    camera->up = glm::vec3(0, 1, 0);
    camera->proj = glm::ortho(l, r, b, t, n, f);
    camera->view = glm::lookAt(camera->position, camera->target, camera->up);
    return camera;
}

Camera* camera_create_perspective(f32 fov, f32 aspect, f32 n, f32 f)
{
    Camera* camera = (Camera*)malloc(sizeof(Camera));
    camera->position = glm::vec3(0, 0, 80);
    camera->target = glm::vec3(0, 0, -1);
    camera->up = glm::vec3(0, 1, 0);
    camera->proj = glm::perspective(fov, aspect, n, f);
    camera->view = glm::lookAt(camera->position, camera->target, camera->up);
    return camera;
}

void camera_update_proj(Camera* camera, Shader* shader) {
    shader_update_M4(shader, "uProj", camera->proj);
}

void camera_update_view(Camera* camera, Shader *shader) {
    camera->view = glm::lookAt(camera->position, camera->target, camera->up);
    shader_update_M4(shader, "uView", camera->view);
}

void camera_destroy(Camera* camera)
{
    free(camera);
}
