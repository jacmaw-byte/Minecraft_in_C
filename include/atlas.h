//
// Created by jacobmawby on 10/09/2026.
//

#ifndef MINECRAFT_IN_C_ATLAS_H
#define MINECRAFT_IN_C_ATLAS_H
// How many images across
#define ATLAS_SIZE 8
#include <cglm/cglm.h>

struct TexCoords {
    vec2 tr, tl, bl, br;
};

static inline struct TexCoords get_atlas_tex_pos(int index) {
    const float tex_size = 1.0f / ATLAS_SIZE;
    const float x_offset = tex_size * (float)(index % ATLAS_SIZE);
    const float y_offset = tex_size * (float)(index / ATLAS_SIZE);

    struct TexCoords tex_coords = {
        .tr = {tex_size + x_offset, 1.0f - y_offset},
        .tl = {x_offset,     1.0f - y_offset},
        .bl = {x_offset,     1.0f - (tex_size + y_offset)},
        .br = {tex_size + x_offset, 1.0f - (tex_size + y_offset)}
    };

    return tex_coords;
}

#endif //MINECRAFT_IN_C_ATLAS_H
