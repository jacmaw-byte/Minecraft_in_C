//
// Created by jacobmawby on 13/09/2026.
//

#ifndef MINECRAFT_IN_C_CHUNK_MAP_H
#define MINECRAFT_IN_C_CHUNK_MAP_H

#include <cglm/cglm.h>
#include "chunk.h"

struct ChunkNode {
    bool filled;
    uint64_t hash;
    ivec2 pos;
    struct Chunk chunk;
    struct ChunkNode* next;
};

struct ChunkMap {
    size_t max_nodes;
    size_t size;
    struct ChunkNode** data;
};

void add_to_chunk_map(struct ChunkMap* this, ivec2 pos, struct Chunk chunk);
bool is_in_chunk_map(struct ChunkMap* this, ivec2 pos);
struct Chunk* get_from_chunk_map(struct ChunkMap* this, ivec2 pos);
void init_chunk_map(struct ChunkMap* this);
void unload_chunk(struct ChunkManager* this, ivec2 pos);
void free_chunk_map(struct ChunkMap* this);

#endif //MINECRAFT_IN_C_CHUNK_MAP_H
