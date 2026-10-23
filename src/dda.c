//
// Created by jacobmawby on 16/09/2026.
//

#include "dda.h"

#include "chunk_manager.h"
#include "chunk_map.h"
#define NULL_HIT (struct RayHit){.hit = false}


struct RayHit dda(struct ChunkManager* manager, struct Ray ray)
{
    float x = ray.origin[0];
    float y = ray.origin[1];
    float z = ray.origin[2];

    int step_x = ray.direction[0] >= 0.0f ? 1 : -1;
    int step_y = ray.direction[1] >= 0.0f ? 1 : -1;
    int step_z = ray.direction[2] >= 0.0f ? 1 : -1;

    int voxel_x = (int)floorf(x);
    int voxel_y = (int)floorf(y);
    int voxel_z = (int)floorf(z);

    float t_delta_x = fabsf(1.0f / ray.direction[0]);
    float t_delta_y = fabsf(1.0f / ray.direction[1]);
    float t_delta_z = fabsf(1.0f / ray.direction[2]);

    float next_x;
    float next_y;
    float next_z;

    if (ray.direction[0] >= 0.0f)
        next_x = ((float)voxel_x + 1.0f - x) * t_delta_x;
    else
        next_x = (x - (float)voxel_x) * t_delta_x;

    if (ray.direction[1] >= 0.0f)
        next_y = ((float)voxel_y + 1.0f - y) * t_delta_y;
    else
        next_y = (y - (float)voxel_y) * t_delta_y;

    if (ray.direction[2] >= 0.0f)
        next_z = ((float)voxel_z + 1.0f - z) * t_delta_z;
    else
        next_z = (z - (float)voxel_z) * t_delta_z;

    float t = 0.0f;

    // Normal of the face we entered the current voxel through.
    ivec3 normal = { 0, 0, 0 };

    while (t <= ray.max_dist) {

        /*
         * Convert world voxel coordinate to chunk coordinate.
         */
        int chunk_x = (int)floorf(
            (float)voxel_x / CHUNK_SIZE_X
        );

        int chunk_z = (int)floorf(
            (float)voxel_z / CHUNK_SIZE_X
        );

        int local_x = voxel_x - chunk_x * CHUNK_SIZE_X;
        int local_z = voxel_z - chunk_z * CHUNK_SIZE_X;

        if (voxel_y < 0 || voxel_y >= CHUNK_SIZE_Y)
            return NULL_HIT;

        ivec2 chunk_pos = {
            chunk_x,
            chunk_z
        };

        struct Chunk* chunk =
            get_from_chunk_map(
                &manager->chunk_map,
                chunk_pos
            );

        if (chunk == nullptr)
            return NULL_HIT;

        int index = chunk_index(
            local_x,
            voxel_y,
            local_z
        );

        if (chunk->voxel_data[index] != AIR) {
            return (struct RayHit) {
                .hit = true,
                .pos = {
                    voxel_x,
                    voxel_y,
                    voxel_z
                },
                .normal = {normal[0], normal[1], normal[2]},
                .chunk = chunk
            };
        }

        /*
         * Advance to the next voxel.
         */
        if (next_x < next_y && next_x < next_z) {
            voxel_x += step_x;
            t = next_x;
            next_x += t_delta_x;

            normal[0] = -step_x;
            normal[1] = 0;
            normal[2] = 0;
        }
        else if (next_y < next_z) {
            voxel_y += step_y;
            t = next_y;
            next_y += t_delta_y;

            normal[0] = 0;
            normal[1] = -step_y;
            normal[2] = 0;
        }
        else {
            voxel_z += step_z;
            t = next_z;
            next_z += t_delta_z;

            normal[0] = 0;
            normal[1] = 0;
            normal[2] = -step_z;
        }
    }

    return NULL_HIT;
}