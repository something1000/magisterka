#ifndef _SECOND_HPP
#define _SECOND_HPP
#include "Benchmark.hpp"
#include "common.hpp"

class SecondBenchmark : public Benchmark {
    public:
        SecondBenchmark(std::string name) {
            this->name = name;
        };
        virtual void RunSerial() override;
        virtual void RunParallel() override;
        virtual void Init(Logger::LoggerClass* file) override;
    private:
        Logger::LoggerClass* file;
};

#endif