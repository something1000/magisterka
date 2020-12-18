#include "QuickSort.hpp"
#include <iostream>
#include <omp.h>
#include <cstring>
#include <functional>

// TODO: replace lambdas to functions
//TODO add final clause benchmark

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


void ParallelQuicksort(float* array, int left, int right) {

}
void QuickSort::RunParallel() {

    auto excel = *this->file;
    int x = 1;
    std::function<void(float*, int, int)> ParallelQuicksort;
    ParallelQuicksort = [&ParallelQuicksort](float *array, int left, int right){
        if ( left < right ) {
            
            float p = PartitionArray(array, left, right);
            mpragma(omp task default(none) firstprivate(ParallelQuicksort, array, left, p)){
                ParallelQuicksort(array, left, p - 1);
            }
            mpragma(omp task default(none) firstprivate(ParallelQuicksort, array, right, p)){
                ParallelQuicksort(array, p + 1, right);
            }
        }
    };
    BENCHMARK_STRUCTURE(
        excel,      // name of csv logger
        "Parallel",   // name of benchmark
        warmup,     // name of warmup rounds variable
        rounds,     // name of benchmark rounds variable
        ELAPSED,    // variable name to store execution time
        {
            std::memcpy(data, input_data, size * sizeof(float));
            mpragma(omp parallel shared(data, size)) {
                mpragma(omp single) {
                    ParallelQuicksort(data, 0, size-1);
                }
                #pragma omp taskwait
            }
        }
   )
//    Print2DArray(&data, 1, size);
//    std::cout << "\n\n=============\n\n";
}

void QuickSort::RunSerial() {

    auto excel = *this->file;

    int x = 1;
    std::function<void(float*, int, int)> Quicksort;
    Quicksort = [&Quicksort, &x](float *array, int left, int right){
        if ( left < right ) {
            float p = PartitionArray(array, left, right);
            Quicksort(array, left, p - 1); // Left branch
            Quicksort(array, p + 1, right); // Right branch
            //std::cout << "x: " << x++ << std::endl;
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
    // Print2DArray(&data, 1, size);
    // std::cout << "\n\n=============\n\n";
}

void QuickSort::Init(Logger::LoggerClass* file, const rapidjson::Value& properties) {
    this->file = file;
    rounds = properties["rounds"].GetInt();
    warmup = properties["warmup"].GetInt();
    size = properties["size"].GetInt();
    Logger::INFO << VAR(size);

    input_data = new float[size];
    data = new float[size];
    FillRandomArray(input_data, size);
    this->initialized = true;
}