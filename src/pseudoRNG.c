#include "pseudoRNG.h"

typedef uint64_t u64;
typedef uint32_t u32;

// ----------- IMPLEMENTATION OF LINEAR CONGRUENTIAL GENERATOR -----------

u32 lcg_rand(u32 *state) {
    u32 a = 1664525;
    u32 c = 1013904223;

    *state = (a * (*state) + c);

    return *state;
}

// ----------- IMPLEMENTATION OF MIDDLE SQUARE METHOD -----------

u32 middle_square_rand(u32 *state) {
    u64 square = (u64)(*state) * (u64)(*state);
    u64 middle = (square >> 16) & 0xFFFFFFFF;

    *state = (u32)middle;

    return *state;
}

// ----------- IMPLEMENTATION OF THE EXPANDER GRAPH METHOD -----------

// Helper function to rotate the number
static inline uint32_t rotl32(uint32_t x, int k) {
    return (x << k) | (x >> (32 - k));
}

void prng_init(ExpanderPRNG *ctx, uint64_t seed) {
    // We split the seed into two 32 bit numbers
    ctx->x = (uint32_t)(seed & 0xFFFFFFFF);
    ctx->y = (uint32_t)(seed >> 32);

    // The origin (0,0) is a fixed point in every linear transformation 
    // If seed is 0, gotta bump it. The users are stupid ffs
    if (ctx->x == 0 && ctx->y == 0) {
        ctx->x = 0xDEADBEEF;
        ctx->y = 0xCAFEBABE;
    }
}

// The actual generator
uint64_t prng_next(ExpanderPRNG *ctx) {
    /*
    1. THE EXPANDER STEP (Linear Mixing)
        We apply Arnold's Cat Map transformation: 
        | x_new |   | 2  1 |   | x_old |
        | y_new | = | 1  1 | x | y_old | (mod 2^32)

        This matrix has a determinant of 1, preserving area (bijective),
        and eigenvalues != 1, ensuring exponential stretching.
        And it goes really crazy really fast! (Chaotic Systems)

        In code, we will do this via two shears for efficiency:
            y = x + y
            x = y_new + x = (x + y) + x = 2x + y
    */
    ctx->y = ctx->x + ctx->y;
    ctx->x = ctx->x + ctx->y;

    /*
    2. Currying (NOT THE LAMBDA CALCULUS) the output
        Pure linear maps fail statistical tests due to linearity.
        Time to insert non-linearity using a Xorshift style rotation.
        We mix x into y, and y into x.
    */
    ctx->x ^= rotl32(ctx->y, 7);
    ctx->y ^= rotl32(ctx->x, 13);

    // 3. Recombine to form a 64-bit output
    return ((uint64_t)ctx->x << 32) | ctx->y;
}
