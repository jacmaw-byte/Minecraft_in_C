//
// Created by jacobmawby on 14/09/2026.
//
#include <constants.h>
#include <block.h>
#include "globals.h"

ivec3 directions[6] = {
    {0, 1, 0}, // Y+
    {0, -1, 0}, // Y-
    {1, 0, 0}, // X+
    {-1, 0, 0}, // X-
    {0, 0, 1}, // Z+
    {0, 0, -1}, // Z-
};

ivec2 neighbour_chunk_dirs[4] = {
    {1, 0},
    {0, 1},
    {-1, 0},
    {0, -1}
};

struct BlockData block_attribute_db[] = {
    {.transparent = true, .top_tex_id = -1, .side_tex_id = -1, .bottom_tex_id = -1, .bill_boarded = false, .culled = false}, //Air
    {.transparent = false, .top_tex_id = 0, .side_tex_id = 1, .bottom_tex_id = 2, .bill_boarded = false, .culled = true}, //Grass
    {.transparent = false, .top_tex_id = 2, .side_tex_id = 2, .bottom_tex_id = 2, .bill_boarded = false, .culled = true}, //Dirt
    {.transparent = true, .top_tex_id = 3, .side_tex_id = 3, .bottom_tex_id = 3, .bill_boarded = false, .culled = true}, //Water
    {.transparent = true, .top_tex_id = 4, .side_tex_id = -1, .bottom_tex_id = -1, .bill_boarded = true, .culled = false}, //Grass 2D
    {.transparent = true, .top_tex_id = 5, .side_tex_id = 5, .bottom_tex_id = 5, .bill_boarded = false, .culled = true} //Glass
};


struct RayHit hit;
struct Player player;
float delta_time;
float last_x = 400.0f;
float last_y = 300.0f;
bool first_mouse = true;
float last_time = 0.0f;
