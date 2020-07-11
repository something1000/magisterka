#include "ForLoopBenchmark.hpp"
#include <iostream>
#include <omp.h>
#include "common.hpp"
#include <cstdlib>
#include <thread>
void ForLoopBenchmark::RunParallel() {
    int i;
    int zz  = 0;
    int threadID = 0;

    auto r = rand() % 10;
    //printf("XXXXXXXXXX%d", r);
    std::this_thread::sleep_for(std::chrono::milliseconds(r));
    // #pragma omp parallel for private(i, threadID)
    // for(i = 0; i < 16; i++ )
    // {
    //     threadID = omp_get_thread_num();
    //     //#pragma omp critical
    //     {
    //         zz+=i;
    //         printf("Thread %d reporting\n", threadID*i);
    //     }
    // }

}

void ForLoopBenchmark::RunSerial() {
    auto r = rand() % 40;
    
    std::this_thread::sleep_for(std::chrono::milliseconds(r));
}

void ForLoopBenchmark::Init() {
    Logger::INFO << "init for loop";
}