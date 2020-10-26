#include "QuantizeTensor.hpp"
#include <iostream>
#include <omp.h>
#include <cstring>
#include <functional>

void QuantizeTensor::RunParallel() {

    auto excel = *this->file;
    int warmup = 5;
    int rounds = 1000;

    float* raw_input = input[0][0][0];
    int8_t* raw_output = output[0][0][0];
    BENCHMARK_STRUCTURE(
        excel,      // name of csv logger
        "Parallel",   // name of benchmark
        warmup,     // name of warmup rounds variable
        rounds,     // name of benchmark rounds variable
        ELAPSED,    // variable name to store execution time
        {
            mpragma(omp simd)
            for(int i=0; i < N*C*H*W; i++) {
                raw_output[i] = (raw_input[i]/scale) + zero_position;
            }
        }
   )
}

void QuantizeTensor::RunSerial() {

    auto excel = *this->file;
    int warmup = 5;
    int rounds = 1000;
    float* raw_input = input[0][0][0];
    int8_t* raw_output = output[0][0][0];
    BENCHMARK_STRUCTURE(
        excel,      // name of csv logger
        "Serial",   // name of benchmark
        warmup,     // name of warmup rounds variable
        rounds,     // name of benchmark rounds variable
        ELAPSED,    // variable name to store execution time
        {
            for(int i=0; i < N*C*H*W; i++) {
                raw_output[i] = (raw_input[i]/scale) + zero_position;
            }
        }
   )
}

void QuantizeTensor::Init(Logger::LoggerClass* file) {
    this->file = file;

    N = 32;
    C = 3;
    H = 224;
    W = 224;
    scale = 0.0384f;
    zero_position = 0;
    
    input = Create4DArray<float>(N, C, H, W);
    output = Create4DArray<int8_t>(N, C, H, W);

    FillRandomArray(input[0][0][0], N*C*H*W);
    this->initialized = true;
}