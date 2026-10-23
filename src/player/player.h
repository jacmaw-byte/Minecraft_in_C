//
// Created by jacobmawby on 17/09/2026.
//

#ifndef MINECRAFT_IN_C_PLAYER_H
#define MINECRAFT_IN_C_PLAYER_H

#include "../dda.h"
#include "inventory.h"
#include "../camera.h"

struct Player {
    struct Camera cam;
    struct Inventory inventory;
};

void destroy_block(struct RayHit* hit);
void place_block(struct RayHit* hit_info);

#endif //MINECRAFT_IN_C_PLAYER_H
