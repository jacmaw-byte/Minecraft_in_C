//
// Created by jacobmawby on 11/09/2026.
//
#include "chunk.h"
#include "noise/value_noise.h"
#include <atlas.h>
#include <stdio.h>

#include "chunk_manager.h"
#include "chunk_map.h"


void init_chunk(struct Chunk* this, vec2 pos, unsigned int chunk_shader, unsigned int transparent_chunk_shader) {
    this->pos[0] = pos[0];
    this->pos[1] = pos[1];

    init_chunk_mesh(&this->mesh, chunk_shader, (vec3){pos[0], 0.0f, pos[1]});
    init_chunk_mesh(&this->transparent_mesh, transparent_chunk_shader, (vec3){pos[0], 0, pos[1]});
}



const int WATER_GENERATION_LEVEL = 47;
const double GRASS_DISTRIBUTION = 0.01;

static struct Vector* current_vertex_array_sorting;
static vec2 current_chunk_pos;
static vec3 cam_pos;


static int comp_faces(const void *a, const void *b)
{
    const int *x = a;
    const int *y = b;

    struct BlockVertex *vertices =
        current_vertex_array_sorting->data;

    vec3 centroid_x = {
        (vertices[x[0]].pos[0] +
         vertices[x[1]].pos[0] +
         vertices[x[2]].pos[0]) / 3.0f,

        (vertices[x[0]].pos[1] +
         vertices[x[1]].pos[1] +
         vertices[x[2]].pos[1]) / 3.0f,

        (vertices[x[0]].pos[2] +
         vertices[x[1]].pos[2] +
         vertices[x[2]].pos[2]) / 3.0f
    };

    vec3 centroid_y = {
        (vertices[y[0]].pos[0] +
         vertices[y[1]].pos[0] +
         vertices[y[2]].pos[0]) / 3.0f,

        (vertices[y[0]].pos[1] +
         vertices[y[1]].pos[1] +
         vertices[y[2]].pos[1]) / 3.0f,

        (vertices[y[0]].pos[2] +
         vertices[y[1]].pos[2] +
         vertices[y[2]].pos[2]) / 3.0f
    };

    float x_dist =
        (centroid_x[0] - cam_pos[0]) * (centroid_x[0] - cam_pos[0]) +
        (centroid_x[1] - cam_pos[1]) * (centroid_x[1] - cam_pos[1]) +
        (centroid_x[2] - cam_pos[2]) * (centroid_x[2] - cam_pos[2]);

    float y_dist =
        (centroid_y[0] - cam_pos[0]) * (centroid_y[0] - cam_pos[0]) +
        (centroid_y[1] - cam_pos[1]) * (centroid_y[1] - cam_pos[1]) +
        (centroid_y[2] - cam_pos[2]) * (centroid_y[2] - cam_pos[2]);

    if (x_dist < y_dist)
        return 1;   // farther first

    if (x_dist > y_dist)
        return -1;

    return 0;
}

void sort_transparent_faces_in_chunk(struct Chunk* this) {
    current_vertex_array_sorting = &this->mesh.vertices;
    current_chunk_pos[0] = this->pos[0];
    current_chunk_pos[1] = this->pos[1];

    cam_pos[0] = *this->manager->camera_pos[0];
    cam_pos[1] = *this->manager->camera_pos[1];
    cam_pos[2] = *this->manager->camera_pos[2];

    qsort(this->transparent_mesh.triangles.data, this->transparent_mesh.triangles.size / 6, 6 * sizeof(int), comp_faces);

    update_chunk_mesh(&this->transparent_mesh);
}

