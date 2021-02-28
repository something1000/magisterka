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
        void RunParallel_1();
        void RunParallel_2();
        void RunParallel_3();
        virtual void Init(Logger::LoggerClass* file, const rapidjson::Value& properties) override;
        virtual ~QuantizeTensor() {
            if(initialized) {
                delete[] input;
                delete[] output;
            }
        }
    private:
        Logger::LoggerClass* file;
        int rounds;
        int warmup;
        bool initialized = false;
        int N,C,H,W;
        int static_size;
        float scale;
        int zero_position;
        Tensor4D<float> input;
        Tensor4D<int8_t> output;
};

#endif