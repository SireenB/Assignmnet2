#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define nb_buckets 10
#define size 1000

int arr[size];

typedef struct {
    int* bucket;
    int count;
} Bucket;

Bucket buckets[nb_buckets];

int compare(const void* a, const void* b) {
    return *((int*)a) - *((int*)b);
}

int main() {
    int i, j;
    double start, end, comptime, commtime, totaltime;
    // Initialize array with random numbers
    srand(time(NULL));
    for (i = 0; i < size; i++) {
        arr[i] = rand() % 1000; // Adjust range as needed
    }
    // Initialize buckets
    for (i = 0; i < nb_buckets; i++) {
        // Calculate the size of each bucket dynamically based on the number of elements it will contain
        int bucket_size = (size / nb_buckets) + ((i < size % nb_buckets) ? 1 : 0);
        buckets[i].bucket = (int*)malloc(bucket_size * sizeof(int));
        buckets[i].count = 0;
    }
    // Distribute array elements into buckets
    for (i = 0; i < size; i++) {
        // Calculate the index of the bucket for the current element
        int index = arr[i] % nb_buckets;
        // Add the current element to the corresponding bucket
        buckets[index].bucket[buckets[index].count++] = array[i];
    }
    // Measure execution time
    start = omp_get_wtime();

    // Parallel sort each bucket using OpenMP
    #pragma omp parallel for
    for (i = 0; i < nb_buckets; i++) {
        qsort(buckets[i].bucket, buckets[i].count, sizeof(int), compare);
    }

    // Merge sorted buckets back into the original array
    int idx = 0;
    for (i = 0; i < nb_buckets; i++) {
        for (j = 0; j < buckets[i].count; j++) {
            arr[idx++] = buckets[i].bucket[j];
        }
        free(buckets[i].bucket);
    }

    end = omp_get_wtime();

    // Calculate execution time
    totaltime = end- start;

    // Computation time is the time spent sorting
    comptime = totaltime;

    // Communication time is 0 because there's no explicit communication
    commtime = 0.0;

    // Output results
    printf("Total Execution Time: %.6f seconds\n", totaltime);
    printf("Computation Time: %.6f seconds\n", comptime);
    printf("Communication Time: %.6f seconds\n", commtime);

    // Calculate communication/computation ratio
    double ratio = commtime / comptime;
    printf("Communication/Computation Ratio: %.6f\n", ratio);

    return 0;
}
