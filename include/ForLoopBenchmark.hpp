#ifndef _FORLOOP_HPP
#define _FORLOOP_HPP
#include "Benchmark.hpp"
#include "common.hpp"

class ForLoopBenchmark : public Benchmark {
    public:
        virtual void RunSerial() override;
        virtual void RunParallel() override;
        virtual void Init(Logger::LoggerClass* file) override;
    private:
        Logger::LoggerClass* file;
};

#endif