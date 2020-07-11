#ifndef _LOGGER_HPP
#define _LOGGER_HPP
#include "common.hpp"
#include <vector>

//Logger::INFO
namespace Logger{

enum LogLevel {
    INFO = 0,
    WARNING = 1,
    ERROR = 2,
    EXCEL = 3
};

class LoggerClass {
    public:
        LoggerClass(LogLevel loglvl, std::ostream& stream)
            : stream(stream), level(loglvl)
        {

            if(loglvl == LogLevel::EXCEL)
                return;
            stream << "[" << this->getHeader(loglvl) << "] ";
        }

        template<class T>
        LoggerClass& operator<<(const T& msg){
            stream << msg;
            if(level == LogLevel::EXCEL)
                stream << ";";
            return *this;
        }
        ~LoggerClass() {
            stream << "\n";
        }

        static LoggerClass getExcelLog(std::ostream& s){
            return LoggerClass(LogLevel::EXCEL, s);
        }
    private:
        inline std::string getHeader(LogLevel loglvl) {
            static std::vector<std::string> lvl_text = {"INFO", "WARN", "ERROR"};
            return lvl_text.at(loglvl);
        }
        int level;
        std::ostream& stream;
};

// static void _Log(LogLevel l) {

// }

#define INFO LoggerClass(Logger::INFO, std::cout)
#define WARN LoggerClass(Logger::INFO, std::cout)
#define ERROR LoggerClass(Logger::INFO, std::cout)
#define EXCEL(fstream) LoggerClass::getExcelLog(fstream)
}



#endif