//
// Created by jacobmawby on 09/09/2026.
//

#ifndef MINECRAFT_IN_C_CHUNK_MESH_H
#define MINECRAFT_IN_C_CHUNK_MESH_H

#include <vector.h>
#include <cglm/cglm.h>

struct BlockVertex {
    vec3 pos;
    vec2 tex_coord;
    float brightness;
};

struct ChunkMesh {
    unsigned int VBO;
    unsigned int VAO;
    unsigned int EBO;
    unsigned int shader;
    int model_loc;
    int view_loc;
    int projection_loc;
    mat4 model_matrix;
    struct Vector vertices;
    struct Vector triangles;
};

void init_chunk_mesh(struct ChunkMesh* this, unsigned int shader_program, vec3 pos);
void update_chunk_mesh(struct ChunkMesh* this);
void draw_chunk_mesh(const struct ChunkMesh* this, mat4* view_matrix, mat4* projection_matrix);
void chunk_mesh_deconstructor(struct ChunkMesh* this);
void clear_mesh_data(struct ChunkMesh* this);

#endif //MINECRAFT_IN_C_CHUNK_MESH_H
