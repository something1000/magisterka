#ifndef _SECOND_HPP
#define _SECOND_HPP
#include "Benchmark.hpp"
#include "common.hpp"

class MatrixMultiplication : public Benchmark {
    public:
        MatrixMultiplication(std::string name) {
            this->name = name;
        };
        virtual void RunSerial() override;
        virtual void RunParallel() override;
        virtual void Init(Logger::LoggerClass* file, const rapidjson::Value& properties) override;
        virtual ~MatrixMultiplication() {
            if(initialized) {
                Free2DArray<float>(sourceA);
                Free2DArray<float>(sourceB);
                Free2DArray<float>(result);
            }
        }
    private:
        Logger::LoggerClass* file;
        int rounds;
        int warmup;
        bool initialized = false;
        int N, M;
        float** sourceA;
        float** sourceB;
        float** result;
};

#endif