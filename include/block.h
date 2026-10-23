//
// Created by jacobmawby on 10/09/2026.
//

#ifndef MINECRAFT_IN_C_BLOCK_H
#define MINECRAFT_IN_C_BLOCK_H

typedef enum Block : unsigned char{
    AIR,
    GRASS,
    DIRT,
    WATER,
    GRASS_2D,
    GLASS
}Block;

struct BlockData {
    bool transparent;
    int top_tex_id;
    int side_tex_id;
    int bottom_tex_id;
    bool bill_boarded;
    bool culled;
};

extern struct BlockData block_attribute_db[];

#endif //MINECRAFT_IN_C_BLOCK_H