void generate_chunk_data(struct Chunk* this, ivec2 world_pos) {

    this->generated_data = true;

    for (int z = 0; z < CHUNK_SIZE_X; ++z) {
        for (int x = 0; x < CHUNK_SIZE_X; ++x) {
            constexpr float scale = 0.005f;
            ivec2 new_pos = {x + world_pos[0], z + world_pos[1]};

            float noise_value = value_noise_FBM((float)new_pos[0], (float)new_pos[1], scale, 2.3f, 0.25f, 4);
            const int height = (int)roundf(noise_value * noise_value * 90.0f);

            for (int y = 0; y < CHUNK_SIZE_Y; ++y) {
                int index = chunk_index(x, y, z);

                if (y == height) {
                    this->voxel_data[index] = GRASS;
                }
                else if (y < height) {
                    this->voxel_data[index] = DIRT;
                }
                else if (y < WATER_GENERATION_LEVEL) {
                    this->voxel_data[index] = WATER;
                }
                else {
                    this->voxel_data[index] = AIR;
                }
            }
            int index = chunk_index(x, height + 1, z);

            if (drand48() < GRASS_DISTRIBUTION) {
                if (this->voxel_data[index] == AIR)
                    this->voxel_data[index] = GRASS_2D;
            }
        }
    }
}

static int get_texid(Block block, int axis) {
    switch (axis) {
        case 0:
            return block_attribute_db[block].top_tex_id;
            break;
        case 1:
            return block_attribute_db[block].bottom_tex_id;
            break;
        default:
            return block_attribute_db[block].side_tex_id;
    }
}

