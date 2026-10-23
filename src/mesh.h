//
// Created by jacobmawby on 16/09/2026.
//

#ifndef MINECRAFT_IN_C_MESH_H
#define MINECRAFT_IN_C_MESH_H

#include <cglm/cglm.h>
#include <vector.h>

struct Vertex {
    vec3 pos;
};

struct Mesh {
    unsigned int VBO;
    unsigned int VAO;
    unsigned int EBO;
    unsigned int shader;
    int model_loc;
    int view_loc;
    int projection_loc;
    vec3 pos;
    mat4 model_matrix;
    struct Vector vertices;
    struct Vector triangles;
};

void init_mesh(struct Mesh* this, unsigned int shader_program, vec3 pos);
void update_mesh(struct Mesh* this);
void draw_mesh(const struct Mesh* this, mat4* view_matrix, mat4* projection_matrix);
void mesh_deconstructor(struct Mesh* this);
void move_mesh(struct Mesh* this, vec3 pos);

static void make_selector_cube(struct Mesh* this, float size, float x, float y, float z) {
    int offset = this->vertices.size;

    append_vector_impl(&this->vertices, &(struct Vertex){.pos = {x - size,     y + size, z - size}});
    append_vector_impl(&this->vertices, &(struct Vertex){.pos = {x - size,     y + size, z + size}});
    append_vector_impl(&this->vertices, &(struct Vertex){.pos = {x + size, y + size, z - size}});
    append_vector_impl(&this->vertices, &(struct Vertex){.pos = {x + size, y + size, z + size}});

    append_vector_lit(&this->triangles, 0 + offset);
    append_vector_lit(&this->triangles, 1 + offset);
    append_vector_lit(&this->triangles, 2 + offset);

    append_vector_lit(&this->triangles, 1 + offset);
    append_vector_lit(&this->triangles, 3 + offset);
    append_vector_lit(&this->triangles, 2 + offset);

    offset = this->vertices.size;

    append_vector_impl(&this->vertices, &(struct Vertex){.pos = {x - size,     y - size, z - size}});
    append_vector_impl(&this->vertices, &(struct Vertex){.pos = {x - size,     y - size, z + size}});
    append_vector_impl(&this->vertices, &(struct Vertex){.pos = {x + size, y - size, z - size}});
    append_vector_impl(&this->vertices, &(struct Vertex){.pos = {x + size, y - size, z + size}});

    append_vector_lit(&this->triangles, 0 + offset);
    append_vector_lit(&this->triangles, 2 + offset);
    append_vector_lit(&this->triangles, 1 + offset);

    append_vector_lit(&this->triangles, 1 + offset);
    append_vector_lit(&this->triangles, 2 + offset);
    append_vector_lit(&this->triangles, 3 + offset);

    offset = this->vertices.size;

    append_vector_impl(&this->vertices, &(struct Vertex){.pos = {x + size, y - size,     z - size}});
    append_vector_impl(&this->vertices, &(struct Vertex){.pos = {x + size, y + size, z - size}});
    append_vector_impl(&this->vertices, &(struct Vertex){.pos = {x + size, y - size,     z + size}});
    append_vector_impl(&this->vertices, &(struct Vertex){.pos = {x + size, y + size, z + size}});

    append_vector_lit(&this->triangles, 0 + offset);
    append_vector_lit(&this->triangles, 1 + offset);
    append_vector_lit(&this->triangles, 2 + offset);

    append_vector_lit(&this->triangles, 1 + offset);
    append_vector_lit(&this->triangles, 3 + offset);
    append_vector_lit(&this->triangles, 2 + offset);

    offset = this->vertices.size;

    append_vector_impl(&this->vertices, &(struct Vertex){.pos = {x - size, y - size,     z - size}});
    append_vector_impl(&this->vertices, &(struct Vertex){.pos = {x - size, y + size, z - size}});
    append_vector_impl(&this->vertices, &(struct Vertex){.pos = {x - size, y - size,     z + size}});
    append_vector_impl(&this->vertices, &(struct Vertex){.pos = {x - size, y + size, z + size}});

    append_vector_lit(&this->triangles, 0 + offset);
    append_vector_lit(&this->triangles, 2 + offset);
    append_vector_lit(&this->triangles, 1 + offset);

    append_vector_lit(&this->triangles, 1 + offset);
    append_vector_lit(&this->triangles, 2 + offset);
    append_vector_lit(&this->triangles, 3 + offset);

    offset = this->vertices.size;

    append_vector_impl(&this->vertices, &(struct Vertex){.pos = {x - size,     y - size,     z + size}});
    append_vector_impl(&this->vertices, &(struct Vertex){.pos = {x - size,     y + size, z + size}});
    append_vector_impl(&this->vertices, &(struct Vertex){.pos = {x + size, y - size,     z + size}});
    append_vector_impl(&this->vertices, &(struct Vertex){.pos = {x + size, y + size, z + size}});

    append_vector_lit(&this->triangles, 0 + offset);
    append_vector_lit(&this->triangles, 2 + offset);
    append_vector_lit(&this->triangles, 1 + offset);

    append_vector_lit(&this->triangles, 1 + offset);
    append_vector_lit(&this->triangles, 2 + offset);
    append_vector_lit(&this->triangles, 3 + offset);

    offset = this->vertices.size;

    append_vector_impl(&this->vertices, &(struct Vertex){.pos = {x - size,     y - size,     z - size}});
    append_vector_impl(&this->vertices, &(struct Vertex){.pos = {x - size,     y + size, z - size}});
    append_vector_impl(&this->vertices, &(struct Vertex){.pos = {x + size, y - size,     z - size}});
    append_vector_impl(&this->vertices, &(struct Vertex){.pos = {x + size, y + size, z - size}});

    append_vector_lit(&this->triangles, 0 + offset);
    append_vector_lit(&this->triangles, 1 + offset);
    append_vector_lit(&this->triangles, 2 + offset);

    append_vector_lit(&this->triangles, 1 + offset);
    append_vector_lit(&this->triangles, 3 + offset);
    append_vector_lit(&this->triangles, 2 + offset);
}



#endif //MINECRAFT_IN_C_MESH_H
