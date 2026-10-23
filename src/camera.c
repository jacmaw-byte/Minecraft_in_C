//
// Created by jacobmawby on 10/09/2026.
//
#include "camera.h"

void make_camera(struct Camera* this, vec3 pos, float speed) {
    glm_vec3_copy(pos, this->pos);

    glm_vec3_copy((vec3){0.0f, 0.0f, -1.0f}, this->direction);

    this->yaw = -90.0f;
    this->pitch = 0.0f;

    update_camera(this);
}

void update_camera(struct Camera* this) {
    glm_cross(up, this->direction, this->right);
    glm_normalize(this->right);

    glm_cross(this->direction, this->right, this->up);

    this->direction[0] = cosf(glm_rad(this->yaw)) * cosf(glm_rad(this->pitch));
    this->direction[1] = sinf(glm_rad(this->pitch));
    this->direction[2] = sinf(glm_rad(this->yaw)) * cosf(glm_rad(this->pitch));
}

void set_view_matrix(struct Camera* this, mat4 view) {

    vec3 centre;
    glm_vec3_add(this->pos, this->direction, centre);

    glm_lookat(this->pos, centre, this->up, view);
}