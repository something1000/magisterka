#ifndef _QUANTIZETENSOR_HPP
#define _QUANTIZETENSOR_HPP
#include "Benchmark.hpp"
#include "common.hpp"

class QuantizeTensor : public Benchmark {
    public:
        QuantizeTensor(std::string name) {
            this->name = name;
        };
        virtual void RunSerial() override;
        virtual void RunParallel() override;
        virtual void Init(Logger::LoggerClass* file) override;
        virtual ~QuantizeTensor() {
            if(initialized) {
                delete[] input;
                delete[] output;
            }
        }
    private:
        Logger::LoggerClass* file;
        bool initialized = false;
        int N,C,H,W;
        float scale;
        int zero_position;
        Tensor4D<float> input;
        Tensor4D<int8_t> output;
};

#endif