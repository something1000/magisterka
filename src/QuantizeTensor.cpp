#include "QuantizeTensor.hpp"
#include <iostream>
#include <omp.h>
#include <cstring>
#include <functional>

void QuantizeTensor::RunParallel() {
    RunParallel_1();
    RunParallel_2();
    RunParallel_3();
}



void QuantizeTensor::RunParallel_1() {

    auto excel = *this->file;
    int warmup = 5;
    int rounds = 1000;

    float* raw_input = input[0][0][0];
    int8_t* raw_output = output[0][0][0];
    BENCHMARK_STRUCTURE(
        excel,      // name of csv logger
        "PARALLEL_FOR_COLLAPSE",   // name of benchmark
        warmup,     // name of warmup rounds variable
        rounds,     // name of benchmark rounds variable
        ELAPSED,    // variable name to store execution time
        {
            mpragma(omp parallel for collapse(4) schedule(static, 100))
            for(int j=0; j < N; ++j) {
                for(int z=0; z < C; ++z) {
                    for(int k=0; k < H; ++k) {
                        for(int p=0; p < W; ++p) {
                            output[j][z][k][p] = static_cast<int8_t>((input[j][z][k][p]/scale) + zero_position);
                        }
                    }
                }
            }
        }
   )
}

void QuantizeTensor::RunParallel_2() {

    auto excel = *this->file;
    int warmup = 5;
    int rounds = 1000;

    float* raw_input = input[0][0][0];
    int8_t* raw_output = output[0][0][0];
    BENCHMARK_STRUCTURE(
        excel,      // name of csv logger
        "PARALLEL_FOR",   // name of benchmark
        warmup,     // name of warmup rounds variable
        rounds,     // name of benchmark rounds variable
        ELAPSED,    // variable name to store execution time
        {
            int size = N*C*H*W;
            mpragma(omp parallel for schedule(static, 100))
            for(int i=0; i < size; i++) {
                raw_output[i] = static_cast<int8_t>((raw_input[i]/scale) + zero_position);
            }
        }
   )
}


void QuantizeTensor::RunParallel_3() {

    auto excel = *this->file;
    int warmup = 5;
    int rounds = 1000;

    float* raw_input = input[0][0][0];
    int8_t* raw_output = output[0][0][0];
    BENCHMARK_STRUCTURE(
        excel,      // name of csv logger
        "PARALLEL_FOR_SIMD",   // name of benchmark
        warmup,     // name of warmup rounds variable
        rounds,     // name of benchmark rounds variable
        ELAPSED,    // variable name to store execution time
        {
            int size = N*C*H*W;
            mpragma(omp parallel for simd)
            for(int i=0; i < size; i++) {
                raw_output[i] = static_cast<int8_t>((raw_input[i]/scale) + zero_position);
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