static void emit_quad(int axis, float x, float y, float z, Block block, struct Vector* vertices, struct Vector* triangles) {
    int offset = vertices->size;
    struct TexCoords uv = get_atlas_tex_pos(get_texid(block, axis));
    float brightness;

    if (block == WATER) y -= 0.125f;

    switch (axis) {
        // Y+
        case 0:
            brightness = 1.0f;

            append_vector_impl(vertices, &(struct BlockVertex){.pos = {x,     y + 1, z},     .tex_coord = {uv.bl[0], uv.bl[1]}, brightness});
            append_vector_impl(vertices, &(struct BlockVertex){.pos = {x,     y + 1, z + 1}, .tex_coord = {uv.tl[0], uv.tl[1]}, brightness});
            append_vector_impl(vertices, &(struct BlockVertex){.pos = {x + 1, y + 1, z},     .tex_coord = {uv.br[0], uv.br[1]}, brightness});
            append_vector_impl(vertices, &(struct BlockVertex){.pos = {x + 1, y + 1, z + 1}, .tex_coord = {uv.tr[0], uv.tr[1]}, brightness});

            append_vector_lit(triangles, 0 + offset);
            append_vector_lit(triangles, 1 + offset);
            append_vector_lit(triangles, 2 + offset);

            append_vector_lit(triangles, 1 + offset);
            append_vector_lit(triangles, 3 + offset);
            append_vector_lit(triangles, 2 + offset);
            break;
        // Y-
        case 1:
            brightness = 0.6f;

            append_vector_impl(vertices, &(struct BlockVertex){.pos = {x,     y, z},     .tex_coord = {uv.bl[0], uv.bl[1]}, brightness});
            append_vector_impl(vertices, &(struct BlockVertex){.pos = {x,     y, z + 1}, .tex_coord = {uv.tl[0], uv.tl[1]}, brightness});
            append_vector_impl(vertices, &(struct BlockVertex){.pos = {x + 1, y, z},     .tex_coord = {uv.br[0], uv.br[1]}, brightness});
            append_vector_impl(vertices, &(struct BlockVertex){.pos = {x + 1, y, z + 1}, .tex_coord = {uv.tr[0], uv.tr[1]}, brightness});

            append_vector_lit(triangles, 0 + offset);
            append_vector_lit(triangles, 2 + offset);
            append_vector_lit(triangles, 1 + offset);

            append_vector_lit(triangles, 1 + offset);
            append_vector_lit(triangles, 2 + offset);
            append_vector_lit(triangles, 3 + offset);
            break;
        // X+
        case 2:
            brightness = 0.90f;
            append_vector_impl(vertices, &(struct BlockVertex){.pos = {x + 1, y,     z},     .tex_coord = {uv.bl[0], uv.bl[1]}, brightness});
            append_vector_impl(vertices, &(struct BlockVertex){.pos = {x + 1, y + 1, z},     .tex_coord = {uv.tl[0], uv.tl[1]}, brightness});
            append_vector_impl(vertices, &(struct BlockVertex){.pos = {x + 1, y,     z + 1}, .tex_coord = {uv.br[0], uv.br[1]}, brightness});
            append_vector_impl(vertices, &(struct BlockVertex){.pos = {x + 1, y + 1, z + 1}, .tex_coord = {uv.tr[0], uv.tr[1]}, brightness});

            append_vector_lit(triangles, 0 + offset);
            append_vector_lit(triangles, 1 + offset);
            append_vector_lit(triangles, 2 + offset);

            append_vector_lit(triangles, 1 + offset);
            append_vector_lit(triangles, 3 + offset);
            append_vector_lit(triangles, 2 + offset);
            break;
        // X-
        case 3:
            brightness = 0.85f;
            append_vector_impl(vertices, &(struct BlockVertex){.pos = {x, y,     z},     .tex_coord = {uv.bl[0], uv.bl[1]}, brightness});
            append_vector_impl(vertices, &(struct BlockVertex){.pos = {x, y + 1, z},     .tex_coord = {uv.tl[0], uv.tl[1]}, brightness});
            append_vector_impl(vertices, &(struct BlockVertex){.pos = {x, y,     z + 1}, .tex_coord = {uv.br[0], uv.br[1]}, brightness});
            append_vector_impl(vertices, &(struct BlockVertex){.pos = {x, y + 1, z + 1}, .tex_coord = {uv.tr[0], uv.tr[1]}, brightness});

            append_vector_lit(triangles, 0 + offset);
            append_vector_lit(triangles, 2 + offset);
            append_vector_lit(triangles, 1 + offset);

            append_vector_lit(triangles, 1 + offset);
            append_vector_lit(triangles, 2 + offset);
            append_vector_lit(triangles, 3 + offset);
            break;
        // Z+
        case 4:
            brightness = 0.85f;
            append_vector_impl(vertices, &(struct BlockVertex){.pos = {x,     y,     z + 1}, .tex_coord = {uv.bl[0], uv.bl[1]}, brightness});
            append_vector_impl(vertices, &(struct BlockVertex){.pos = {x,     y + 1, z + 1}, .tex_coord = {uv.tl[0], uv.tl[1]}, brightness});
            append_vector_impl(vertices, &(struct BlockVertex){.pos = {x + 1, y,     z + 1}, .tex_coord = {uv.br[0], uv.br[1]}, brightness});
            append_vector_impl(vertices, &(struct BlockVertex){.pos = {x + 1, y + 1, z + 1}, .tex_coord = {uv.tr[0], uv.tr[1]}, brightness});

            append_vector_lit(triangles, 0 + offset);
            append_vector_lit(triangles, 2 + offset);
            append_vector_lit(triangles, 1 + offset);

            append_vector_lit(triangles, 1 + offset);
            append_vector_lit(triangles, 2 + offset);
            append_vector_lit(triangles, 3 + offset);
            break;
        // Z-
        case 5:
            brightness = 0.85f;
            append_vector_impl(vertices, &(struct BlockVertex){.pos = {x,     y,     z}, .tex_coord = {uv.bl[0], uv.bl[1]}, brightness});
            append_vector_impl(vertices, &(struct BlockVertex){.pos = {x,     y + 1, z}, .tex_coord = {uv.tl[0], uv.tl[1]}, brightness});
            append_vector_impl(vertices, &(struct BlockVertex){.pos = {x + 1, y,     z}, .tex_coord = {uv.br[0], uv.br[1]}, brightness});
            append_vector_impl(vertices, &(struct BlockVertex){.pos = {x + 1, y + 1, z}, .tex_coord = {uv.tr[0], uv.tr[1]}, brightness});

            append_vector_lit(triangles, 0 + offset);
            append_vector_lit(triangles, 1 + offset);
            append_vector_lit(triangles, 2 + offset);

            append_vector_lit(triangles, 1 + offset);
            append_vector_lit(triangles, 3 + offset);
            append_vector_lit(triangles, 2 + offset);
            break;
    }


}

