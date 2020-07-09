#include "Benchmark.hpp"
#ifndef _FORLOOP_HPP
#define _FORLOOP_HPP

class ForLoopBenchmark : public Benchmark {
    public:
        virtual void RunSerial() override;
        virtual void RunParallel() override;
        virtual void Init() override;
};

#endif