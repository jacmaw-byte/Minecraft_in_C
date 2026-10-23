//
// Created by jacobmawby on 09/09/2026.
//

#ifndef MINECRAFT_IN_C_SHADER_H
#define MINECRAFT_IN_C_SHADER_H

#include <glad.h>
#include <file_utils.h>

static inline unsigned int get_shader(const char* path, unsigned int type) {
    // Reading the file
    const char* shader_data = read_file(path);

    if (shader_data == nullptr) {
        printf("Shader loading failed \n");
        exit(-1);
    }

    const unsigned int shader = glCreateShader(type);
    glShaderSource(shader, 1, &shader_data, nullptr);
    glCompileShader(shader);

    free(shader_data);

    return shader;
}

static inline unsigned int make_shader_program(const char* fragment_path, const char* vertex_path) {

    unsigned int shader_program = glCreateProgram();

    glAttachShader(shader_program, get_shader(fragment_path, GL_FRAGMENT_SHADER));
    glAttachShader(shader_program, get_shader(vertex_path, GL_VERTEX_SHADER));
    glLinkProgram(shader_program);

    return shader_program;
}

#endif //MINECRAFT_IN_C_SHADER_H
