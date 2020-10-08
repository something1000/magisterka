#include "QuickSort.hpp"
#include <iostream>
#include <omp.h>
#include <cstring>
#include <functional>

float PartitionArray(float *array, int left, int right) {
    int partitionIndex = left + (right - left) / 2;
    float partitionValue = array[partitionIndex];
    std::swap(array[partitionIndex], array[right]);

    int currentPosition = left;
    for(int i=left; i < right; ++i) {
        if (array[i] < partitionValue) {
            std::swap(array[i], array[currentPosition]);
            currentPosition++;
        }
    }
    std::swap(array[currentPosition], array[right]);
    return currentPosition;
}


void QuickSort::RunParallel() {

    auto excel = *this->file;
    int warmup = 10;
    int rounds = 1000;

}

void QuickSort::RunSerial() {

    auto excel = *this->file;
    int warmup = 5;
    int rounds = 100;

    std::function<void(float*, int, int)> Quicksort;
    Quicksort = [&Quicksort](float *array, int left, int right){
        if ( left < right ) {
            float p = PartitionArray(array, left, right);
            Quicksort(array, left, p - 1); // Left branch
            Quicksort(array, p + 1, right); // Right branch
        }
    };
    BENCHMARK_STRUCTURE(
        excel,      // name of csv logger
        "Serial",   // name of benchmark
        warmup,     // name of warmup rounds variable
        rounds,     // name of benchmark rounds variable
        ELAPSED,    // variable name to store execution time
        {
            std::memcpy(data, input_data, size * sizeof(float));
            Quicksort(data, 0, size-1);
        }
   )
}

void QuickSort::Init(Logger::LoggerClass* file) {
    this->file = file;
    size = 20000;
    input_data = new float[size];
    data = new float[size];
    FillRandomArray(input_data, size);
    this->initialized = true;
}