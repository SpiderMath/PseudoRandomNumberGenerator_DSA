#include <stdint.h>

#ifndef QUICKSORT_H
#define QUICKSORT_H

/**
 * Sorts an array of integers using the Quicksort algorithm.
 * @param arr   The array to be sorted
 * @param low   The starting index (usually 0)
 * @param high  The ending index (usually size - 1)
 */
void quickSort(uint64_t arr[], int low, int high);

#endif