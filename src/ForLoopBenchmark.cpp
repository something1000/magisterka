#include "ForLoopBenchmark.hpp"
#include <iostream>
#include <omp.h>
#include "common.hpp"

void ForLoopBenchmark::RunParallel() {
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
            printf("Thread %d reporting\n", threadID*i);
        }
    }

}

void ForLoopBenchmark::RunSerial() {

}

void ForLoopBenchmark::Init() {
    Logger::INFO << "init for loop";
}