//
// Created by jacobmawby on 13/09/2026.
//
#include "chunk_map.h"
#include <hash.h>

static size_t chunk_map_index(struct ChunkMap *this, uint64_t hash)
{
    return hash & this->max_nodes - 1;
}

void init_chunk_map(struct ChunkMap* this) {
    this->max_nodes = 16;
    this->size = 0;
    this->data = calloc(this->max_nodes, sizeof(struct ChunkNode*));
}

static void rehash_chunk_map(struct ChunkMap* this) {
    size_t new_capacity = this->max_nodes * 2;

    struct ChunkNode** buffer = calloc(new_capacity, sizeof(struct ChunkNode*));

    for (int i = 0; i < this->max_nodes; ++i) {
        struct ChunkNode* ptr = this->data[i];

        while (ptr != nullptr) {
            struct ChunkNode *next = ptr->next;
            uint64_t new_index = ptr->hash & new_capacity - 1;

            ptr->next = buffer[new_index];
            buffer[new_index] = ptr;

            ptr = next;
        }

    }

    free(this->data);
    this->max_nodes = new_capacity;
    this->data = buffer;
}

void add_to_chunk_map(
    struct ChunkMap *this,
    ivec2 pos,
    struct Chunk chunk
) {
    uint64_t hash = hash_64_impl(pos, 8);

    // Resize before adding if the new element would exceed 75%
    if ((float)(this->size + 1) > (float)this->max_nodes * 0.75f) {
        rehash_chunk_map(this);
    }

    struct ChunkNode *new_node = calloc(1, sizeof(struct ChunkNode));

    if (new_node == nullptr) {
        // Handle allocation failure
        return;
    }

    *new_node = (struct ChunkNode) {
        .hash = hash,
        .pos = { pos[0], pos[1] },
        .next = nullptr,
        .chunk = chunk,
        .filled = true
    };

    uint64_t index = chunk_map_index(this, hash);

    // Insert at head of bucket
    new_node->next = this->data[index];
    this->data[index] = new_node;

    this->size++;
}

struct Chunk* get_from_chunk_map(struct ChunkMap* this, ivec2 pos) {
    uint64_t hash = hash_64_impl(pos, 8);
    size_t index = chunk_map_index(this, hash);
    struct ChunkNode* node = this->data[index];

    while (node != nullptr) {
        if (node->filled &&
            node->hash == hash &&
            node->pos[0] == pos[0] &&
            node->pos[1] == pos[1]) {

            return &node->chunk;
            }

        node = node->next;
    }

    return nullptr;
}

bool is_in_chunk_map(struct ChunkMap *this, ivec2 pos)
{
    uint64_t hash = hash_64_impl(pos, 8);
    size_t index = chunk_map_index(this, hash);

    struct ChunkNode *current = this->data[index];

    while (current != nullptr) {
        if (current->hash == hash &&
            current->pos[0] == pos[0] &&
            current->pos[1] == pos[1]) {

            return true;
            }

        current = current->next;
    }

    return false;
}

void free_chunk_map(struct ChunkMap* this) {
    for (int i = 0; i < this->max_nodes; ++i) {
        struct ChunkNode* current = this->data[i];

        while (current != nullptr) {
            struct ChunkNode* next = current->next;
            if (current->filled) {



                free_chunk(&current->chunk);
                free(current);

            }
            current = next;
        }
    }
    free(this->data);

}