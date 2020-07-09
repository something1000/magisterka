#ifndef _BENCHMARK_HPP
#define _BENCHMARK_HPP

class Benchmark {
    public:
        virtual void RunSerial() = 0;
        virtual void RunParallel() = 0;
        virtual void Init() = 0;
};

#endif