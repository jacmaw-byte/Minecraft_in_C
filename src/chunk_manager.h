//
// Created by jacobmawby on 12/09/2026.
//

#ifndef MINECRAFT_IN_C_CHUNK_MANAGER_H
#define MINECRAFT_IN_C_CHUNK_MANAGER_H
#include "chunk_map.h"
#include <vector.h>
#include <cglm/cglm.h>
#include "chunk.h"


struct ChunkManager {
    struct ChunkMap chunk_map;
    int render_distance;
    vec3* camera_pos;
    unsigned int chunk_shader;
    unsigned int transparent_chunk_shader;
    struct Vector generated_offsets;
};

void generate_chunks(struct ChunkManager* this);
void init_chunk_manager(struct ChunkManager* this, int render_distance, vec3* camera_pos, unsigned int chunk_shader, unsigned int transparent_chunk_shader);
void free_chunk_manager(struct ChunkManager* this);

#endif //MINECRAFT_IN_C_CHUNK_MANAGER_H
