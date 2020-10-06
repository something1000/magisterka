#include "Convolution2D.hpp"
#include <iostream>
#include <omp.h>
#include <cstring>

void Convolution2D::RunParallel() {

}


void Convolution2D::RunSerial() {

    auto excel = *this->file;
    int warmup = 10;
    int rounds = 1000;
    const int row_size = this->row_size;
    const int col_size = this->col_size;
    const int kernel_size = this->kernel_size;
    const int kernel_center = kernel_size/2;

    BENCHMARK_STRUCTURE(
        excel,      // name of csv logger
        "Serial",   // name of benchmark
        warmup,     // name of warmup rounds variable
        rounds,     // name of benchmark rounds variable
        ELAPSED,    // variable name to store execution time
        {

            for(int batch=0; batch < batch_size; ++batch) {

                for(int y=kernel_center; y < row_size - kernel_center; ++y) {
                    for(int x=kernel_center; x < col_size - kernel_center; ++x) {
                        result[batch][y-kernel_center][x-kernel_center] = 0.0f;
                        for(int ky=0; ky < kernel_size; ++ky) {
                            for(int kx=0; kx < kernel_size; ++kx) {

                                int input_index_y = y + (kernel_center - ky);
                                int input_index_x = x + (kernel_center - kx);
                                result[batch][y-kernel_center][x-kernel_center] += input_data[batch][input_index_y][input_index_x] * kernel_data[ky][kx];
                            }
                        }
                    }
                }
            }
       }
   )
}

void Convolution2D::Init(Logger::LoggerClass* file) {
    this->file = file;
    
    batch_size = 32;
    row_size = 64;
    col_size = 32;
    kernel_size = 3;
    int kernel_center = kernel_size / 2;

    input_data = Create3DArray<float>(batch_size, row_size, col_size);
    FillRandom3DArray(input_data, batch_size, row_size, col_size);

    kernel_data = Create2DArray<float>(kernel_size, kernel_size);
    kernel_data[0][0] = 0; kernel_data[0][1] = 0; kernel_data[0][2] = 0;
    kernel_data[1][0] = 1; kernel_data[1][1] = 1; kernel_data[1][2] = 1;
    kernel_data[2][0] = 0; kernel_data[2][1] = 0; kernel_data[2][2] = 0;

    int res_rows = row_size - 2 * kernel_center;
    int res_cols = col_size - 2 * kernel_center;
    result  = Create3DArray<float>(batch_size, res_rows, res_cols);
    std::memset(result, batch_size * res_rows * res_cols, 0);

    //FillRandom2DArray(kernel_data, kernel_size, kernel_size);
    this->initialized = true;
}