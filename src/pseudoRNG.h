#ifndef PSEUDO_RNG_H
#define PSEUDO_RNG_H

#include <stdint.h>

#define SEED_DEFAULT 1234

// LINEAR CONGRUENTIAL GENERATORS
// Apparently if it's not ULL then it throws a tantrum
// ULL means unsigned long long
#define LCG_DEFAULT_A 6364136223846793005ULL
#define LCG_DEFAULT_C 1442695040888963407ULL

// Macros for Linear Congruential Generator initialisation
extern void __lcg_init(uint64_t seed, uint64_t param_a, uint64_t param_c);

#define GET_LCG_MACRO(_1, _2, _3, NAME, ...) NAME

#define lcg_init(...) GET_LCG_MACRO(__VA_ARGS__, lcg_init4, lcg_init3, lcg_init2, lcg_init1)(__VA_ARGS__)

// Case 1: user provides nothing
#define lcg_init1() __lcg_init(SEED_DEFAULT, LCG_DEFAULT_A, LCG_DEFAULT_C)
// Case 2: user provides only seed
#define lcg_init2(seed) __lcg_init(seed, LCG_DEFAULT_A, LCG_DEFAULT_C)
// Case 3: user provides seed, A => Use default C
#define lcg_init3(seed, a) __lcg_init(seed, a, LCG_DEFAULT_C)
// Case 4: user provides everything (surprising) => Pass it all through
#define lcg_init4(seed, a, c) __lcg_init(seed, a, c)

// LCG Next function
uint64_t lcg_next();

// MIDDLE SQUARE
extern void __msm_init(uint64_t seed);

#define GET_MSM_MACRO(_1, NAME, ...) NAME

#define msm_init(...) GET_MSM_MACRO(__VA_ARGS__, msm_init2, msm_init1)(__VA_ARGS__)

// Case 1: user provided nothing
#define msm_init1() __msm_init(SEED_DEFAULT)
// Case 2: user provided seed
#define msm_init2(seed) __msm_init(seed)

// MSM Next function
uint64_t msm_next();

// CUSTOM PRNG
extern void __custom_prng_init(uint64_t seed);

#define GET_CUSTOM_PRNG_MACRO(_1, _2, _3, NAME, ...) NAME

#define custom_prng_init(...) GET_CUSTOM_PRNG_MACRO(__VA_ARGS__, custom_prng_init2, custom_prng_init1)(__VA_ARGS__)

// Case 1: user provided nothing
#define custom_prng_init1() __custom_prng_init(SEED_DEFAULT)
// Case 2: user provided seed
#define custom_prng_init2(seed) __custom_prng_init(seed)

uint64_t custom_prng_next();
#endif