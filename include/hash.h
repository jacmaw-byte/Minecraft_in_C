//
// Created by jacobmawby on 12/09/2026.
//

#ifndef MINECRAFT_IN_C_HASH_H
#define MINECRAFT_IN_C_HASH_H

#include <stdint.h>

// Uses fnv-1a for hashing
static inline uint32_t hash_32_impl(void* v, int num_bytes) {
    uint32_t hash = 0x811c9dc5;
    for (int i = 0; i < num_bytes; ++i) {
        hash ^= *((unsigned char *)v + i);
        hash *= 0x01000193;
    }
    return hash;
}
static inline uint64_t hash_64_impl(void* v, int num_bytes) {
    uint64_t hash = 0xcbf29ce484222325;

    for (int i = 0; i < num_bytes; ++i) {
        hash ^= *((unsigned char *)v + i);
        hash *= 0x00000100000001b3;
    }
    return hash;
}

#endif //MINECRAFT_IN_C_HASH_H
