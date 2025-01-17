#include "mergesort.hpp"
#include <omp.h>
#include <vector>

constexpr long cutoff = 10000;

void mergesort_serial(int* A, int* temp, long lo, long hi) {
    if (lo >= hi) return;

    long mid = lo + (hi - lo) / 2;
    mergesort_serial(A, temp, lo, mid);
    mergesort_serial(A, temp, mid + 1, hi);

    long i = lo, j = mid + 1, k = lo;
    while (i <= mid && j <= hi) {
        if (A[i] <= A[j]) temp[k++] = A[i++];
        else temp[k++] = A[j++];
    }
    while (i <= mid) temp[k++] = A[i++];
    while (j <= hi) temp[k++] = A[j++];

    for (long l = lo; l <= hi; l++) A[l] = temp[l];
}

void mergesort_parallel(int* A, int* temp, long lo, long hi) {
    if (hi - lo < cutoff) {
        mergesort_serial(A, temp, lo, hi);
        return;
    }

    long mid = lo + (hi - lo) / 2;
    #pragma omp task
    mergesort_parallel(A, temp, lo, mid);
    #pragma omp task
    mergesort_parallel(A, temp, mid + 1, hi);
    #pragma omp taskwait

    long i = lo, j = mid + 1, k = lo;
    while (i <= mid && j <= hi) {
        if (A[i] <= A[j]) temp[k++] = A[i++];
        else temp[k++] = A[j++];
    }
    while (i <= mid) temp[k++] = A[i++];
    while (j <= hi) temp[k++] = A[j++];

    for (long l = lo; l <= hi; l++) A[l] = temp[l];
}

void mergesort_parallel_wrapper(int* A, long N) {
    std::vector<int> temp(N);
    #pragma omp parallel
    {
        #pragma omp single
        mergesort_parallel(A, temp.data(), 0, N - 1);
    }
}
