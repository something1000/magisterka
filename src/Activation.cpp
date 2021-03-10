#include "Activation.hpp"
#include <iostream>
#include <omp.h>
#include <cstring>
#include <functional>

void Activation::RunParallel() {
    RunParallel_1();
    RunParallel_2();
    RunParallel_3();
}



void Activation::RunParallel_1() {
    auto excel = *this->file;

    BENCHMARK_STRUCTURE(
        excel,      // name of csv logger
        "PARALLEL_FOR_COLLAPSE",   // name of benchmark
        warmup,     // name of warmup rounds variable
        rounds,     // name of benchmark rounds variable
        ELAPSED,    // variable name to store execution time
        {
            mpragma(omp parallel for collapse(4) schedule(static, static_size))
            for(int j=0; j < N; ++j) {
                for(int z=0; z < C; ++z) {
                    for(int k=0; k < H; ++k) {
                        for(int p=0; p < W; ++p) {
                            output[j][z][k][p] = output[j][z][k][p] > 0 ? output[j][z][k][p] : 0;
                        }
                    }
                }
            }
        }
   )
}

void Activation::RunParallel_2() {
    auto excel = *this->file;

    float* raw_input = input[0][0][0];
    float* raw_output = output[0][0][0];
    BENCHMARK_STRUCTURE(
        excel,      // name of csv logger
        "PARALLEL_FOR",   // name of benchmark
        warmup,     // name of warmup rounds variable
        rounds,     // name of benchmark rounds variable
        ELAPSED,    // variable name to store execution time
        {
            int size = N*C*H*W;
            mpragma(omp parallel for schedule(static, static_size))
            for(int i=0; i < size; i++) {
                raw_output[i] = raw_input[i] > 0 ? raw_input[i] : 0;
            }
        }
   )
}


void Activation::RunParallel_3() {
    auto excel = *this->file;

    float* raw_input = input[0][0][0];
    float* raw_output = output[0][0][0];
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
                raw_output[i] = raw_input[i] > 0 ? raw_input[i] : 0;
            }
        }
   )
}

void Activation::RunSerial() {
    auto excel = *this->file;

    float* raw_input = input[0][0][0];
    float* raw_output = output[0][0][0];
    BENCHMARK_STRUCTURE(
        excel,      // name of csv logger
        "Serial",   // name of benchmark
        warmup,     // name of warmup rounds variable
        rounds,     // name of benchmark rounds variable
        ELAPSED,    // variable name to store execution time
        {
            for(int i=0; i < N*C*H*W; i++) {
                raw_output[i] = raw_input[i] > 0 ? raw_input[i] : 0;
            }
        }
   )
}

void Activation::Init(Logger::LoggerClass* file, const rapidjson::Value& properties) {
    this->file = file;
    rounds = properties["rounds"].GetInt();//128;
    warmup = properties["warmup"].GetInt();//128;

    N =  properties["N"].GetInt();
    C =  properties["C"].GetInt();
    H =  properties["H"].GetInt();
    W =  properties["W"].GetInt();
    static_size = properties["static_size"].GetInt();
    Logger::INFO << VAR(N) << VAR(C) << VAR(H) << VAR(W) << VAR(static_size);

    input = Create4DArray<float>(N, C, H, W);
    output = Create4DArray<float>(N, C, H, W);

    FillRandomArray(input[0][0][0], N*C*H*W);
    this->initialized = true;
}