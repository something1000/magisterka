#include "BatchNorm.hpp"
#include <iostream>
#include <omp.h>
#include <cstring>
#include <functional>
#include <cmath>

#define EPS 0.00000001f

void BatchNorm::RunParallel() {

    auto excel = *this->file;
    int warmup = 10;
    int rounds = 1000;

}

void BatchNorm::RunSerial() {

    auto excel = *this->file;
    int warmup = 0;
    int rounds = 1;


    BENCHMARK_STRUCTURE(
        excel,      // name of csv logger
        "Serial",   // name of benchmark
        warmup,     // name of warmup rounds variable
        rounds,     // name of benchmark rounds variable
        ELAPSED,    // variable name to store execution time
        {
            //# mini-batch mean
            for(int i=0; i < C; ++i) {
                for(int j=0; j < N; ++j) {
                    for(int k=0; k < H; ++k) {
                        for(int p=0; p < W; ++p) {
                            mean[i] += input_data[j][i][k][p];
                        }
                    }
                }
                mean[i] = mean[i]/(1.0f*(N*H*W));
            }

            //# mini-batch variance
            //variance = np.mean((arr - mean.reshape((1, 3, 1, 1))) ** 2, axis=(0, 2, 3))
            for(int i=0; i < C; ++i) {
                for(int j=0; j < N; ++j) {
                    for(int k=0; k < H; ++k) {
                        for(int p=0; p < W; ++p) {
                            variance[i] += (input_data[j][i][k][p] - mean[i]) * (input_data[j][i][k][p] - mean[i]);
                            //x_hat[j][i][k][p] = input[j][i][k][p] - mean[i];
                        }
                    }
                }
                variance[i] = variance[i]/(1.0f*(N*H*W));
            }

            float* norm_divider = new float[C];
            for(int i=0; i < C; ++i){
                norm_divider[i] = sqrt(variance[i] + EPS);
            }

            for(int i=0; i < C; ++i) {
                for(int j=0; j < N; ++j) {
                    for(int k=0; k < H; ++k) {
                        for(int p=0; p < W; ++p) {
                            output[j][i][k][p] = gamma[i] * ((input_data[j][i][k][p] - mean[i]) / norm_divider[i]) + beta[i];
                        }
                    }
                }
                variance[i] = variance[i]/(N*H*W);
            }
        }
   )
}

void BatchNorm::Init(Logger::LoggerClass* file) {
    this->file = file;
    N = 2;
    C = 3;
    H = 4;
    W = 4;
    input_data = Create4DArray<float>(N, C, H, W);
    gamma = new float[C];
    beta = new float[C];
    output = Create4DArray<float>(N, C, H, W);
    mean = new float[C]; //Create3DArray<float>(C, H, W);
    variance = new float[C]; //Create3DArray<float>(C, H, W);


    gamma[0] = 0.1f; gamma[1] = 0.2f; gamma[2] = 0.3f;
    beta[0] = 0.01f; beta[1] = 0.02f; beta[2] = 0.03f;
}