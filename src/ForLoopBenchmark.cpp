#include "ForLoopBenchmark.hpp"
#include <iostream>
#include <omp.h>
#include <cstdlib>
#include <thread>
void ForLoopBenchmark::RunParallel() {
    int dummy = 0;
    int threadID = 0;
    auto excel = *this->file;
    excel << "Test!";
    int warmup = 20;
    int rounds = 10000;
    BENCHMARK_STRUCTURE(warmup, rounds, ELAPSED, {
        mpragma(omp parallel for private(threadID))
        for(int i = 0; i < 100000; i++ )
        {
           LOOP_UNOPTIMIZER(dummy);
        }
    })

    Logger::INFO << "Dummy" << dummy;
    excel << "HEHESZKI!";
}

void ForLoopBenchmark::RunSerial() {
    int dummy=0;

    auto excel = *this->file;
    excel << "Test!";
    int warmup = 20;
    int rounds = 10000;
    BENCHMARK_STRUCTURE(warmup, rounds, ELAPSED, {
        for(int i = 0; i < 100000; i++ )
        {
            LOOP_UNOPTIMIZER(dummy);
        }
    })
    Logger::INFO << ELAPSED << " ";
    excel << "HEHESZKI!";
    Logger::INFO << "Dummy" << dummy;
}

void ForLoopBenchmark::Init(Logger::LoggerClass* file) {
    this->file = file;
}