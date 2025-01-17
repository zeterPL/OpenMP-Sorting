#include "utilities.hpp"
#include "quicksort.hpp"
#include "mergesort.hpp"
#include <iostream>
#include <chrono>
#include <cstdlib>
#include <ctime>
#include <algorithm>

std::vector<int> generate_random_data(long N) {
    std::vector<int> data(N);
    std::srand(std::time(nullptr));
    std::generate(data.begin(), data.end(), []() { return std::rand(); });
    return data;
}

void test_scalability() {
    const std::vector<long> sizes = {100000, 1000000, 10000000};

    for (long N : sizes) {
        std::vector<int> data = generate_random_data(N);

        std::vector<int> copy = data;

        std::cout << "Testing N = " << N << "\n";

        auto start = std::chrono::high_resolution_clock::now();
        quicksort_serial(copy.data(), 0, N - 1);
        auto end = std::chrono::high_resolution_clock::now();
        std::cout << "QuickSort time: " << std::chrono::duration<double>(end - start).count() << " seconds\n";

        copy = data;
        start = std::chrono::high_resolution_clock::now();
        mergesort_serial(copy.data(), new int[N], 0, N - 1);
        end = std::chrono::high_resolution_clock::now();
        std::cout << "MergeSort time: " << std::chrono::duration<double>(end - start).count() << " seconds\n";
    }
}
