#include "MatrixMultiplication.hpp"
#include <iostream>
#include <omp.h>
#include <cstring>

void MatrixMultiplication::RunParallel() {

    auto excel = *this->file;
    int warmup = 10;
    int rounds = 1000;
    {
        BENCHMARK_STRUCTURE(
            excel,                  // name of csv logger
            "Parallel_Collapse",    // name of benchmark
            warmup,                 // name of warmup rounds variable
            rounds,                 // name of benchmark rounds variable
            ELAPSED_Collapse,       // variable name to store execution time
            {
                mpragma(omp parallel for collapse(2) schedule(static)) // static as every thread have same work to do
                for(int i=0; i < N; i++) {
                    for(int j=0; j < N; j++) {
                        result[i][j] = 0.0f;
                        for(int k=0; k < M; k++) {
                            result[i][j] += sourceA[i][k] * sourceB[k][j];
                        }
                    }
                }
            }
        )
    }

    // second benchmark without collapse clause
    {
        BENCHMARK_STRUCTURE(
            excel,              // name of csv logger
            "Parallel_Normal",  // name of benchmark
            warmup,             // name of warmup rounds variable
            rounds,             // name of benchmark rounds variable
            ELAPSED,            // variable name to store execution time
            {
                mpragma(omp parallel for schedule(static)) // static as every thread have same work to do
                for(int i=0; i < N; i++) {
                    for(int j=0; j < N; j++) {
                        result[i][j] = 0.0f;
                        for(int k=0; k < M; k++) {
                            result[i][j] += sourceA[i][k] * sourceB[k][j];
                        }
                    }
                }
            }
        )
    }
}


void MatrixMultiplication::RunSerial() {

    auto excel = *this->file;
    int warmup = 10;
    int rounds = 1000;
    BENCHMARK_STRUCTURE(
        excel,      // name of csv logger
        "Serial",   // name of benchmark
        warmup,     // name of warmup rounds variable
        rounds,     // name of benchmark rounds variable
        ELAPSED,    // variable name to store execution time
        {
            for(int i=0; i < N; i++) {
                for(int j=0; j < N; j++) {
                    result[i][j] = 0.0f;
                    for(int k=0; k < M; k++) {
                        result[i][j] += sourceA[i][k] * sourceB[k][j];
                    }
                }
            }
        }
    )
}
void MatrixMultiplication::Init(Logger::LoggerClass* file) {
    this->file = file;
    
    N = 20;
    M = 20;
    // Create contingouse memory arrays

    // NxM * MxN = NxN
    sourceA = Create2DArray<float>(N, M);
    sourceB = Create2DArray<float>(M, N);
    result  = Create2DArray<float>(N, N);
    FillRandom2DArray(sourceA, N, M);
    FillRandom2DArray(sourceB, M, N);
    std::memset(result, N*N, 0);
    this->initialized = true;
}