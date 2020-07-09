#ifndef _BENCHENGINE_HPP
#define _BENCHENGINE_HPP
#include "common.hpp"

class BenchEngine {
    public:
        static void Start(BenchmarkMap benchmarks);
        static void SetWarmupRounds(uint rounds);
        static void Configure(){
            warmupRounds = 20;
            executionRounds = 100;
        }
    private:
        static uint warmupRounds;
        static uint executionRounds;

};
#endif