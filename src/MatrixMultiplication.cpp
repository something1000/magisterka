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

void MatrixMultiplication::RunParallel_2() {
    auto excel = *this->file;

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
void MatrixMultiplication::Init(Logger::LoggerClass* file, const rapidjson::Value& properties) {
    this->file = file;
    rounds = properties["rounds"].GetInt();
    warmup = properties["warmup"].GetInt();
    N = properties["N"].GetInt();
    M = properties["M"].GetInt();
    Logger::INFO << VAR(N) << VAR(M);

    // Create contingouse memory arrays
    // NxM * MxN = NxN
    sourceA = Create2DArray<float>(N, M);
    sourceB = Create2DArray<float>(M, N);
    result  = Create2DArray<float>(N, N);
    FillRandom2DArray(sourceA, N, M);
    FillRandom2DArray(sourceB, M, N);
    std::memset(result, 0, N*N);
    this->initialized = true;
}