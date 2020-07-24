#include "SecondBenchmark.hpp"
#include <iostream>
#include <omp.h>

void SecondBenchmark::RunParallel() {
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


void SecondBenchmark::RunSerial() {

}
void SecondBenchmark::Init(Logger::LoggerClass* file) {
    this->file = file;
}