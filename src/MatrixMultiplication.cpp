#include "MatrixMultiplication.hpp"
#include <iostream>
#include <omp.h>
#include <cstring>

void MatrixMultiplication::RunParallel() {
    int i;
    int zz  = 0;
    int threadID = 0;
    #pragma omp parallel for private(i, threadID)
    for(i = 0; i < 16; i++ )
    {
        threadID = omp_get_thread_num();
        //#pragma omp critical
        {
            zz+=i;
            //printf("Thread %d reporting\n", threadID*i);
        }
    }

}


void MatrixMultiplication::RunSerial() {

    int dummy=0;
    auto excel = *this->file;
    int warmup = 2;
    int rounds = 100;
    BENCHMARK_STRUCTURE(
        excel,      // name of csv logger
        "Serial",
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
    
    N = 1000;
    M = 100;
    // Create contingouse memory arrays

    sourceA = Create2DArray<float>(N, M);
    sourceB = Create2DArray<float>(M, N);
    result  = Create2DArray<float>(N, N);
    FillRandom2DArray(sourceA, N, M);
    FillRandom2DArray(sourceB, M, N);
    sourceB[0][0] = -1;
    sourceA[0][0] = 2;
    std::memset(result, N*N, 0);
    this->initialized = true;
}