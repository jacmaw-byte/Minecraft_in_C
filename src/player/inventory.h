//
// Created by jacobmawby on 17/09/2026.
//

#ifndef MINECRAFT_IN_C_INVENTORY_H
#define MINECRAFT_IN_C_INVENTORY_H
#include <items.h>

#define INVENTORY_HEIGHT 3
#define INVENTORY_WIDTH 9
#define INVENTORY_CAPACITY (INVENTORY_HEIGHT * INVENTORY_WIDTH)
#define HOTBAR_SIZE 9

struct Inventory {
    Item items_in_inventory[INVENTORY_CAPACITY];
    Item items_in_hotbar[HOTBAR_SIZE];
};

#endif //MINECRAFT_IN_C_INVENTORY_H
