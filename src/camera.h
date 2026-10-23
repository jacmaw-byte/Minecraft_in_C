//
// Created by jacobmawby on 10/09/2026.
//

#ifndef MINECRAFT_IN_C_CAMERA_H
#define MINECRAFT_IN_C_CAMERA_H
#include <cglm/cglm.h>

static vec3 up = {0.0f, 1.0f, 0.0f};

struct Camera {
    float yaw;
    float pitch;
    float speed;
    vec3 pos;
    vec3 direction;
    vec3 right;
    vec3 up;
    vec3 front;
};
void make_camera(struct Camera* this, vec3 pos, float speed);
void set_view_matrix(struct Camera* this, mat4 view);
void update_camera(struct Camera* this);

#endif //MINECRAFT_IN_C_CAMERA_H
