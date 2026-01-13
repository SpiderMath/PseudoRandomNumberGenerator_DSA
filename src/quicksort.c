#include "quicksort.h"

static void swap(uint64_t* a, uint64_t* b) {
    uint64_t t = *a;
    *a = *b;
    *b = t;
}

static int partition(uint64_t arr[], int low, int high) {
    uint64_t pivot = arr[high];
    int i = (low - 1);

    for (int j = low; j <= high - 1; j++) {
        if (arr[j] < pivot) {
            i++;
            swap(&arr[i], &arr[j]);
        }
    }
    swap(&arr[i + 1], &arr[high]);
    
    return (i + 1);
}

void quickSort(uint64_t arr[], int low, int high) {
    if (low < high) {
        int pi = partition(arr, low, high);

        quickSort(arr, low, pi - 1);
        quickSort(arr, pi + 1, high);
    }
}