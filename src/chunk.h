//
// Created by jacobmawby on 11/09/2026.
//

#ifndef MINECRAFT_IN_C_CHUNK_H
#define MINECRAFT_IN_C_CHUNK_H
#include <constants.h>
#include "chunk_mesh.h"
#include <block.h>

struct ChunkManager;

struct Chunk {
    bool generated_data;
    bool generated_mesh;
    bool has_transparent_mesh;
    vec2 pos;
    ivec2 chunk_pos;
    struct ChunkManager* manager;
    struct ChunkMesh mesh;
    struct ChunkMesh transparent_mesh;
    Block voxel_data[CHUNK_SIZE_X * CHUNK_SIZE_X * CHUNK_SIZE_Y];
};

void init_chunk(struct Chunk* this, vec2 pos, unsigned int chunk_shader, unsigned int transparent_chunk_shader);
void generate_chunk_data(struct Chunk* this, ivec2 world_pos);
void generate_chunk_mesh(struct Chunk* this);
void sort_transparent_faces_in_chunk(struct Chunk* this);
void free_chunk(struct Chunk* this);

static int chunk_index(int x, int y, int z) {
    return x + y * CHUNK_SIZE_X + z * CHUNK_SIZE_X * CHUNK_SIZE_Y;
}

#endif //MINECRAFT_IN_C_CHUNK_H
