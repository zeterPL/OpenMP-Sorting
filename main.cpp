#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <algorithm>
#include <omp.h>
#include <chrono>

constexpr long cutoff = 10000; // Próg przejścia na wersję sekwencyjną

long partition(int *A, long lo, long hi)
{
    int pivot = A[hi];
    long i = lo - 1;
    for (long j = lo; j <= hi - 1; j++)
    {
        if (A[j] < pivot)
        {
            i++;
            std::swap(A[i], A[j]);
        }
    }
    std::swap(A[i + 1], A[hi]);
    return i + 1;
}

// Sekwencyjny QuickSort
void quicksort_serial(int *A, long lo, long hi)
{
    if (lo < hi)
    {
        long p = partition(A, lo, hi);
        quicksort_serial(A, lo, p - 1);
        quicksort_serial(A, p + 1, hi);
    }
}

// Równoległy QuickSort z OpenMP
void quicksort_parallel(int *A, long lo, long hi)
{
    if (hi <= lo)
        return;

    if (hi - lo < cutoff)
    {
        quicksort_serial(A, lo, hi);
        return;
    }

    long p = partition(A, lo, hi);

#pragma omp task default(none) firstprivate(lo, p, A)
    quicksort_parallel(A, lo, p - 1);

#pragma omp task default(none) firstprivate(hi, p, A)
    quicksort_parallel(A, p + 1, hi);
}

void quicksort_parallel_wrapper(int *A, long N)
{
#pragma omp parallel
    {
#pragma omp single
        quicksort_parallel(A, 0, N - 1);
    }
}

// Sekwencyjny MergeSort
void mergesort_serial(int *A, int *temp, long lo, long hi)
{
    if (lo >= hi)
        return;

    long mid = lo + (hi - lo) / 2;
    mergesort_serial(A, temp, lo, mid);
    mergesort_serial(A, temp, mid + 1, hi);

    long i = lo, j = mid + 1, k = lo;
    while (i <= mid && j <= hi)
    {
        if (A[i] <= A[j])
        {
            temp[k++] = A[i++];
        }
        else
        {
            temp[k++] = A[j++];
        }
    }
    while (i <= mid)
        temp[k++] = A[i++];
    while (j <= hi)
        temp[k++] = A[j++];

    for (long l = lo; l <= hi; l++)
    {
        A[l] = temp[l];
    }
}

// Równoległy MergeSort z OpenMP
void mergesort_parallel(int *A, int *temp, long lo, long hi)
{
    if (hi - lo < cutoff)
    {
        mergesort_serial(A, temp, lo, hi);
        return;
    }

    long mid = lo + (hi - lo) / 2;

#pragma omp task default(none) firstprivate(A, temp, lo, mid)
    mergesort_parallel(A, temp, lo, mid);

#pragma omp task default(none) firstprivate(A, temp, mid, hi)
    mergesort_parallel(A, temp, mid + 1, hi);

#pragma omp taskwait

    long i = lo, j = mid + 1, k = lo;
    while (i <= mid && j <= hi)
    {
        if (A[i] <= A[j])
        {
            temp[k++] = A[i++];
        }
        else
        {
            temp[k++] = A[j++];
        }
    }
    while (i <= mid)
        temp[k++] = A[i++];
    while (j <= hi)
        temp[k++] = A[j++];

    for (long l = lo; l <= hi; l++)
    {
        A[l] = temp[l];
    }
}

void mergesort_parallel_wrapper(int *A, long N)
{
    std::vector<int> temp(N);
#pragma omp parallel
    {
#pragma omp single
        mergesort_parallel(A, temp.data(), 0, N - 1);
    }
}

// Generowanie losowych danych
std::vector<int> generate_random_data(long N)
{
    std::vector<int> data(N);
    std::srand(std::time(nullptr));
    std::generate(data.begin(), data.end(), []()
                  { return std::rand(); });
    return data;
}

// Funkcja testująca skalowalność
void test_scalability()
{
    const std::vector<long> sizes = {100000, 1000000, 10000000, 100000000};

    for (long N : sizes)
    {
        std::vector<int> data = generate_random_data(N);

        std::vector<int> copy = data;

        std::cout << "Testing N = " << N << "\n";

        // Sekwencyjny QuickSort
        auto start = std::chrono::high_resolution_clock::now();
        quicksort_serial(copy.data(), 0, N - 1);
        auto end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> seq_time = end - start;
        std::cout << "Sequential QuickSort time: " << seq_time.count() << " seconds\n";

        // Równoległy QuickSort
        copy = data;
        start = std::chrono::high_resolution_clock::now();
        quicksort_parallel_wrapper(copy.data(), N);
        end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> par_time = end - start;
        std::cout << "Parallel QuickSort time: " << par_time.count() << " seconds\n";

        std::cout << "Speedup: " << seq_time.count() / par_time.count() << "\n\n";

        // Sekwencyjny MergeSort
        copy = data;
        std::vector<int> temp(N);
        start = std::chrono::high_resolution_clock::now();
        mergesort_serial(copy.data(), temp.data(), 0, N - 1);
        end = std::chrono::high_resolution_clock::now();
        seq_time = end - start;
        std::cout << "Sequential MergeSort time: " << seq_time.count() << " seconds\n";

        // Równoległy MergeSort
        copy = data;
        start = std::chrono::high_resolution_clock::now();
        mergesort_parallel_wrapper(copy.data(), N);
        end = std::chrono::high_resolution_clock::now();
        par_time = end - start;
        std::cout << "Parallel MergeSort time: " << par_time.count() << " seconds\n";

        std::cout << "Speedup: " << seq_time.count() / par_time.count() << "\n\n";
    }
}

int main()
{
    test_scalability();
    return 0;
}
