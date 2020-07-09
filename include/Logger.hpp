#ifndef _LOGGER_HPP
#define _LOGGER_HPP
#include "common.hpp"
#include <vector>

//Logger::INFO
namespace Logger{

enum LogLevel {
    INFO = 0,
    WARNING = 1,
    ERROR = 2
};

class LoggerClass {
    public:
        LoggerClass(LogLevel loglvl)
        {
            std::cout << "[" << this->getHeader(loglvl) << "] ";
        }

        template<class T>
        LoggerClass& operator<<(const T& msg){
            std::cout << msg;
            return *this;
        }
        ~LoggerClass() {
            std::cout << "\n";
        }
    private:
        inline std::string getHeader(LogLevel loglvl) {
            static std::vector<std::string> lvl_text = {"INFO", "WARN", "ERROR"};
            return lvl_text.at(loglvl);
    }
};

// static void _Log(LogLevel l) {

// }

#define INFO LoggerClass(Logger::INFO)
#define WARN LoggerClass(Logger::INFO)
#define ERROR LoggerClass(Logger::INFO)
}



#endif