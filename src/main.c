#include "pseudoRNG.h"
#include "quicksort.h"
#include <stdio.h>

typedef uint64_t (*rng_func)(void);

rng_func setupRNG() {
    // If using Linear Congruent Generators
    lcg_init(
        69420
    );
    rng_func next = *lcg_next;

    // If using Middle Square
    // msm_init(69420);
    // rng_func next = *msm_next;

    // If using my custom graph
    // custom_prng_init(69420);
    // rng_func next = *custom_prng_next;

    return next;
}

int main() {
    rng_func next = setupRNG();
    uint64_t numbers[10];

    printf("Input Array: ");
    for(int i = 0; i < 10; i++) {
        numbers[i] = next();
        printf("%lu ", numbers[i]);
    }

    printf("\n\n\nOutput Array: ");

    quickSort(numbers, 0, 9);

    for(int i = 0; i < 9; i++)
        printf("%lu ", numbers[i]);
}