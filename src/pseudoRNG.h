#ifndef PSEUDORNG_H
#define PSEUDORNG_H

#include <stdint.h>

/**
 * @param state A pointer to the current state (seed) of the generator 
 * The function updates this value for the next call
 * @return The generated pseudo-random 32-bit integer
 */
uint32_t lcg_rand(uint32_t *state);

/**
 * @param state A pointer to the current state (seed) of the generator
 * The function updates this value for the next call
 * Note: This algorithm can possibly go to zero if the seed is not chosen carefully
 * @return The generated pseudo-random 32-bit integer
 */
uint32_t middle_square_rand(uint32_t *state);

/*
 * EXPANDER GRAPH PRNG
 * Based on Arnold's Cat Map and Xorshift.
 * State Space: 2^64 (represented as a 2D grid of 2^32 x 2^32).
 * Period: ~2^64 (Hopefully? Results from testing the first 2^14 outputs on 500 seeds seems promising till now)
 */
typedef struct {
    uint32_t x;
    uint32_t y;
} ExpanderPRNG;

/**
 * @brief Initializes the PRNG state
 * @param ctx Pointer to the PRNG context structure
 * @param seed A 64-bit seed value
 * (Note: If you set seed 0, it'll change it to what I've hard-coded)
 */
void prng_init(ExpanderPRNG *ctx, uint64_t seed);

/**
 * @brief Advances the state and returns the next pseudo-random number
 * Steps:
 * 1. Applies Arnold's Cat 🐈 for Expansion
 * 2. Applies XOR-Rotation for Non-linearity and unpredictability
 * @param ctx Pointer to the PRNG context structure
 * @return uint64_t A 64-bit pseudo-random number
 */
uint64_t prng_next(ExpanderPRNG *ctx);

#endif