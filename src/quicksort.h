#ifndef QUICKSORT_H
#define QUICKSORT_H

#include <stddef.h>

/**
 * @param base Pointer to the first element of the array to be sorted.
 * @param nmemb The number of elements in the array.
 * @param size The size in bytes of each element.
 * @param compar Function pointer that compares two elements. 
 * Must return an integer less than, equal to, or greater than zero 
 * if the first argument is considered to be respectively less than, 
 * equal to, or greater than the second.
 */
void quicksort(void *base, size_t nmemb, size_t size, int (*compar)(const void *, const void *));

#endif
