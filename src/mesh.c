//
// Created by jacobmawby on 16/09/2026.
//

#include "mesh.h"
#include <glad.h>

void init_mesh(struct Mesh* this, unsigned int shader_program, vec3 pos) {
    init_vector(&this->triangles, sizeof(int));
    init_vector(&this->vertices, sizeof(struct Vertex));

    glGenBuffers(1, &this->VBO);
    glGenBuffers(1, &this->EBO);
    glGenVertexArrays(1, &this->VAO);

    this->shader = shader_program;
    glm_mat4_identity(this->model_matrix);
    glm_translate(this->model_matrix, pos);
    glm_vec3_copy(pos, this->pos);

    this->model_loc = glGetUniformLocation(shader_program, "model");
    this->view_loc = glGetUniformLocation(shader_program, "view");
    this->projection_loc = glGetUniformLocation(shader_program, "projection");
}

void move_mesh(struct Mesh* this, vec3 pos) {
    this->pos[0] = pos[0];
    this->pos[1] = pos[1];
    this->pos[2] = pos[2];

    glm_mat4_identity(this->model_matrix);
    glm_translate(this->model_matrix, pos);
}

void update_mesh(struct Mesh* this) {
    glBindVertexArray(this->VAO);

    glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
    glBufferData(GL_ARRAY_BUFFER, get_vector_bytes(&this->vertices), this->vertices.data, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, get_vector_bytes(&this->triangles), this->triangles.data, GL_STATIC_DRAW);

    int stride = sizeof(struct Vertex);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, nullptr);

    glEnableVertexAttribArray(0);

    glBindVertexArray(0);
}

void draw_mesh(const struct Mesh* this, mat4* view_matrix, mat4* projection_matrix) {
    glUseProgram(this->shader);
    glBindVertexArray(this->VAO);

    glUniformMatrix4fv(this->model_loc, 1, GL_FALSE, this->model_matrix[0]);
    glUniformMatrix4fv(this->view_loc, 1, GL_FALSE, *view_matrix[0]);
    glUniformMatrix4fv(this->projection_loc, 1, GL_FALSE, *projection_matrix[0]);


    glDrawElements(GL_TRIANGLES, this->triangles.size, GL_UNSIGNED_INT, nullptr);
    glBindVertexArray(0);

}

void mesh_deconstructor(struct Mesh* this) {
    free_vector(&this->vertices);
    free_vector(&this->triangles);
}