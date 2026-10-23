//
// Created by jacobmawby on 11/09/2026.
//

#ifndef MINECRAFT_IN_C_CONSTANTS_H
#define MINECRAFT_IN_C_CONSTANTS_H

#define CHUNK_SIZE_X 16
#define CHUNK_SIZE_Y 256
#include <cglm/cglm.h>

constexpr int SCR_WIDTH = 1920;
constexpr int SCR_HEIGHT = 1080;
constexpr float aspect_ratio = (float)SCR_WIDTH / SCR_HEIGHT;

extern ivec3 directions[6];
extern ivec2 neighbour_chunk_dirs[4];

#endif //MINECRAFT_IN_C_CONSTANTS_H
