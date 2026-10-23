//
// Created by jacobmawby on 11/09/2026.
//

#ifndef MINECRAFT_IN_C_VALUE_NOISE_H
#define MINECRAFT_IN_C_VALUE_NOISE_H
#include <math.h>
#include <stdint.h>

static inline float smoothstep(float t) {
    return t * t * (3 - 2 * t);
}
static inline float smoothstep_prime(float t) {
    return 6 * (t - t * t);
}
float value_noise(float x, float y);
void set_seed(int new_seed);
float value_noise_FBM(float x, float y, float f, float l, float p, float octaves);

#endif //MINECRAFT_IN_C_VALUE_NOISE_H
