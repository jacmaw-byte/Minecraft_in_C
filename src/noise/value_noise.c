//
// Created by jacobmawby on 11/09/2026.
//
#include "value_noise.h"
#include <stdlib.h>

uint32_t noise_seed = 1337;
static inline uint32_t hash(uint32_t x)
{
    x ^= x >> 16;
    x *= 0x7feb352d;
    x ^= x >> 15;
    x *= 0x846ca68b;
    x ^= x >> 16;
    return x;
}

static inline float get_random(uint32_t seed)
{
    return (float)hash(seed) / 4294967296.0f;
}
static inline uint32_t hash2d(int x, int y)
{
    uint32_t h = (uint32_t)x * 0x8da6b343u;
    h ^= (uint32_t)y * 0xd8163841u;
    return hash(h);
}
float value_noise(float x, float y)
{
    int x1 = (int)floorf(x);
    int y1 = (int)floorf(y);

    int x2 = x1 + 1;
    int y2 = y1 + 1;

    float tx = x - (float)x1;
    float ty = y - (float)y1;

    float tl = get_random(hash2d(x1, y1) ^ noise_seed);
    float tr = get_random(hash2d(x2, y1) ^ noise_seed);
    float bl = get_random(hash2d(x1, y2) ^ noise_seed);
    float br = get_random(hash2d(x2, y2) ^ noise_seed);

    tx = smoothstep(tx);
    ty = smoothstep(ty);

    float top = tl + (tr - tl) * tx;
    float bottom = bl + (br - bl) * tx;

    return top + (bottom - top) * ty;
}
float value_noise_FBM(float x, float y, float f, float l, float p, float octaves) {
    float a = 1.0f;
    float t = 0.0f;
    for(int i = 0; i < octaves; i++) {
        t += a * value_noise(f * x, f * y);
        f *= l;
        a *= p;
    }
    return t;
}


void set_seed(int new_seed) {
    noise_seed = new_seed;
}