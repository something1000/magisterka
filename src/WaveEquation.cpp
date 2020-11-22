#include "WaveEquation.hpp"
#include <iostream>
#include <omp.h>
#include <tuple>
#include <math.h>
#include <cstring>

void WaveEquation::RunParallel() {

    auto excel = *this->file;
    int warmup = 0;
    int rounds = 1;

    BENCHMARK_STRUCTURE(
        excel,      // name of csv logger
        "Parallel",   // name of benchmark
        warmup,     // name of warmup rounds variable
        rounds,     // name of benchmark rounds variable
        ELAPSED,    // variable name to store execution time
        {
            mpragma(omp parallel) {
                mpragma(omp single) {
                    //  _CTFFT_Parallel(input, output, size, 1);
                    int i =0;
                }
            }
        }
   )

}

void WaveEquation::RunSerial() {

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
            int i = 0;
        }
   )
    for(int i=0; i < M; ++i) {
        for(int j=0; j < N; ++j) {
            std::cout << wave[0][i][j] << " ";
        }
        std::cout << "\n";
    }
}

void WaveEquation::Init(Logger::LoggerClass* file) {
    this->file = file;


    v = 100;
    a = 1.2;
    b = 0.8;
    M = 10;
    N = 6;
    K = 20;
    double* x = new double[M]; //linspace(0,a,M); // utworz M elementów od 0 do a z równym odstępem
    double* y = new double[M]; //linspace(0,b,N); // utworz N elementów od 0 do b z równym odstępem

    auto linspace = [=](double* arr, double l, double r, int size) {
        const double delta = (r - l) / (size - 1);
        for(int i=0; i < size; ++i) {
            arr[i] = l + delta * i;
        }
    };

    linspace(x, 0, a, M);
    linspace(y, 0, b, N);



    dx = x[2]-x[1];
    dy = y[2]-y[1];
    dt = dx*dy / v / (dx+dy);
    px = (v*dt/dx) * (v*dt/dx);
    py = (v*dt/dy) * (v*dt/dy);

    beta=20;
    q=1+beta*dt;
    w=1-beta*dt;

    auto meshgrid = [=](double* arr_1, int size_arr_1, double* arr_2, int size_arr_2) {

        //Tensor2D<double> ret_1 = Create2DArray<double>(size_arr_2, size_arr_1);
        //Tensor2D<double> ret_2 = Create2DArray<double>(size_arr_2, size_arr_1);
        Tensor2D<double> ret_1 = Create2DArray<double>(size_arr_1, size_arr_2); // transpose /\'
        Tensor2D<double> ret_2 = Create2DArray<double>(size_arr_1, size_arr_2); //transpose /\'
        for(int i=0; i < size_arr_1; ++i) {
            for(int j=0; j < size_arr_2; ++j) {
                ret_1[i][j] = arr_1[i];
                ret_2[i][j] = arr_2[j];
            }
        }
        return std::tuple<Tensor2D<double>, Tensor2D<double>>(ret_1, ret_2);
    };

    auto meshgrid_XY = meshgrid(x, M, y, N);
    auto X = std::get<0>(meshgrid_XY);
    auto Y = std::get<1>(meshgrid_XY);

    std::cout << "\n\nmeshgrid x\n";
    
    for(int i=0; i < M; ++i) {
        for(int j=0; j < N; ++j) {
            std::cout << X[i][j] << " ";
        }
        std::cout << "\n";
    }



    std::cout << "\n\nmeshgrid y\n";
    for(int i=0; i < M; ++i) {
        for(int j=0; j < N; ++j) {
            std::cout << Y[i][j] << " ";
        }
        std::cout << "\n";
    }
    std::cout << "\n\n\n\n\n";
    //[X,Y]=meshgrid(x,y); // z vectorów x i y stworz kombinacje: wektor X powtorzony len(y) razy w pionie - wektor y powtorzony len(x) razy w poziomie
    //X=X'; // transpozycja
    //Y=Y'; // transpozycja
    
    //s=X.^2.*(a-X).*Y.^2.*(b-Y);
    Tensor2D<double> s = Create2DArray<double>(M, N);
    std::memset(s[0], 0, M*N*sizeof(double));
    std::cout << "\n\nZERO S y\n";
    for(int i=0; i < M; ++i) {
        for(int j=0; j < N; ++j) {
            std::cout << s[i][j] << " ";
        }
        std::cout << "\n";
    }

        std::cout << "\n";
        std::cout << "\n";
    for(int i=0; i < M; ++i) {
        for(int j=0; j < N; ++j) { //elemwise
            // s    =        X.^2.       *     (a-X).     *         Y.^2.       *     (b-Y);
            s[i][j] = (X[i][j] * X[i][j]) * (a - X[i][j]) * (Y[i][j] * Y[i][j]) * (b - Y[i][j]);
        }
    }


    std::cout << "\n\nS\n";
    for(int i=0; i < M; ++i) {
        for(int j=0; j < N; ++j) {
            std::cout << s[i][j] << " ";
        }
        std::cout << "\n";
    }
    //g = Create2DArray(M, N);// zeros(M,N);
    // d = Create3DArray(1, N, K);//  d=zeros(1,N,K);
    // r = Create3DArray(M, 1, K);//  r=zeros(M,1,K);
    // u = Create3DArray(1, N, K);//  u=zeros(1,N,K);
    // l = Create3DArray(M, 1, K);//  l=zeros(M,1,K);;
    wave = Create3DArray<double>(K, M, N);//  f=zeros(M,N,K)

    // memset(g[0],    0, M * K *sizeof(double));
    // memset(d[0][0], 0, N * K *sizeof(double));
    // memset(r[0][0], 0, M * K *sizeof(double));
    // memset(u[0][0], 0, N * K *sizeof(double));
    // memset(l[0][0], 0, M * K *sizeof(double));
    //memset(f[0][0], 0, M * N * K *sizeof(double)); // tylko
    // f(M,:,:)=d(1,:,:); f(:,N,:)=r(:,1,:); f(1,:,:)=u(1,:,:); f(:,1,:)=l(:,1,:);
    //memset(f[0][0], 0, M * N * K *sizeof(double)); // tylko to potrzebne - d r u l nie uzywane w obliczeniach

        std::cout << "\n";
        std::cout << "\n";
    for(int i=0; i < M; i++) {
        for(int j=0; j < N; j++) {
            wave[0][i][j] = s[i][j];
        }
    }

    //f(2:M-1,2:N-1,2)=(1-px-py)*f(2:M-1,2:N-1,1)+px*(f(3:M,2:N-1,1)+f(1:M-2,2:N-1,1))/2
    //    +py*(f(2:M-1,3:N,1)+f(2:M-1,1:N-2,1))/2 + w*dt*g(2:M-1,2:N-1);

    for(int i=1; i < M-1; i++) {
        for(int j=1; j < N-1; j++) {
            wave[1][i][j] = (1 - px - py) * wave[0][i][j]               // (1-px-py)*f(2:M-1,2:N-1,1)
                          + px * (wave[0][i+1][j] + wave[0][i-1][j])/2  // px*(f(3:M,2:N-1,1)+f(1:M-2,2:N-1,1))/2
                          + py * (wave[0][i][j+1] + wave[0][i][j-1])/2; // py*(f(2:M-1,3:N,1)+f(2:M-1,1:N-2,1))/2
        }
    }


    for(int k=1; k < K-1; k++) {
        for(int i=1; i < M-1; i++) {
            for(int j=1; j < N-1; j++) {
                wave[k+1][i][j] = 2.0 / q * (1 - px - py)* wave[k][i][j]      // 2/q*(1-px-py)*f(2:M-1,2:N-1,k)
                                + px * (wave[k][i+1][j] + wave[k][i-1][j])/q  // px*(f(3:M,2:N-1,k)+f(1:M-2,2:N-1,k))/q
                                + py * (wave[k][i][j+1] + wave[k][i][j-1])/q  // py*( f(2:M-1,3:N,k)+f(2:M-1,1:N-2,k))/q
                                 - w*wave[k-1][i][j]/q;                        // w*f(2:M-1,2:N-1,k-1)/q
            }
        }
    }

// f(2:M-1,2:N-1,2)=(1-px-py)*f(2:M-1,2:N-1,1)+px*(f(3:M,2:N-1,1)+f(1:M-2,2:N-1,1))/2+py*(f(2:M-1,3:N,1)+f(2:M-1,1:N-2,1))/2 + w*dt*g(2:M-1,2:N-1);

// for k=2:K-1
//     f(2:M-1,2:N-1,k+1)=2/q*(1-px-py)*f(2:M-1,2:N-1,k)+px*(f(3:M,2:N-1,k)+f(1:M-2,2:N-1,k))/q + py*( f(2:M-1,3:N,k)+f(2:M-1,1:N-2,k))/q-w*f(2:M-1,2:N-1,k-1)/q;
// end
    
    for(int k=0; k < K; k++) {
            std::cout << "\n";
        std::cout << "WAVE " << k << "\n";
        for(int i=0; i < M; ++i) {
            for(int j=0; j < N; ++j) {
                std::cout << wave[k][i][j] << " ";
            }
            std::cout << "\n";
        }
    }

    this->initialized = true;
}