#include "ForLoopBenchmark.hpp"
#include <iostream>
#include <omp.h>
#include "BenchTimer.hpp"

void ForLoopBenchmark::Run() {
    int i;
    int zz  = 0;
    int threadID = 0;
    BenchTimer bt;
    bt.Tic();
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

    bt.Tock();

    bt.Print();
}

void ForLoopBenchmark::Init() {
    std::cout << "xInitDD1D";
}