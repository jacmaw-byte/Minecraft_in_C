//
// Created by jacobmawby on 20/09/2026.
//

#ifndef MINECRAFT_IN_C_OPENGL_STUFF_H
#define MINECRAFT_IN_C_OPENGL_STUFF_H

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#include <glad.h>

static void load_atlas(const char* filename) {
    unsigned int texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    stbi_set_flip_vertically_on_load(true);

    int width, height, nr_channels;
    unsigned char* atlas = stbi_load(filename, &width, &height, &nr_channels, 4);

    if (!atlas) {
        printf("Failed to load image \n");
        exit(-1);
    }

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, atlas);
    glGenerateMipmap(GL_TEXTURE_2D);

    stbi_image_free(atlas);
}

static void init_opengl_setting() {
    glEnable(GL_DEPTH_TEST);

    glEnable(GL_CULL_FACE);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

#endif //MINECRAFT_IN_C_OPENGL_STUFF_H
