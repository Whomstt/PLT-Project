#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define SIZE 1000000 // Define a large array size for better performance visibility

// Function using regular loop to sum array elements
void sum_regular(int arr[], int size) {
    int sum = 0;
    clock_t start, end;
    double cpu_time_used;
    
    start = clock();
    
    // Regular loop - process one element per iteration
    for (int i = 0; i < size; i++) {
        sum += arr[i];
    }
    
    end = clock();
    cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
    
    printf("Regular loop: Sum = %d\n", sum);
    printf("Time taken by regular loop: %.10f seconds\n\n", cpu_time_used);
}

// Function using loop unrolling to sum array elements
void sum_unrolled(int arr[], int size) {
    int sum = 0;
    clock_t start, end;
    double cpu_time_used;
    int i;
    
    start = clock();
    
    // Process 4 elements per iteration
    // First handle the main chunk of array that's divisible by 4
    for (i = 0; i < size - 3; i += 4) {
        sum += arr[i];
        sum += arr[i + 1];
        sum += arr[i + 2];
        sum += arr[i + 3];
    }
    
    // Handle the remaining elements (if any)
    for (; i < size; i++) {
        sum += arr[i];
    }
    
    end = clock();
    cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
    
    printf("Unrolled loop: Sum = %d\n", sum);
    printf("Time taken by loop unrolling: %.10f seconds\n", cpu_time_used);
}

int main() {
    // Allocate memory for array
    int *arr = (int *)malloc(SIZE * sizeof(int));
    if (arr == NULL) {
        printf("Memory allocation failed\n");
        return 1;
    }
    
    // Fill the array with 1's for simplicity
    for (int i = 0; i < SIZE; i++) {
        arr[i] = 1;
    }
    
    // Test regular loop
    sum_regular(arr, SIZE);
    
    // Test unrolled loop
    sum_unrolled(arr, SIZE);
    
    // Free allocated memory
    free(arr);
    
    return 0;
}