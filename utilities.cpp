#include "utilities.hpp"
#include "quicksort.hpp"
#include "mergesort.hpp"
#include <iostream>
#include <chrono>
#include <cstdlib>
#include <ctime>
#include <algorithm>

vector<int> generate_random_data(long N) {
    vector<int> data(N);
    srand(std::time(nullptr));
    generate(data.begin(), data.end(), []() { return std::rand(); });
    return data;
}

void test_scalability() {
    const vector<long> sizes = {100000, 1000000, 10000000};

    for (long N : sizes) {
        vector<int> data = generate_random_data(N);

        vector<int> copy = data;

        cout << "Testing N = " << N << "\n";

        // Sekwencyjny QuickSort
        auto start = chrono::high_resolution_clock::now();
        quicksort_serial(copy.data(), 0, N - 1);
        auto end = chrono::high_resolution_clock::now();
        chrono::duration<double> seq_quicksort_time = end - start;
        cout << "Sequential QuickSort time: " << seq_quicksort_time.count() << " seconds\n";

        // Równoległy QuickSort
        copy = data;
        start = chrono::high_resolution_clock::now();
        quicksort_parallel_wrapper(copy.data(), N);
        end = chrono::high_resolution_clock::now();
        chrono::duration<double> par_quicksort_time = end - start;
        cout << "Parallel QuickSort time: " << par_quicksort_time.count() << " seconds\n";

        cout << "QuickSort Speedup: " << seq_quicksort_time.count() / par_quicksort_time.count() << "\n\n";

        // Sekwencyjny MergeSort
        copy = data;
        vector<int> temp(N);
        start = chrono::high_resolution_clock::now();
        mergesort_serial(copy.data(), temp.data(), 0, N - 1);
        end = chrono::high_resolution_clock::now();
        chrono::duration<double> seq_mergesort_time = end - start;
        cout << "Sequential MergeSort time: " << seq_mergesort_time.count() << " seconds\n";

        // Równoległy MergeSort
        copy = data;
        start = chrono::high_resolution_clock::now();
        mergesort_parallel_wrapper(copy.data(), N);
        end = chrono::high_resolution_clock::now();
        chrono::duration<double> par_mergesort_time = end - start;
        cout << "Parallel MergeSort time: " << par_mergesort_time.count() << " seconds\n";

        cout << "MergeSort Speedup: " << seq_mergesort_time.count() / par_mergesort_time.count() << "\n\n";
    }
}