static void emit_billboard(
    float x, float y, float z,
    Block block,
    struct Vector* vertices,
    struct Vector* triangles
) {
    struct TexCoords uv =
        get_atlas_tex_pos(block_attribute_db[block].top_tex_id);

    float brightness = 0.7f;

    int offset = vertices->size;

    append_vector_impl(vertices, &(struct BlockVertex){
        .pos = {x, y, z},
        .tex_coord = {uv.bl[0], uv.bl[1]},
        .brightness = brightness
    });

    append_vector_impl(vertices, &(struct BlockVertex){
        .pos = {x, y + 1, z},
        .tex_coord = {uv.tl[0], uv.tl[1]},
        .brightness = brightness
    });

    append_vector_impl(vertices, &(struct BlockVertex){
        .pos = {x + 1, y, z + 1},
        .tex_coord = {uv.br[0], uv.br[1]},
        .brightness = brightness
    });

    append_vector_impl(vertices, &(struct BlockVertex){
        .pos = {x + 1, y + 1, z + 1},
        .tex_coord = {uv.tr[0], uv.tr[1]},
        .brightness = brightness
    });

    // Front
    append_vector_lit(triangles, offset + 0);
    append_vector_lit(triangles, offset + 1);
    append_vector_lit(triangles, offset + 2);

    append_vector_lit(triangles, offset + 1);
    append_vector_lit(triangles, offset + 3);
    append_vector_lit(triangles, offset + 2);

    // Back
    append_vector_lit(triangles, offset + 0);
    append_vector_lit(triangles, offset + 2);
    append_vector_lit(triangles, offset + 1);

    append_vector_lit(triangles, offset + 1);
    append_vector_lit(triangles, offset + 2);
    append_vector_lit(triangles, offset + 3);

    offset = vertices->size;

    append_vector_impl(vertices, &(struct BlockVertex){
        .pos = {x + 1, y, z},
        .tex_coord = {uv.bl[0], uv.bl[1]},
        .brightness = brightness
    });

    append_vector_impl(vertices, &(struct BlockVertex){
        .pos = {x + 1, y + 1, z},
        .tex_coord = {uv.tl[0], uv.tl[1]},
        .brightness = brightness
    });

    append_vector_impl(vertices, &(struct BlockVertex){
        .pos = {x, y, z + 1},
        .tex_coord = {uv.br[0], uv.br[1]},
        .brightness = brightness
    });

    append_vector_impl(vertices, &(struct BlockVertex){
        .pos = {x, y + 1, z + 1},
        .tex_coord = {uv.tr[0], uv.tr[1]},
        .brightness = brightness
    });

    // Front
    append_vector_lit(triangles, offset + 0);
    append_vector_lit(triangles, offset + 1);
    append_vector_lit(triangles, offset + 2);

    append_vector_lit(triangles, offset + 1);
    append_vector_lit(triangles, offset + 3);
    append_vector_lit(triangles, offset + 2);

    // Back
    append_vector_lit(triangles, offset + 0);
    append_vector_lit(triangles, offset + 2);
    append_vector_lit(triangles, offset + 1);

    append_vector_lit(triangles, offset + 1);
    append_vector_lit(triangles, offset + 2);
    append_vector_lit(triangles, offset + 3);
}

