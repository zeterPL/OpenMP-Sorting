#include "quicksort.hpp"
#include <algorithm>
#include <omp.h>

constexpr long cutoff = 10000;

long partition(int* A, long lo, long hi) {
    int pivot = A[hi];
    long i = lo - 1;
    for (long j = lo; j <= hi - 1; j++) {
        if (A[j] < pivot) {
            i++;
            swap(A[i], A[j]);
        }
    }
    swap(A[i + 1], A[hi]);
    return i + 1;
}

void quicksort_serial(int* A, long lo, long hi) {
    if (lo < hi) {
        long p = partition(A, lo, hi);
        quicksort_serial(A, lo, p - 1);
        quicksort_serial(A, p + 1, hi);
    }
}

void quicksort_parallel(int* A, long lo, long hi) {
    if (hi <= lo) return;
    if (hi - lo < cutoff) {
        quicksort_serial(A, lo, hi);
        return;
    }

    long p = partition(A, lo, hi);
    #pragma omp task
    quicksort_parallel(A, lo, p - 1);
    #pragma omp task
    quicksort_parallel(A, p + 1, hi);
}

void quicksort_parallel_wrapper(int* A, long N) {
    #pragma omp parallel
    {
        #pragma omp single
        quicksort_parallel(A, 0, N - 1);
    }
}
