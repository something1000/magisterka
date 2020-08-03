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
        virtual void Init(Logger::LoggerClass* file) override;
        virtual ~MatrixMultiplication() {
            if(initialized) {
                delete[] sourceA[0];
                delete[] sourceA;

                delete[] sourceB[0];
                delete[] sourceB;

                delete[] result[0];
                delete[] result;
            }
        }
    private:
        Logger::LoggerClass* file;
        bool initialized = false;
        int N, M;
        float** sourceA;
        float** sourceB;
        float** result;
};

#endif