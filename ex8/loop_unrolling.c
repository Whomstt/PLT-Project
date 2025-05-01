#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define SIZE 1000000  // Array size for performance test
#define UNROLL_FACTOR 4

// Measure and print execution time of a given summing function
static void measure(const char *label, int (*sum_func)(const int *, int), const int *arr, int size) {
    clock_t start = clock();
    int sum = sum_func(arr, size);
    clock_t end = clock();
    double elapsed = (double)(end - start) / CLOCKS_PER_SEC;

    printf("%s: Sum = %d, Time = %.8f s\n", label, sum, elapsed);
}

// Sum array elements with a simple loop
static int sum_simple(const int *arr, int size) {
    int sum = 0;
    for (int i = 0; i < size; ++i) {
        sum += arr[i];
    }
    return sum;
}

// Sum array elements with loop unrolling
static int sum_unrolled(const int *arr, int size) {
    int sum = 0;
    int i = 0;
    int limit = size - (size % UNROLL_FACTOR);

    // Unrolled loop
    for (; i < limit; i += UNROLL_FACTOR) {
        sum += arr[i] + arr[i+1] + arr[i+2] + arr[i+3];
    }
    // Remainder
    for (; i < size; ++i) {
        sum += arr[i];
    }
    return sum;
}

int main(void) {
    int *arr = malloc(sizeof *arr * SIZE);
    if (!arr) {
        fprintf(stderr, "Memory allocation failed\n");
        return EXIT_FAILURE;
    }

    // Initialize array
    for (int i = 0; i < SIZE; ++i) {
        arr[i] = 1;
    }

    // Run benchmarks
    measure("Simple loop", sum_simple, arr, SIZE);
    measure("Unrolled loop", sum_unrolled, arr, SIZE);

    free(arr);
    return EXIT_SUCCESS;
}
