#include "Convolution2D.hpp"
#include <iostream>
#include <omp.h>
#include <cstring>

void Convolution2D::RunParallel() {
    RunParallel_1();
    RunParallel_2();
}

void Convolution2D::RunParallel_1() {
    auto excel = *this->file;

    auto fn = [&]() {
        int _N = N;
        int _H = H;
        int _W = W;
        int _kernel = kernel;
        int kernel_center = kernel/2;
        int rH = (H - 2 * kernel_center);
        int rW = (W - 2 * kernel_center);
        int res_size = N * rH * rW;
        float* raw_input  = input_data[0][0];
        float* raw_result = result[0][0];
        float* raw_kernel = kernel_data[0];

        #pragma omp target teams distribute parallel for collapse(3) \
                map(to:raw_input[0:_N*_H*_W]) map(to:raw_kernel[0:_kernel*_kernel]) \
                map(tofrom:raw_result[0:res_size])
            for(int batch=0; batch < _N; ++batch) {

                for(int y=kernel_center; y < _H - kernel_center; ++y) {
                    for(int x=kernel_center; x < _W - kernel_center; ++x) {
                        raw_result[batch*rH*rW + (y-kernel_center) * rW + x-kernel_center] += 0.0f;
                        for(int ky=0; ky < _kernel; ++ky) {
                            for(int kx=0; kx < _kernel; ++kx) {

                                int input_index_y = y + (kernel_center - ky);
                                int input_index_x = x + (kernel_center - kx);
                                raw_result[batch*rH*rW + (y-kernel_center) * rW + x-kernel_center] +=
                                        raw_input[batch*_H*_W + input_index_y * _W + input_index_x] * raw_kernel[ky * _kernel + kx];
                                //result[batch][y-kernel_center][x-kernel_center] += input_data[batch][input_index_y][input_index_x] * kernel_data[ky][kx];
                            }
                        }
                    }
                }
            }
    };

    BenchmarkIt(excel, "Parallel_Collapse_3", warmup, rounds, fn);
}


void Convolution2D::RunParallel_2() {
    auto excel = *this->file;

    auto fn = [&]() {
        int _N = N;
        int _H = H;
        int _W = W;
        int _kernel = kernel;
        int kernel_center = kernel/2;
        int rH = (H - 2 * kernel_center);
        int rW = (W - 2 * kernel_center);
        int res_size = N * rH * rW;
        float* raw_input  = input_data[0][0];
        float* raw_result = result[0][0];
        float* raw_kernel = kernel_data[0];

        #pragma omp target teams distribute parallel for collapse(2) \
                map(to:raw_input[0:_N*_H*_W]) map(to:raw_kernel[0:_kernel*_kernel]) \
                map(tofrom:raw_result[0:res_size])
            for(int batch=0; batch < _N; ++batch) {

                for(int y=kernel_center; y < _H - kernel_center; ++y) {
                    for(int x=kernel_center; x < _W - kernel_center; ++x) {
                        raw_result[batch*rH*rW + (y-kernel_center) * rW + x-kernel_center] += 0.0f;
                        for(int ky=0; ky < _kernel; ++ky) {
                            for(int kx=0; kx < _kernel; ++kx) {

                                int input_index_y = y + (kernel_center - ky);
                                int input_index_x = x + (kernel_center - kx);
                                raw_result[batch*rH*rW + (y-kernel_center) * rW + x-kernel_center] +=
                                        raw_input[batch*_H*_W + input_index_y * _W + input_index_x] * raw_kernel[ky * _kernel + kx];
                                //result[batch][y-kernel_center][x-kernel_center] += input_data[batch][input_index_y][input_index_x] * kernel_data[ky][kx];
                            }
                        }
                    }
                }
            }
    };

    BenchmarkIt(excel, "Parallel_Collapse_2", warmup, rounds, fn);
}

void Convolution2D::RunSerial() {
    auto excel = *this->file;

    auto fn = [&]() {
        int _N = N;
        int _H = H;
        int _W = W;
        int _kernel = kernel;
        int kernel_center = kernel/2;
        int rH = (H - 2 * kernel_center);
        int rW = (W - 2 * kernel_center);
        int res_size = N * rH * rW;
        float* raw_input  = input_data[0][0];
        float* raw_result = result[0][0];
        float* raw_kernel = kernel_data[0];

        for(int batch=0; batch < _N; ++batch) {

            for(int y=kernel_center; y < _H - kernel_center; ++y) {
                for(int x=kernel_center; x < _W - kernel_center; ++x) {
                    raw_result[batch*rH*rW + (y-kernel_center) * rW + x-kernel_center] += 0.0f;
                    for(int ky=0; ky < _kernel; ++ky) {
                        for(int kx=0; kx < _kernel; ++kx) {

                            int input_index_y = y + (kernel_center - ky);
                            int input_index_x = x + (kernel_center - kx);
                            raw_result[batch*rH*rW + (y-kernel_center) * rW + x-kernel_center] +=
                                    raw_input[batch*_H*_W + input_index_y * _W + input_index_x] * raw_kernel[ky * _kernel + kx];
                            //result[batch][y-kernel_center][x-kernel_center] += input_data[batch][input_index_y][input_index_x] * kernel_data[ky][kx];
                        }
                    }
                }
            }
        }
    };
       
    BenchmarkIt(excel, "Serial", warmup, rounds, fn);
}


bool Convolution2D::Validate() {
    const int kernel_center = kernel / 2;
    const int res_rows = H - 2 * kernel_center;
    const int res_cols = W - 2 * kernel_center;
    Tensor3D<float> out_serial = Create3DArray<float>(N, res_rows, res_cols);
    Tensor3D<float> out_parallel_1 = Create3DArray<float>(N, res_rows, res_cols);
    Tensor3D<float> out_parallel_2 = Create3DArray<float>(N, res_rows, res_cols);
    rounds = 1;
    warmup = 0;

    Tensor3D<float> tmp = result;

    result = out_serial;
    RunSerial();

    result = out_parallel_1;
    RunParallel_1();

    result = out_parallel_2;
    RunParallel_2();

    bool is_valid = Compare3DArray(out_serial, out_parallel_1, N, res_rows, res_cols);
    is_valid = is_valid && Compare3DArray(out_serial, out_parallel_2, N, res_rows, res_cols);

    result = tmp;
    Free3DArray<float>(out_serial);
    Free3DArray<float>(out_parallel_1);
    Free3DArray<float>(out_parallel_2);
    return is_valid;
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
    Reinitialize();
}

void Convolution2D::Reinitialize() {
    if(initialized) {
        Free3DArray<float>(input_data);
        Free2DArray<float>(kernel_data);
        Free3DArray<float>(result);
    }

    int kernel_center = kernel / 2;
    int res_rows = H - 2 * kernel_center;
    int res_cols = W - 2 * kernel_center;

    input_data = Create3DArray<float>(N, H, W);
    kernel_data = Create2DArray<float>(kernel, kernel);
    result  = Create3DArray<float>(N, res_rows, res_cols);

    FillRandom3DArray(input_data, N, H, W);
    FillRandom2DArray(kernel_data, kernel, kernel);
    float* raw_result_ptr = result[0][0];
    std::memset(raw_result_ptr, 0, N * res_rows * res_cols * sizeof(float));

    this->initialized = true;
}