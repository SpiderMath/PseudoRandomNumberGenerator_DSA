#include "quicksort.h"

static void swap(void *a, void *b, size_t size) {
	char t;
	char *p = (char *)a;
	char *q = (char *)b;
	size_t i;
	for (i = 0; i < size; ++i) {
		t = p[i];
		p[i] = q[i];
		q[i] = t;
	}
}

static void quicksort_recursive(void *base, size_t size, int (*compar)(const void *, const void *), int left, int right) {
	if (left >= right) return;

	char *ptr_base = (char *)base;
	int pivot_idx = right;
	char *pivot_ptr = ptr_base + (pivot_idx * size);

	int i = left - 1;
	int j;

	for (j = left; j < right; j++) {
		char *curr_ptr = ptr_base + (j * size);

		if (compar(curr_ptr, pivot_ptr) <= 0) {
			i++;
			char *swap_dest = ptr_base + (i * size);
			swap(curr_ptr, swap_dest, size);
		}
	}

	char *final_pivot_pos = ptr_base + ((i + 1) * size);
	swap(final_pivot_pos, pivot_ptr, size);

	int partition_index = i + 1;

	quicksort_recursive(base, size, compar, left, partition_index - 1);
	quicksort_recursive(base, size, compar, partition_index + 1, right);
}

void quicksort(void *base, size_t nmemb, size_t size, int (*compar)(const void *, const void *)) {
	if (nmemb > 0)
		quicksort_recursive(base, size, compar, 0, (int)nmemb - 1);
}
