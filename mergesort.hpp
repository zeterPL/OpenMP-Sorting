#ifndef MERGESORT_H
#define MERGESORT_H

using namespace std;

void mergesort_serial(int* A, int* temp, long lo, long hi);
void mergesort_parallel_wrapper(int* A, long N);

#endif // MERGESORT_H
