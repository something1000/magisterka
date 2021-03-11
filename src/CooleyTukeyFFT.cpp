#include "CooleyTukeyFFT.hpp"
#include <iostream>
#include <omp.h>

#include <math.h>

//TODO add final clause benchmark

#define PI 3.14159265358979323846
using namespace std::complex_literals;

void CTFFT(float* x, std::complex<double>* out, int N, int s){
    unsigned int k;
    std::complex<double> t;
    if (N == 1) {
        out[0] = x[0];
        return;
    }

    CTFFT(x, out, N/2, 2*s);
    CTFFT(x+s, out + N/2, N/2, 2*s);

    for (k = 0; k < N/2; k++) {
        t = out[k];
        out[k] = t + exp(std::complex<double>(0,-2 * PI * k / N)) * out[k + N/2];
        out[k + N/2] = t - exp(std::complex<double>(0,-2 * PI  * k / N)) * out[k + N/2];
    }
}


void CTFFT_Parallel(float* x, std::complex<double>* out, int N, int s){
    std::complex<double> t;

    if (N == 1) {
        out[0] = x[0];
        return;
    }
    
    mpragma(omp task default(none) firstprivate(x, out, N, s)) {
        CTFFT_Parallel(x, out, N/2, 2*s);
    }

    CTFFT_Parallel  (x+s, out + N/2, N/2, 2*s);

    mpragma(omp taskwait)
    for (int k = 0; k < N/2; k++) {
        t = out[k];
        out[k] = out[k] + exp(std::complex<double>(0,-2 * PI * k / N)) * out[k + N/2];
        out[k + N/2] = out[k] - exp(std::complex<double>(0,-2 * PI  * k / N)) * out[k + N/2];
    }
}




void CTFFT_Parallel_final(float* x, std::complex<double>* out, int N, int nodes){
    std::complex<double> t;

    if (N == 1) {
        out[0] = x[0];
        return;
    }

    // Cooley-Tukey: recursively split in two, then combine beneath.
    int nthreads = omp_get_num_threads();

    mpragma(omp task default(none) firstprivate(x, out, N, nodes) final(nodes >= nthreads)) {
        CTFFT_Parallel_final(x, out, N/2, 2*nodes);
    }
    
    mpragma(omp task default(none) firstprivate(x, out, N, nodes) final(nodes >= nthreads)) {
        CTFFT_Parallel_final(x+nodes, out + N/2, N/2, 2*nodes + 1);
    }
    mpragma(omp taskwait)

    for (int k = 0; k < N/2; k++) {
        t = out[k];
        out[k] = out[k] + exp(std::complex<double>(0,-2 * PI * k / N)) * out[k + N/2];
        out[k + N/2] = out[k] - exp(std::complex<double>(0,-2 * PI  * k / N)) * out[k + N/2];
    }
}


void CTFFT_Parallel_single(float* x, std::complex<double>* out, int N, int nodes){
    std::complex<double> t;

    if (N == 1) {
        out[0] = x[0];
        return;
    }

    int nthreads = omp_get_num_threads();


    mpragma(omp task default(none) firstprivate(x, out, N, nodes) final(nodes >= nthreads)) {
        CTFFT_Parallel_single(x, out, N/2, 2*nodes);
    }
    CTFFT_Parallel_single(x+nodes, out + N/2, N/2, 2*nodes + 1);
    
    mpragma(omp taskwait)

    for (int k = 0; k < N/2; k++) {
        t = out[k];
        out[k] = out[k] + exp(std::complex<double>(0,-2 * PI * k / N)) * out[k + N/2];
        out[k + N/2] = out[k] - exp(std::complex<double>(0,-2 * PI  * k / N)) * out[k + N/2];
    }
}

void CooleyTukeyFFT::RunParallel() {
    RunParallel_Bad();
    RunParallel_Final();
    RunParallel_Single();
}

void CooleyTukeyFFT::RunParallel_Bad() {
    auto excel = *this->file;


    BENCHMARK_STRUCTURE(
        excel,      // name of csv logger
        "Parallel_Bad",   // name of benchmark
        warmup,     // name of warmup rounds variable
        rounds,     // name of benchmark rounds variable
        ELAPSED,    // variable name to store execution time
        {
            mpragma(omp parallel shared(input, output, size)) {
                mpragma(omp single) {
                     CTFFT_Parallel(input, output, size, 1);
                }
            }
        }
   )
    // for(int i=0;i < size;i++) {
    //     std::cout << output[i] << std::endl;
    // }
}

void CooleyTukeyFFT::RunParallel_Final() {
    auto excel = *this->file;


    BENCHMARK_STRUCTURE(
        excel,      // name of csv logger
        "Parallel_Final",   // name of benchmark
        warmup,     // name of warmup rounds variable
        rounds,     // name of benchmark rounds variable
        ELAPSED,    // variable name to store execution time
        {
            mpragma(omp parallel shared(input, output, size)) {
                mpragma(omp single) {
                     CTFFT_Parallel_final(input, output, size, 1);
                }
            }
        }
   )
   
    // for(int i=0;i < size;i++) {
    //     std::cout << output[i] << std::endl;
    // }
}


void CooleyTukeyFFT::RunParallel_Single() {
    auto excel = *this->file;


    BENCHMARK_STRUCTURE(
        excel,      // name of csv logger
        "Parallel_Single",   // name of benchmark
        warmup,     // name of warmup rounds variable
        rounds,     // name of benchmark rounds variable
        ELAPSED,    // variable name to store execution time
        {
            mpragma(omp parallel shared(input, output, size)) {
                mpragma(omp single) {
                     CTFFT_Parallel_single(input, output, size, 1);
                }
            }
        }
   )

    // for(int i=0;i < size;i++) {
    //     std::cout << output[i] << std::endl;
    // }
}

void CooleyTukeyFFT::RunSerial() {
    auto excel = *this->file;

    BENCHMARK_STRUCTURE(
        excel,      // name of csv logger
        "Serial",   // name of benchmark
        warmup,     // name of warmup rounds variable
        rounds,     // name of benchmark rounds variable
        ELAPSED,    // variable name to store execution time
        {
            CTFFT(input, output, size, 1);
        }
   )
}

void CooleyTukeyFFT::Init(Logger::LoggerClass* file, const rapidjson::Value& properties) {
    this->file = file;
    
    rounds = properties["rounds"].GetInt();
    warmup = properties["warmup"].GetInt();
    
    size = properties["size"].GetInt();//128;

    Logger::INFO << VAR(size);

    input = new float[size];

    for(int i=0;i < size;i++) {
        input[i] = 2*sin(i) + cos(i*2);
    }
    output = new std::complex<double>[size];

    this->initialized = true;

}