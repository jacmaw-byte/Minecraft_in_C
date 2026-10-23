//
// Created by jacobmawby on 09/09/2026.
//
#include "chunk_mesh.h"
#include <glad.h>

void init_chunk_mesh(struct ChunkMesh* this, unsigned int shader_program, vec3 pos) {
    init_vector(&this->triangles, sizeof(int));
    init_vector(&this->vertices, sizeof(struct BlockVertex));

    glGenBuffers(1, &this->VBO);
    glGenBuffers(1, &this->EBO);
    glGenVertexArrays(1, &this->VAO);

    this->shader = shader_program;
    glm_mat4_identity(this->model_matrix);
    glm_translate(this->model_matrix, pos);

    this->model_loc = glGetUniformLocation(shader_program, "model");
    this->view_loc = glGetUniformLocation(shader_program, "view");
    this->projection_loc = glGetUniformLocation(shader_program, "projection");
}

void update_chunk_mesh(struct ChunkMesh* this) {
    glBindVertexArray(this->VAO);

    glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
    glBufferData(GL_ARRAY_BUFFER, get_vector_bytes(&this->vertices), this->vertices.data, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, get_vector_bytes(&this->triangles), this->triangles.data, GL_STATIC_DRAW);

    int stride = sizeof(struct BlockVertex);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, nullptr);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, stride, (void*)offsetof(struct BlockVertex, tex_coord));
    glVertexAttribPointer(2, 1, GL_FLOAT, GL_FALSE, stride, (void*)offsetof(struct BlockVertex, brightness));

    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);

    glBindVertexArray(0);
}

void draw_chunk_mesh(const struct ChunkMesh* this, mat4* view_matrix, mat4* projection_matrix) {
    glUseProgram(this->shader);
    glBindVertexArray(this->VAO);

    glUniformMatrix4fv(this->model_loc, 1, GL_FALSE, this->model_matrix[0]);
    glUniformMatrix4fv(this->view_loc, 1, GL_FALSE, *view_matrix[0]);
    glUniformMatrix4fv(this->projection_loc, 1, GL_FALSE, *projection_matrix[0]);


    glDrawElements(GL_TRIANGLES, this->triangles.size, GL_UNSIGNED_INT, nullptr);
    glBindVertexArray(0);

}
void clear_mesh_data(struct ChunkMesh* this) {
    free_vector(&this->triangles);
    free_vector(&this->vertices);

    init_vector(&this->triangles, sizeof(int));
    init_vector(&this->vertices, sizeof(struct BlockVertex));
}
void chunk_mesh_deconstructor(struct ChunkMesh* this) {
    free_vector(&this->vertices);
    free_vector(&this->triangles);
}