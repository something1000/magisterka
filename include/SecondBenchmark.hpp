#include "Benchmark.hpp"
#ifndef _SECOND_HPP
#define _SECOND_HPP

class SecondBenchmark : public Benchmark {
    public:
        virtual void RunSerial() override;
        virtual void RunParallel() override;
        virtual void Init() override;
};

#endif