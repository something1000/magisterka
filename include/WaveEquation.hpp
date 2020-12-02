#ifndef _WAVEEQUATION_HPP
#define _WAVEEQUATION_HPP
#include "Benchmark.hpp"
#include "common.hpp"
#include <complex>

class WaveEquation : public Benchmark {
    public:
        WaveEquation(std::string name) {
            this->name = name;
        };
        virtual void RunSerial() override;
        virtual void RunParallel_1();
        virtual void RunParallel_2();
        virtual void RunParallel() override;
        virtual void Init(Logger::LoggerClass* file) override;
        virtual ~WaveEquation() {
            if(initialized) {
                Free3DArray<double>(waves);
            }
        }
    private:
        Logger::LoggerClass* file;
        bool initialized = false;
        
        double v, a, b, q, w;
        int M, N, K, beta;
        double dx, dy, dt, px, py;

        Tensor3D<double> waves;
};

#endif