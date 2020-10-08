#ifndef _QUICKSORT_HPP
#define _QUICKSORT_HPP
#include "Benchmark.hpp"
#include "common.hpp"

class QuickSort : public Benchmark {
    public:
        QuickSort(std::string name) {
            this->name = name;
        };
        virtual void RunSerial() override;
        virtual void RunParallel() override;
        virtual void Init(Logger::LoggerClass* file) override;
        virtual ~QuickSort() {
            if(initialized) {
                delete[] input_data;
                delete[] data;
            }
        }
    private:
        Logger::LoggerClass* file;
        bool initialized = false;
        int size;
        float* input_data;  // random data
        float* data;        // copy input data into new memory as quicksort is inplace
};

#endif