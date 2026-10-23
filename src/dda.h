//
// Created by jacobmawby on 16/09/2026.
//

#ifndef MINECRAFT_IN_C_DDA_H
#define MINECRAFT_IN_C_DDA_H

#include <cglm/cglm.h>
#include "chunk.h"

struct Ray {
    float max_dist;
    vec3 origin;
    vec3 direction;
};

struct RayHit {
    bool hit;
    struct Chunk* chunk;
    ivec3 pos;
    ivec3 normal;
};

struct RayHit dda(struct ChunkManager* manager, struct Ray ray);

#endif //MINECRAFT_IN_C_DDA_H
