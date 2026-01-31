#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "pseudoRNG.h"
#include "quicksort.h"

typedef uint64_t u64;
typedef uint32_t u32;

/**
 * @brief Helper to handle input defaults
 * Returns 1 if user typed something valid, 0 if they just hit Enter (skip)
 */
int get_input_u32(const char *prompt, u32 *out_val) {
	printf("%s", prompt);
	char buffer[64];

	if (fgets(buffer, sizeof(buffer), stdin) != NULL) {
		// Check if empty (just newline)
		if (buffer[0] == '\n')
			return 0;

		// Basic parsing
		char *endptr;
		unsigned long val = strtoul(buffer, &endptr, 10);

		if (endptr != buffer && (*endptr == '\n' || *endptr == '\0')) {
			*out_val = (u32)val;
			return 1;
		}
	}
	return 0; // Error or something => time to use default
}

/**
 * @brief Comparator for quicksort (u32)
 */
int compare_u32(const void *a, const void *b) {
	u32 arg1 = *(const u32 *)a;
	u32 arg2 = *(const u32 *)b;

	if (arg1 < arg2) return -1;
	if (arg1 > arg2) return 1;

	return 0;
}

int main() {
	int choice = 0;
	u32 seed = 42, seed_copy;
	u32 count = 1000;
	char *prng_name = "Unknown";

	// ----- 1. Choose PRNG -----
	while (choice < 1 || choice > 3) {
		printf("\033[33m Select PRNG: \033[0m\n");
		printf("1. Linear Congruential Generator (LCG)\n");
		printf("2. Middle Square Method [\033[3mPlease put in a large seed in that case\033[0m]\n");
		printf("3. Expander Graph (Arnold's Cat + Xorshift)\n");
		printf("\033[33m Choice [1-3]:\033[0m ");

		char buffer[16];
		if (fgets(buffer, sizeof(buffer), stdin)) {
			choice = atoi(buffer);
		}
	}

	// ----- 2. Get Seed (Default 42) -----
	// Answer to the ULTIMATE QUESTION OF LIFE, THE UNIVERSE, AND EVERYTHING
	if (!get_input_u32("\033[36m Enter Seed (default 42): \033[0m", &seed))
		seed = 42;

	seed_copy = seed;

	// ----- 3. Get Count (Default 1000, Min 3) -----
	if (!get_input_u32("\033[35m Enter count (default 1000): \033[0m ", &count))
		count = 1000;

	if (count < 1) count = 1;

	u32 *raw_data = malloc(count * sizeof(u32));
	u32 *sorted_data = malloc(count * sizeof(u32));

	if (!raw_data || !sorted_data) {
		fprintf(stderr, "\033[31mMemory allocation failed. \033[0m\n");
		return 1;
	}

	// Initialize State variables
	u32 lcg_state = seed;
	u32 ms_state = seed;
	ExpanderPRNG ex_ctx; 

	if (choice == 3)
		prng_init(&ex_ctx, (u64)seed);

	// ----- 4. Generate Numbers -----
	printf("\nGenerating %u integers...\n", count);
	for (size_t i = 0; i < count; i++) {
		switch (choice) {
			case 1:
				prng_name = "LCG";
				raw_data[i] = lcg_rand(&lcg_state);
				break;
			case 2:
				prng_name = "Middle Square";
				raw_data[i] = middle_square_rand(&ms_state);
				break;
			case 3:
				prng_name = "Expander Graph";
				// Cast 64-bit result to 32-bit for uniformity in this test
				raw_data[i] = (u32)prng_next(&ex_ctx);
				break;
		}
	}

	// Copy to sorted buffer obviously
	memcpy(sorted_data, raw_data, count * sizeof(u32));

	// ----- 5. Run Quicksort -----
	printf("\033[33mSorting...\033[0m \n");
	quicksort(sorted_data, count, sizeof(u32), compare_u32);

	// ----- 6. Write to File -----
	const char *filename = "prng_report.txt";
	FILE *f = fopen(filename, "w");
	if (!f) {
		perror("\033[31mFailed to open file\033[0m");
		free(raw_data);
		free(sorted_data);
		return 1;
	}

	fprintf(f, "%s Demonstration Run\n", prng_name);
	fprintf(f, "Number of Random Integers: %u\n", count);
	fprintf(f, "Chosen Seed: %u\n", seed_copy);

	fprintf(f, "Random Integers:\n");

	for (size_t i = 0; i < count; i++)
		fprintf(f, "%u ", raw_data[i]);

	fprintf(f, "\n");
	fprintf(f, "Sorted Integers:\n");

	for (size_t i = 0; i < count; i++)
		fprintf(f, "%u ", sorted_data[i]);

	fprintf(f, "\n");

	fclose(f);
	printf("\033[32mDone! Results written to %s\033[0m\n", filename);

	// Cleanup
	free(raw_data);
	free(sorted_data);

	return 0;
}
