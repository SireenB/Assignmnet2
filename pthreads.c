#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define nb_buckets 10
#define nb_threads 5
#define size 1000

int arr[size];
int segsize = size / nb_threads;

typedef struct {
    int* bucket;
    int count;
} Bucket;

Bucket buckets[nb_buckets];

void* bucketsort(void* arg) {
    Bucket* bucket = (Bucket*)arg;
    // Simple insertion sort for the bucket
    int i, j, key;
    for (i = 1; i < bucket->count; i++) {
        key = bucket->bucket[i];
        j = i - 1;

        while (j >= 0 && bucket->bucket[j] > key) {
            bucket->bucket[j + 1] = bucket->bucket[j];
            j = j - 1;
        }
        bucket->bucket[j + 1] = key;
    }
    pthread_exit(NULL);
}

int main() {
    pthread_t threads[nb_threads];
    int i, j;
    struct timespec start, end;
    double comptime, commtime, totaltime;

    // Initialize array with random numbers
    srand(time(NULL));
    for (i = 0; i < size; i++) {
        arr[i] = rand() % 1000; // Adjust range as needed
    }

    // Initialize buckets
    for (i = 0; i < nb_threads; i++) {
        buckets[i].bucket = (int*)malloc((size / nb_buckets) * sizeof(int));
        buckets[i].count = 0;
    }

    // Distribute array elements into buckets
    for (i = 0; i < size; i++) {
        int index = i % nb_buckets; // Distribute elements evenly among buckets
        buckets[index].bucket[buckets[index].count++] = array[i];
    }

    clock_gettime(CLOCK_MONOTONIC, &start);

    // Create threads to sort each bucket
    for (i = 0; i < nb_threads; i++) {
        pthread_create(&threads[i], NULL, sort_bucket, (void*)&buckets[i]);
    }

    // Join threads
    for (i = 0; i < nb_threads; i++) {
        pthread_join(threads[i], NULL);
    }

    clock_gettime(CLOCK_MONOTONIC, &end);

    // Merge sorted buckets back into the original array
    int idx = 0;
    for (i = 0; i < nb_buckets; i++) {
        for (j = 0; j < buckets[i].count; j++) {
            array[idx++] = buckets[i].bucket[j];
        }
        free(buckets[i].bucket);
    }

    // Calculate execution time
    comptime = (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / 1e9;
    commtime = comptime; // Assuming communication time is negligible
    totaltime = comptime + commtime;

    // Output results
    printf("Total Execution Time: %.6f seconds\n", totaltime);
    printf("Computation Time: %.6f seconds\n", comptime);
    printf("Communication Time: %.6f seconds\n", commtime);

    return 0;
}
