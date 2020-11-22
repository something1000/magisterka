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
        virtual void RunParallel() override;
        virtual void Init(Logger::LoggerClass* file) override;
        virtual ~WaveEquation() {
            if(initialized) {
                Free3DArray<double>(wave);
            }
        }
    private:
        Logger::LoggerClass* file;
        bool initialized = false;
        
        double v, a, b, q, w;
        int M, N, K, beta;
        double dx, dy, dt, px, py;
        Tensor3D<double> wave;
};

#endif