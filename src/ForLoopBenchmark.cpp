#include "ForLoopBenchmark.hpp"
#include <iostream>
#include <omp.h>
#include <cstdlib>
#include <thread>
void ForLoopBenchmark::RunParallel() {
    int i;
    int zz  = 0;
    int threadID = 0;
    auto excel = *this->file;
    excel << "Test!";
    TIME(tic);
    
    #pragma omp parallel for private(i, threadID)
    for(i = 0; i < 30000000; i++ )
    {
        threadID = omp_get_thread_num();
        #pragma omp critical
        {
            zz+=rand() % 100;
        }
    }

    TIME(toc);
    auto elapsed = toc - tic;
    Logger::INFO << elapsed << " " << zz;
    excel << "HEHESZKI!";
}

void ForLoopBenchmark::RunSerial() {
    auto r = rand() % 40;
    
    std::this_thread::sleep_for(std::chrono::milliseconds(r));
}

void ForLoopBenchmark::Init(Logger::LoggerClass* file) {
    this->file = file;
}