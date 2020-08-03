#ifndef _EMPTYFORLOOP_HPP
#define _EMPTYFORLOOP_HPP
#include "Benchmark.hpp"
#include "common.hpp"

class EmptyForLoopBenchmark : public Benchmark {
    public:
        EmptyForLoopBenchmark(std::string name) {
            this->name = name;
        };
        virtual void RunSerial() override;
        virtual void RunParallel() override;
        virtual void Init(Logger::LoggerClass* file) override;
    private:
        Logger::LoggerClass* file;
};

#endif