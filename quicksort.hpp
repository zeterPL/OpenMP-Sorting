#ifndef QUICKSORT_H
#define QUICKSORT_H

using namespace std;

void quicksort_serial(int* A, long lo, long hi);
void quicksort_parallel_wrapper(int* A, long N);

#endif // QUICKSORT_H
