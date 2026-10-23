//
// Created by jacobmawby on 17/09/2026.
//
#include "player.h"
#include "../chunk_manager.h"

void place_block(struct RayHit* hit) {
    ivec3 voxel_local_pos = {
        hit->pos[0] - hit->chunk->chunk_pos[0] * CHUNK_SIZE_X,
        hit->pos[1],
        hit->pos[2] - hit->chunk->chunk_pos[1] * CHUNK_SIZE_X
    };

    ivec3 place_pos;

    glm_ivec3_add(hit->normal, voxel_local_pos, place_pos);

    int index = 0;

    if (place_pos[0] >= CHUNK_SIZE_X) {
        ivec2 neighbour_pos = {hit->chunk->chunk_pos[0] + 1, hit->chunk->chunk_pos[1]};
        struct Chunk* neighbour_ptr = get_from_chunk_map(&hit->chunk->manager->chunk_map, neighbour_pos);

        if (neighbour_ptr != nullptr) {
            hit->chunk = neighbour_ptr;

            index = chunk_index(0, place_pos[1], place_pos[2]);
        }

    }
    else if (place_pos[0] < 0){
        ivec2 neighbour_pos = {hit->chunk->chunk_pos[0] - 1, hit->chunk->chunk_pos[1]};
        struct Chunk* neighbour_ptr = get_from_chunk_map(&hit->chunk->manager->chunk_map, neighbour_pos);

        if (neighbour_ptr != nullptr) {
            hit->chunk = neighbour_ptr;

            index = chunk_index(CHUNK_SIZE_X - 1, place_pos[1], place_pos[2]);
        }
    }
    else if (place_pos[2] >= CHUNK_SIZE_X) {
        ivec2 neighbour_pos = {hit->chunk->chunk_pos[0], hit->chunk->chunk_pos[1] + 1};
        struct Chunk* neighbour_ptr = get_from_chunk_map(&hit->chunk->manager->chunk_map, neighbour_pos);

        if (neighbour_ptr != nullptr) {
            hit->chunk = neighbour_ptr;

            index = chunk_index(place_pos[0], place_pos[1], 0);
        }

    }
    else if (place_pos[2] < 0) {
        ivec2 neighbour_pos = {hit->chunk->chunk_pos[0], hit->chunk->chunk_pos[1] - 1};
        struct Chunk* neighbour_ptr = get_from_chunk_map(&hit->chunk->manager->chunk_map, neighbour_pos);

        if (neighbour_ptr != nullptr) {
            hit->chunk = neighbour_ptr;

            index = chunk_index(place_pos[0], place_pos[1], CHUNK_SIZE_X - 1);
        }
    }
    else {
        index = chunk_index(place_pos[0], place_pos[1], place_pos[2]);
    }

    hit->chunk->voxel_data[index] = GLASS;

    clear_mesh_data(&hit->chunk->mesh);
    generate_chunk_mesh(hit->chunk);

    if (place_pos[0] == CHUNK_SIZE_X - 1) {
        ivec2 neighbour_pos = {hit->chunk->chunk_pos[0] + 1, hit->chunk->chunk_pos[1]};

        struct Chunk* neighbour_ptr = get_from_chunk_map(&hit->chunk->manager->chunk_map, neighbour_pos);

        if (neighbour_ptr != nullptr) {
            clear_mesh_data(&neighbour_ptr->mesh);
            generate_chunk_mesh(neighbour_ptr);
        }
    }

    if (place_pos[0] == 0) {
        ivec2 neighbour_pos = {hit->chunk->chunk_pos[0] - 1, hit->chunk->chunk_pos[1]};

        struct Chunk* neighbour_ptr = get_from_chunk_map(&hit->chunk->manager->chunk_map, neighbour_pos);

        if (neighbour_ptr != nullptr) {
            clear_mesh_data(&neighbour_ptr->mesh);
            generate_chunk_mesh(neighbour_ptr);
        }
    }

    if (place_pos[2] == CHUNK_SIZE_X - 1) {
        ivec2 neighbour_pos = {hit->chunk->chunk_pos[0], hit->chunk->chunk_pos[1] + 1};

        struct Chunk* neighbour_ptr = get_from_chunk_map(&hit->chunk->manager->chunk_map, neighbour_pos);

        if (neighbour_ptr != nullptr) {
            clear_mesh_data(&neighbour_ptr->mesh);
            generate_chunk_mesh(neighbour_ptr);
        }
    }

    if (place_pos[2] == 0) {
        ivec2 neighbour_pos = {hit->chunk->chunk_pos[0], hit->chunk->chunk_pos[1] - 1};

        struct Chunk* neighbour_ptr = get_from_chunk_map(&hit->chunk->manager->chunk_map, neighbour_pos);

        if (neighbour_ptr != nullptr) {
            clear_mesh_data(&neighbour_ptr->mesh);
            generate_chunk_mesh(neighbour_ptr);
        }
    }
}
