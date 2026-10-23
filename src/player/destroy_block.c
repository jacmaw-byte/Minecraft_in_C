//
// Created by jacobmawby on 17/09/2026.
//
#include "player.h"
#include "../chunk_manager.h"

void destroy_block(struct RayHit* hit) {
    ivec3 voxel_local_pos = {
        hit->pos[0] - hit->chunk->chunk_pos[0] * CHUNK_SIZE_X,
        hit->pos[1],
        hit->pos[2] - hit->chunk->chunk_pos[1] * CHUNK_SIZE_X
    };

    int index = chunk_index(voxel_local_pos[0], voxel_local_pos[1], voxel_local_pos[2]);

    hit->chunk->voxel_data[index] = AIR;

    clear_mesh_data(&hit->chunk->mesh);
    generate_chunk_mesh(hit->chunk);

    if (voxel_local_pos[0] == CHUNK_SIZE_X - 1) {
        ivec2 neighbour_pos = {hit->chunk->chunk_pos[0] + 1, hit->chunk->chunk_pos[1]};

        struct Chunk* neighbour_ptr = get_from_chunk_map(&hit->chunk->manager->chunk_map, neighbour_pos);

        if (neighbour_ptr != nullptr) {
            clear_mesh_data(&neighbour_ptr->mesh);
            generate_chunk_mesh(neighbour_ptr);
        }
    }

    if (voxel_local_pos[0] == 0) {
        ivec2 neighbour_pos = {hit->chunk->chunk_pos[0] - 1, hit->chunk->chunk_pos[1]};

        struct Chunk* neighbour_ptr = get_from_chunk_map(&hit->chunk->manager->chunk_map, neighbour_pos);

        if (neighbour_ptr != nullptr) {
            clear_mesh_data(&neighbour_ptr->mesh);
            generate_chunk_mesh(neighbour_ptr);
        }
    }

    if (voxel_local_pos[2] == CHUNK_SIZE_X - 1) {
        ivec2 neighbour_pos = {hit->chunk->chunk_pos[0], hit->chunk->chunk_pos[1] + 1};

        struct Chunk* neighbour_ptr = get_from_chunk_map(&hit->chunk->manager->chunk_map, neighbour_pos);

        if (neighbour_ptr != nullptr) {
            clear_mesh_data(&neighbour_ptr->mesh);
            generate_chunk_mesh(neighbour_ptr);
        }
    }

    if (voxel_local_pos[2] == 0) {
        ivec2 neighbour_pos = {hit->chunk->chunk_pos[0], hit->chunk->chunk_pos[1] - 1};

        struct Chunk* neighbour_ptr = get_from_chunk_map(&hit->chunk->manager->chunk_map, neighbour_pos);

        if (neighbour_ptr != nullptr) {
            clear_mesh_data(&neighbour_ptr->mesh);
            generate_chunk_mesh(neighbour_ptr);
        }
    }
}