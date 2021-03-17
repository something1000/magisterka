#include "MatrixMultiplication.hpp"
#include <iostream>
#include <omp.h>
#include <cstring>

void MatrixMultiplication::RunParallel() {
    RunParallel_1();
    RunParallel_2();
}

void MatrixMultiplication::RunParallel_1() {
    auto excel = *this->file;

    BENCHMARK_STRUCTURE(
        excel,                  // name of csv logger
        "Parallel_Collapse",    // name of benchmark
        warmup,                 // name of warmup rounds variable
        rounds,                 // name of benchmark rounds variable
        ELAPSED_Collapse,       // variable name to store execution time
        {
            mpragma(omp parallel for collapse(2) schedule(static)) // static as every thread have same work to do
            for(int i=0; i < N; i++) {
                for(int j=0; j < K; j++) {
                    result[i][j] = 0.0f;
                    for(int c=0; c < M; c++) {
                        result[i][j] += sourceA[i][c] * sourceB[c][j];
                    }
                }
            }
        }
    )
}

void MatrixMultiplication::RunParallel_2() {
    auto excel = *this->file;

    // second benchmark without collapse clause
    BENCHMARK_STRUCTURE(
        excel,              // name of csv logger
        "Parallel_Normal",  // name of benchmark
        warmup,             // name of warmup rounds variable
        rounds,             // name of benchmark rounds variable
        ELAPSED,            // variable name to store execution time
        {
            mpragma(omp parallel for schedule(static)) // static as every thread have same work to do
            for(int i=0; i < N; i++) {
                for(int j=0; j < K; j++) {
                    result[i][j] = 0.0f;
                    for(int c=0; c < M; c++) {
                        result[i][j] += sourceA[i][c] * sourceB[c][j];
                    }
                }
            }
        }
    )
}


void MatrixMultiplication::RunSerial() {

    auto excel = *this->file;
    BENCHMARK_STRUCTURE(
        excel,      // name of csv logger
        "Serial",   // name of benchmark
        warmup,     // name of warmup rounds variable
        rounds,     // name of benchmark rounds variable
        ELAPSED,    // variable name to store execution time
        {
            for(int i=0; i < N; i++) {
                for(int j=0; j < K; j++) {
                    result[i][j] = 0.0f;
                    for(int c=0; c < M; c++) {
                        result[i][j] += sourceA[i][c] * sourceB[c][j];
                    }
                }
            }
        }
    )
}

bool MatrixMultiplication::Validate() {
    Tensor2D<float> out_serial = Create2DArray<float>(N, K);
    Tensor2D<float> out_parallel_2 = Create2DArray<float>(N, K);
    Tensor2D<float> out_parallel_1 = Create2DArray<float>(N, K);
    rounds = 1;
    warmup = 0;

    Tensor2D<float> tmp = result;

    result = out_serial;
    RunSerial();

    result = out_parallel_1;
    RunParallel_1();

    result = out_parallel_2;
    RunParallel_2();

    bool is_valid = Compare2DArray(out_serial, out_parallel_1, N, K);
    is_valid = is_valid && Compare2DArray(out_serial, out_parallel_2, N, K);

    result = tmp;
    Free2DArray<float>(out_serial);
    Free2DArray<float>(out_parallel_1);
    Free2DArray<float>(out_parallel_2);
    return is_valid;
}


void MatrixMultiplication::Init(Logger::LoggerClass* file, const rapidjson::Value& properties) {
    this->file = file;
    rounds = properties["rounds"].GetInt();
    warmup = properties["warmup"].GetInt();
    N = properties["N"].GetInt();
    M = properties["M"].GetInt();
    K = properties["K"].GetInt();
    Logger::INFO << VAR(N) << VAR(M) << VAR(K);

    Reinitialize();
}

void MatrixMultiplication::Reinitialize() {
    if(initialized) {
        Free2DArray<float>(sourceA);
        Free2DArray<float>(sourceB);
        Free2DArray<float>(result);
    }
    // Create contingouse memory arrays
    // NxM * MxK = NxK
    sourceA = Create2DArray<float>(N, M);
    sourceB = Create2DArray<float>(M, K);
    result  = Create2DArray<float>(N, K);
    FillRandom2DArray(sourceA, N, M);
    FillRandom2DArray(sourceB, M, K);
    float* raw_result_ptr = result[0];
    std::memset(raw_result_ptr, 0, N * K * sizeof(float));
    this->initialized = true;
}