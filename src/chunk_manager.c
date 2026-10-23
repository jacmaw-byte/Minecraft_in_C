//
// Created by jacobmawby on 12/09/2026.
//

#include "chunk_manager.h"
#include "chunk.h"
#include <hash.h>
#define MAX_CHUNKS_A_FRAME 8

int comp(const void *a, const void *b) {

    int* vec_a = (int *)a;
    int* vec_b = (int *)b;

    float a_length = sqrtf((float)(vec_a[0] * vec_a[0]) + (float)(vec_a[1] * vec_a[1]));
    float b_length = sqrtf((float)(vec_b[0] * vec_b[0]) + (float)(vec_b[1] * vec_b[1]));

    if (a_length < b_length)
        return -1;
    if (a_length > b_length)
        return 1;
    return 0;
}

void init_chunk_manager(struct ChunkManager* this, int render_distance, vec3* camera_pos, unsigned int chunk_shader, unsigned int transparent_chunk_shader) {
    this->render_distance = render_distance;
    this->camera_pos = camera_pos;
    this->chunk_shader = chunk_shader;
    this->transparent_chunk_shader = transparent_chunk_shader;

    init_chunk_map(&this->chunk_map);

    init_vector(&this->generated_offsets, 8);

    for (int x = -this->render_distance; x < this->render_distance; ++x) {
        for (int z = -this->render_distance; z < this->render_distance; ++z) {
            if (x * x + z * z <= this->render_distance * this->render_distance) {
                append_vector_impl(&this->generated_offsets, &(ivec2){x, z});
            }
        }
    }

    qsort(this->generated_offsets.data, this->generated_offsets.size, 8, comp);
}

void unload_chunks(struct ChunkManager* this) {
    for (int i = 0; i < this->chunk_map.max_nodes; ++i) {
        struct ChunkNode* node = this->chunk_map.data[i];

        while (node != nullptr) {
            struct ChunkNode* next = node->next;
            if (node->filled) {
                ivec2 pos = {node->pos[0], node->pos[1]};
                vec2 cam_pos_floored = {floorf((*this->camera_pos)[0] / CHUNK_SIZE_X), floorf((*this->camera_pos)[2] / CHUNK_SIZE_X)};
                ivec2 cam_pos_floored_i = {(int)cam_pos_floored[0], (int)cam_pos_floored[1]};

                ivec2 new_pos;
                glm_ivec2_sub(pos, cam_pos_floored_i, new_pos);

                if (new_pos[0] * new_pos[0] + new_pos[1] * new_pos[1] > this->render_distance * this->render_distance) {
                    unload_chunk(this, pos);
                }
            }

            node = next;
        }
    }
}

void generate_chunks(struct ChunkManager* this) {
    unload_chunks(this);

    vec2 cam_pos_floored = {floorf((*this->camera_pos)[0] / CHUNK_SIZE_X), floorf((*this->camera_pos)[2] / CHUNK_SIZE_X)};
    ivec2 cam_pos_floored_i = {(int)cam_pos_floored[0], (int)cam_pos_floored[1]};

    int chunks_this_frame = 0;

    for (int i = 0; i < this->generated_offsets.size; ++i) {
        int x = ((ivec2 *)this->generated_offsets.data)[i][0];
        int z = ((ivec2 *)this->generated_offsets.data)[i][1];

        if (chunks_this_frame >= MAX_CHUNKS_A_FRAME) return;

        ivec2 chunk_pos;
        glm_ivec2_add(cam_pos_floored_i, (ivec2){x, z}, chunk_pos);

        struct Chunk* chunk_to_check = get_from_chunk_map(&this->chunk_map, chunk_pos);

        if (chunk_to_check == nullptr) {
            ivec2 chunk_world_pos = {chunk_pos[0] * CHUNK_SIZE_X, chunk_pos[1] * CHUNK_SIZE_X};

            struct Chunk chunk;
            init_chunk(&chunk, (vec2){(float)chunk_world_pos[0], (float)chunk_world_pos[1]}, this->chunk_shader, this->transparent_chunk_shader);

            chunk.manager = this;

            glm_ivec2_copy(chunk_pos, chunk.chunk_pos);

            generate_chunk_data(&chunk, chunk_world_pos);

            bool neighbours_generated = true;
            for (int i = 0; i < 4; ++i) {
                ivec2 neighbour_pos;
                glm_ivec2_add(chunk.chunk_pos, neighbour_chunk_dirs[i], neighbour_pos);

                // Only generate meshes that are surrounded by chunks with data generated
                if (!is_in_chunk_map(&this->chunk_map, neighbour_pos)) {
                    neighbours_generated = false;
                    break;
                }
            }

            if (neighbours_generated) {
                generate_chunk_mesh(&chunk);
            }


            add_to_chunk_map(&this->chunk_map, chunk_pos, chunk);

            chunks_this_frame++;
        }
        else {
            if (!chunk_to_check->generated_mesh) {
                bool neighbours_generated = true;

                for (int i = 0; i < 4; ++i) {
                    ivec2 neighbour_pos;
                    glm_ivec2_add(chunk_to_check->chunk_pos, neighbour_chunk_dirs[i], neighbour_pos);

                    // Only generate meshes that are surrounded by chunks with data generated
                    if (!is_in_chunk_map(&this->chunk_map, neighbour_pos)) {
                        neighbours_generated = false;
                        break;
                    }
                }
                if (neighbours_generated) {
                    generate_chunk_mesh(chunk_to_check);
                }
            }
        }
    }
}





void unload_chunk(struct ChunkManager* this, ivec2 pos) {

    uint64_t hash = hash_64_impl(pos, 8);
    size_t index = hash & this->chunk_map.max_nodes - 1;

    struct ChunkNode* current = this->chunk_map.data[index];
    struct ChunkNode* previous = nullptr;

    while (current != nullptr) {
        if (current->filled &&
            current->hash == hash &&
            current->pos[0] == pos[0] &&
            current->pos[1] == pos[1]) {

            free_chunk(&current->chunk);

            if (previous == nullptr) {
                this->chunk_map.data[index] = current->next;
            }
            else {
                previous->next = current->next;
            }

            free(current);
            return;
        }
        previous = current;
        current = current->next;
    }
}



void free_chunk_manager(struct ChunkManager* this) {
    free_chunk_map(&this->chunk_map);
    free_vector(&this->generated_offsets);
}