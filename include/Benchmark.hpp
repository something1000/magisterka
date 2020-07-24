#ifndef _BENCHMARK_HPP
#define _BENCHMARK_HPP

#include <memory>
#include "Logger.hpp"

class Benchmark {
    public:
        virtual void RunSerial() = 0;
        virtual void RunParallel() = 0;
        virtual void Init(Logger::LoggerClass* file) = 0;
};

#endif