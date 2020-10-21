#ifndef _BATCHNORM_HPP
#define _BATCHNORM_HPP
#include "Benchmark.hpp"
#include "common.hpp"

class BatchNorm : public Benchmark {
    public:
        BatchNorm(std::string name) {
            this->name = name;
        };
        virtual void RunSerial() override;
        virtual void RunParallel() override;
        virtual void Init(Logger::LoggerClass* file) override;
        virtual ~BatchNorm() {
            if(initialized) {
                Free4DArray(input_data);
                Free4DArray(output);
                delete[] mean;
                delete[] variance;
                delete[] gamma;
                delete[] beta;
            }
        }
    private:
        Logger::LoggerClass* file;
        bool initialized = false;
        int N,C,H,W;
        Float4D input_data;
        Float4D output;
        float* mean;
        float* variance;
        float* gamma;
        float* beta;
};

#endif