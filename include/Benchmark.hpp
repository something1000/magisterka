#ifndef _BENCHMARK_HPP
#define _BENCHMARK_HPP

#include <memory>
#include "Logger.hpp"
#include "rapidjson/document.h"

class Benchmark {
    public:
        virtual void RunSerial() = 0;
        virtual void RunParallel() = 0;
        virtual bool Validate() {
            return true;
        };
        virtual void Init(Logger::LoggerClass* file, const rapidjson::Value& properties) = 0;
    protected:
        std::string name = "Benchmark";
};

#endif