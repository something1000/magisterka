#include "BatchNorm.hpp"
#include <iostream>
#include <omp.h>
#include <cstring>
#include <functional>
#include <cmath>

#define EPS 0.00000001f

//TODO: check icc vectorization raport
void BatchNorm::RunParallel() {
    auto excel = *this->file;

    float* norm_divider = new float[C];
    BENCHMARK_STRUCTURE(
        excel,      // name of csv logger
        "RunParallel_opt",   // name of benchmark
        warmup,     // name of warmup rounds variable
        rounds,     // name of benchmark rounds variable
        ELAPSED,    // variable name to store execution time
        {
            float* input_raw = input_data[0][0][0];
            //# mini-batch mean
            mpragma(omp parallel)
            {
                mpragma(omp for)
                for(int i=0; i < C; ++i) {
                    variance[i] = 0;
                    norm_divider[i] = 0;
                    mean[i] = 0;
                }

                mpragma(omp for reduction(+:mean[:C]) collapse(2) schedule(static))
                for(int i=0; i < C; ++i) {
                    for(int j=0; j < N; ++j) { //check if N or C should be first
                        for(int k=0; k < H; ++k) {
                            for(int p=0; p < W; ++p) {
                                mean[i] += input_raw[i*H*W + j*C*H*W + k*W + p];
                                //assert(&(input_data[j][i][k][p]) == &(input_raw[i*H*W + j*C*H*W + k*W + p]));
                            }
                        }
                    }
                }

                mpragma(omp for)
                for(int i=0; i < C; ++i) {
                    mean[i] = mean[i] / (1.0f*(N*H*W));
                }

                mpragma(omp for reduction(+:variance[:C]) collapse(2) schedule(static))
                for(int i=0; i < C; ++i) {
                    for(int j=0; j < N; ++j) {
                        for(int k=0; k < H; ++k) {
                            for(int p=0; p < W; ++p) {
                                auto in = input_raw[i*H*W + j*C*H*W + k*W + p];
                                variance[i] += (in - mean[i]) * (in - mean[i]);
                                //assert(&(input_data[j][i][k][p]) == &(input_raw[i*H*W + j*C*H*W + k*W + p]));
                                //x_hat[j][i][k][p] = input[j][i][k][p] - mean[i];
                            }
                        }
                    }
            //      variance[i] = var_i / (1.0f*(N*H*W));
                }

                mpragma(omp for)
                for(int i=0; i < C; ++i) {
                    variance[i] = variance[i] / (1.0f*(N*H*W));
                    norm_divider[i] = sqrt(variance[i] + EPS);
                }

                mpragma(omp for schedule(static) collapse(2))
                for(int i=0; i < C; ++i) {
                    for(int j=0; j < N; ++j) {
                        for(int k=0; k < H; ++k) {
                            for(int p=0; p < W; ++p) {
                                output[j][i][k][p] = gamma[i] * ((input_data[j][i][k][p] - mean[i]) / norm_divider[i]) + beta[i];
                            }
                        }
                    }
                }
            }
        }
    )
//    for(int i=0; i < N; ++i) {
//        for(int j=0; j < C; ++j) {
//            Print2DArray(output[i][j], H, W);
//            std::cout << std::endl;
//        }
//    }
}




void BatchNorm::RunSerial() {
    auto excel = *this->file;

    float* norm_divider = new float[C];
    BENCHMARK_STRUCTURE(
        excel,      // name of csv logger
        "Serial",   // name of benchmark
        warmup,     // name of warmup rounds variable
        rounds,     // name of benchmark rounds variable
        ELAPSED,    // variable name to store execution time
        {
            float* input_raw = input_data[0][0][0];
            //# mini-batch mean
            for(int i=0; i < C; ++i) {
                variance[i] = 0;
                norm_divider[i] = 0;
                mean[i] = 0;
            }

            for(int j=0; j < N; ++j) {
                for(int i=0; i < C; ++i) {
                    for(int k=0; k < H; ++k) {
                        for(int p=0; p < W; ++p) {
                            mean[i] += input_raw[i*H*W + j*C*H*W + k*W + p];
                            //assert(&(input_data[j][i][k][p]) == &(input_raw[i*H*W + j*C*H*W + k*W + p]));
                        }
                    }
                }
            }

            for(int i=0; i < C; ++i) {
                mean[i] = mean[i] / (1.0f*(N*H*W));
            }
            //# mini-batch variance
            //variance = np.mean((arr - mean.reshape((1, 3, 1, 1))) ** 2, axis=(0, 2, 3))
            for(int j=0; j < N; ++j) {
                for(int i=0; i < C; ++i) {
                    for(int k=0; k < H; ++k) {
                        for(int p=0; p < W; ++p) {
                            auto in = input_raw[i*H*W + j*C*H*W + k*W + p];
                            variance[i] += (in - mean[i]) * (in - mean[i]);
                            //assert(&(input_data[j][i][k][p]) == &(input_raw[i*H*W + j*C*H*W + k*W + p]));
                            //x_hat[j][i][k][p] = input[j][i][k][p] - mean[i];
                        }
                    }
                }
        //      variance[i] = var_i / (1.0f*(N*H*W));
            }

            for(int i=0; i < C; ++i) {
                variance[i] = variance[i] / (1.0f*(N*H*W));
                norm_divider[i] = sqrt(variance[i] + EPS);
            }

            for(int j=0; j < N; ++j) {
                for(int i=0; i < C; ++i) {
                    for(int k=0; k < H; ++k) {
                        for(int p=0; p < W; ++p) {
                            output[j][i][k][p] = gamma[i] * ((input_data[j][i][k][p] - mean[i]) / norm_divider[i]) + beta[i];
                        }
                    }
                }
            }
        }
   )
//     for(int i=0; i < N; ++i) {
//        for(int j=0; j < C; ++j) {
//            Print2DArray(output[i][j], H, W);
//            std::cout << std::endl;
//        }
//    }
}

void BatchNorm::Init(Logger::LoggerClass* file, const rapidjson::Value& properties) {
    this->file = file;
    
    rounds = properties["rounds"].GetInt();
    warmup = properties["warmup"].GetInt();

    N = properties["N"].GetInt();
    C = properties["C"].GetInt();
    H = properties["H"].GetInt();
    W = properties["W"].GetInt();
    Logger::INFO << VAR(N) << VAR(C) << VAR(H) << VAR(W);

    input_data = Create4DArray<float>(N, C, H, W);
    FillRandom4DArray(input_data, N, C, H, W);
    gamma = new float[C];
    beta = new float[C];
    output = Create4DArray<float>(N, C, H, W);
    mean = new float[C]; //Create3DArray<float>(C, H, W);
    variance = new float[C]; //Create3DArray<float>(C, H, W);


    FillRandomArray(gamma, C);
    FillRandomArray(beta, C);
    //gamma[0] = 0.1f; gamma[1] = 0.2f; gamma[2] = 0.3f;
    //beta[0] = 0.01f; beta[1] = 0.02f; beta[2] = 0.03f;
}