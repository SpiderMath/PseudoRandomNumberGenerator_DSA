#include "pseudoRNG.h"

typedef uint64_t u64;
typedef uint32_t u32;

// ----------- IMPLEMENTATION OF LINEAR CONGRUENTIAL GENERATOR -----------

static u64 lcg_state;
static u64 a;
static u64 c;

void __lcg_init(u64 seed, u64 param_a, u64 param_c) {
    lcg_state = seed;
    a = param_a;
    c = param_c;
}

u64 lcg_next() {
    lcg_state = (a * lcg_state) + c;

    return lcg_state;
}

// ----------- IMPLEMENTATION OF MIDDLE SQUARE METHOD -----------

static u64 msm_state;

void __msm_init(u64 seed) {
    msm_state = seed;
}

u64 msm_next() {
    msm_state = msm_state << 2;
}

// ----------- IMPLEMENTATION OF THE EXPANDER GRAPH METHOD -----------

static u32 g_x;
static u32 g_y;
static u64 g_lcg_state = 0;


void __custom_prng_init(u64 seed, u64 gx, u64 gy) {
    lcg_init(seed);
    g_x = gx;
    g_y = gy;
    u64 start_pos = lcg_next();

    g_x = (uint32_t)(start_pos >> 32);
    g_y = (uint32_t)(start_pos & 0xFFFFFFFF);
}


u64 custom_prng_next() {
    // Take 3 steps on the graph to mix the LCG's output
    for (int i = 0; i < 3; i++) {

        // We use the LCG output AND our current position to decide.
        u64 driver_input = lcg_next() ^ g_x ^ g_y;
        int choice = driver_input & 3;

        // Moving the Walker (Margulis Expander Transformations) [added +1 in case of reaching (x, y) = (0, 0)]
        switch (choice) {
            case 0: g_y = g_x + g_y + 1; break; // Move Up-Right skew
            case 1: g_x = g_x + g_y + 1; break; // Move Right-Up skew
            case 2: g_y = g_y - g_x + 1; break; // Move Down-Left skew
            case 3: g_x = g_x - g_y + 1; break; // Move Left-Down skew
        }
    }

    return ((u64)g_x << 32) | g_y;
}
