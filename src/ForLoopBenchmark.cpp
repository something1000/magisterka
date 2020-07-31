#include "ForLoopBenchmark.hpp"
#include <iostream>
#include <omp.h>
#include <cstdlib>
#include <thread>

void ForLoopBenchmark::RunParallel() {
    int dummy = 0;

    auto excel = *this->file;
    int warmup = 20;
    int rounds = 10000;

    BENCHMARK_STRUCTURE(
        excel,      // name of csv logger
        "Parallel",
        warmup,     // name of warmup rounds variable
        rounds,     // name of benchmark rounds variable
        ELAPSED,    // variable name to store execution time
        {
            mpragma(omp parallel for)
            for(int i = 0; i < 100000; i++ )
            {
                LOOP_UNOPTIMIZER(dummy);
            }
        }
    )
}

void ForLoopBenchmark::RunSerial() {
    int dummy=0;

    auto excel = *this->file;
    int warmup = 20;
    int rounds = 10000;
    BENCHMARK_STRUCTURE(
        excel,      // name of csv logger
        "Serial",
        warmup,     // name of warmup rounds variable
        rounds,     // name of benchmark rounds variable
        ELAPSED,    // variable name to store execution time
        {
            for(int i = 0; i < 100000; i++ )
            {
                LOOP_UNOPTIMIZER(dummy);
            }
        }
    )
}

void ForLoopBenchmark::Init(Logger::LoggerClass* file) {
    this->file = file;
}