static void process_voxel_at(struct Chunk* this, int x, int y, int z) {
    ivec3 voxel_pos = {x, y, z};

    int voxel_index = chunk_index(voxel_pos[0], voxel_pos[1], voxel_pos[2]);
    Block block = this->voxel_data[voxel_index];
    struct BlockData* block_data = &block_attribute_db[block];

    if (block == AIR) {
        return;
    }

    if (!block_data->bill_boarded){

        for (int axis = 0; axis < 6; ++axis) {

            ivec3 neighbour_pos;
            glm_ivec3_add(voxel_pos, directions[axis], neighbour_pos);

            Block neighbour_block = AIR;

            // Sort out X+ case
            if (neighbour_pos[0] >= CHUNK_SIZE_X) {
                // Get the neighbour chunk
                ivec2 neighbour_chunk_pos = {this->chunk_pos[0] + 1, this->chunk_pos[1]};
                const struct Chunk* neighbour_chunk = get_from_chunk_map(&this->manager->chunk_map, neighbour_chunk_pos);

                const int index = chunk_index(0, y, z);

                neighbour_block = neighbour_chunk->voxel_data[index];
            }
            // Sort out X- case
            else if (neighbour_pos[0] < 0) {
                // Get the neighbour chunk
                ivec2 neighbour_chunk_pos = {this->chunk_pos[0] - 1, this->chunk_pos[1]};
                const struct Chunk* neighbour_chunk = get_from_chunk_map(&this->manager->chunk_map, neighbour_chunk_pos);

                const int index = chunk_index(CHUNK_SIZE_X - 1, y, z);

                neighbour_block = neighbour_chunk->voxel_data[index];
            }
            // Sort out the Z+ case
            else if (neighbour_pos[2] >= CHUNK_SIZE_X) {
                // Get the neighbour chunk
                ivec2 neighbour_chunk_pos = {this->chunk_pos[0], this->chunk_pos[1] + 1};
                const struct Chunk* neighbour_chunk = get_from_chunk_map(&this->manager->chunk_map, neighbour_chunk_pos);

                const int index = chunk_index(x, y, 0);

                neighbour_block = neighbour_chunk->voxel_data[index];
            }
            // Sort out the Z- case
            else if (neighbour_pos[2] < 0) {
                // Get the neighbour chunk
                ivec2 neighbour_chunk_pos = {this->chunk_pos[0], this->chunk_pos[1] - 1};
                const struct Chunk* neighbour_chunk = get_from_chunk_map(&this->manager->chunk_map, neighbour_chunk_pos);

                const int index = chunk_index(x, y, CHUNK_SIZE_X - 1);

                neighbour_block = neighbour_chunk->voxel_data[index];
            }
            else if (neighbour_pos[1] < 0) {
                continue;
            }
            else if (neighbour_pos[1] <= CHUNK_SIZE_Y) {
                int checking_index = chunk_index(neighbour_pos[0], neighbour_pos[1], neighbour_pos[2]);
                neighbour_block = this->voxel_data[checking_index];
            }
            if (block_data->transparent) {
                // Transparent blocks only render faces to air
                if (neighbour_block == AIR) {
                    emit_quad(axis, (float)x, (float)y, (float)z, block, &this->transparent_mesh.vertices, &this->transparent_mesh.triangles);
                    this->has_transparent_mesh = true;
                }
            }
            else {
                if (block_attribute_db[neighbour_block].transparent) {
                    emit_quad(axis, (float)x, (float)y, (float)z, block, &this->mesh.vertices, &this->mesh.triangles);
                }
            }
        }
    }
    else {
        emit_billboard((float)x, (float)y, (float)z, block, &this->transparent_mesh.vertices, &this->transparent_mesh.triangles);
        this->has_transparent_mesh = true;
    }
}

void generate_chunk_mesh(struct Chunk* this) {
    this->generated_mesh = true;
    for (int z = 0; z < CHUNK_SIZE_X; ++z) {
        for (int y = 0; y < CHUNK_SIZE_Y; ++y) {
            for (int x = 0; x < CHUNK_SIZE_X; ++x) {
                process_voxel_at(this, x, y, z);
            }
        }
    }
    update_chunk_mesh(&this->transparent_mesh);
    update_chunk_mesh(&this->mesh);
}

void free_chunk(struct Chunk* this) {
    chunk_mesh_deconstructor(&this->mesh);
    chunk_mesh_deconstructor(&this->transparent_mesh);
}