#ifndef _ACTIVATION_HPP
#define _ACTIVATION_HPP
#include "Benchmark.hpp"
#include "common.hpp"

class Activation : public Benchmark {
    public:
        Activation(std::string name) {
            this->name = name;
        };
        virtual void Init(Logger::LoggerClass* file, const rapidjson::Value& properties) override;
        virtual void RunSerial() override;
        virtual void RunParallel() override;
        void RunParallel_1();
        void RunParallel_2();
        void RunParallel_3();
        virtual bool Validate() override;
        virtual ~Activation() {
            if(initialized) {
                delete[] input;
                delete[] output;
            }
        }
    private:
        void Reinitialize();

        Logger::LoggerClass* file;
        int rounds;
        int warmup;
        bool initialized = false;
        int size, static_size;
        float* input;
        float* output;
};

#endif