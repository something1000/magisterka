#ifndef _CONVOLUTION2D_HPP
#define _CONVOLUTION2D_HPP
#include "Benchmark.hpp"
#include "common.hpp"

class Convolution2D : public Benchmark {
    public:
        Convolution2D(std::string name) {
            this->name = name;
        };
        virtual void RunSerial() override;
        virtual void RunParallel() override;
        virtual void Init(Logger::LoggerClass* file) override;
        virtual ~Convolution2D() {
            if(initialized) {
                delete[] input_data[0][0];
                delete[] input_data[0];
                delete[] input_data;

                delete[] kernel_data[0];
                delete[] kernel_data;

                delete[] result[0][0];
                delete[] result[0];
                delete[] result;
            }
        }
    private:
        Logger::LoggerClass* file;
        bool initialized = false;
        int batch_size, row_size, col_size, kernel_size;
        float*** input_data;
        float** kernel_data;
        float*** result;
};

#endif