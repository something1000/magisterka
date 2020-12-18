#include "Convolution2D.hpp"
#include <iostream>
#include <omp.h>
#include <cstring>

void Convolution2D::RunParallel() {
    auto excel = *this->file;

    const int H = this->H;
    const int W = this->W;
    const int kernel = this->kernel;
    const int kernel_center = kernel/2;

    {
        BENCHMARK_STRUCTURE(
            excel,      // name of csv logger
            "Parallel_Collapse_3",   // name of benchmark
            warmup,     // name of warmup rounds variable
            rounds,     // name of benchmark rounds variable
            ELAPSED_2,    // variable name to store execution time
            {

                mpragma(omp parallel for collapse(3))
                for(int batch=0; batch < N; ++batch) {

                    for(int y=kernel_center; y < H - kernel_center; ++y) {
                        for(int x=kernel_center; x < W - kernel_center; ++x) {
                            result[batch][y-kernel_center][x-kernel_center] = 0.0f;
                            for(int ky=0; ky < kernel; ++ky) {
                                for(int kx=0; kx < kernel; ++kx) {

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


    //TODO rozdzielic na dwie funkcje
    {
        BENCHMARK_STRUCTURE(
            excel,      // name of csv logger
            "Parallel_Collapse_2",   // name of benchmark
            warmup,     // name of warmup rounds variable
            rounds,     // name of benchmark rounds variable
            ELAPSED_3,    // variable name to store execution time
            {

                mpragma(omp parallel for collapse(2))
                for(int batch=0; batch < N; ++batch) {

                    for(int y=kernel_center; y < H - kernel_center; ++y) {
                        for(int x=kernel_center; x < W - kernel_center; ++x) {
                            result[batch][y-kernel_center][x-kernel_center] = 0.0f;
                            for(int ky=0; ky < kernel; ++ky) {
                                for(int kx=0; kx < kernel; ++kx) {

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
}


void Convolution2D::RunSerial() {
    auto excel = *this->file;

    const int H = this->H;
    const int W = this->W;
    const int kernel = this->kernel;
    const int kernel_center = kernel/2;

    BENCHMARK_STRUCTURE(
        excel,      // name of csv logger
        "Serial",   // name of benchmark
        warmup,     // name of warmup rounds variable
        rounds,     // name of benchmark rounds variable
        ELAPSED,    // variable name to store execution time
        {

            for(int batch=0; batch < N; ++batch) {

                for(int y=kernel_center; y < H - kernel_center; ++y) {
                    for(int x=kernel_center; x < W - kernel_center; ++x) {
                        result[batch][y-kernel_center][x-kernel_center] = 0.0f;
                        for(int ky=0; ky < kernel; ++ky) {
                            for(int kx=0; kx < kernel; ++kx) {

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

void Convolution2D::Init(Logger::LoggerClass* file, const rapidjson::Value& properties) {
    this->file = file;

    rounds = properties["rounds"].GetInt();
    warmup = properties["warmup"].GetInt();

    N = properties["N"].GetInt();
    H = properties["H"].GetInt();
    W = properties["W"].GetInt();
    kernel = properties["kernel"].GetInt();

    Logger::INFO << VAR(N) << VAR(H) << VAR(W) << VAR(kernel);

    int kernel_center = kernel / 2;

    input_data = Create3DArray<float>(N, H, W);
    FillRandom3DArray(input_data, N, H, W);

    kernel_data = Create2DArray<float>(kernel, kernel);
    //TODO: randomize?
    kernel_data[0][0] = 0; kernel_data[0][1] = 0; kernel_data[0][2] = 0;
    kernel_data[1][0] = 1; kernel_data[1][1] = 1; kernel_data[1][2] = 1;
    kernel_data[2][0] = 0; kernel_data[2][1] = 0; kernel_data[2][2] = 0;

    int res_rows = H - 2 * kernel_center;
    int res_cols = W - 2 * kernel_center;
    result  = Create3DArray<float>(N, res_rows, res_cols);
    std::memset(result, N * res_rows * res_cols, 0);

    //FillRandom2DArray(kernel_data, kernel, kernel);
    this->initialized = true;
}