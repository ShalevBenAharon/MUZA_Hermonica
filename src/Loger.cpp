#include "Loger.hpp"

Logger::Logger(const std::string& filename = "Musa_log.txt") : filename_(filename) {
        logFile_.open(filename_, std::ios::app);
        if (!logFile_.is_open()) {
            std::cerr << "Error: Could not open log file: " << filename_ << std::endl;
        }
    }

Logger::~Logger() {
    if (logFile_.is_open()) {
        logFile_.close();
    }
}

void Logger::write (const std::string& message) {
    if (logFile_.is_open()) {
        std::time_t now = std::time(nullptr);
        std::tm tm_struct;
        #ifdef _WIN32
            localtime_s(&tm_struct, &now);
        #else
            localtime_r(&now, &tm_struct);
        #endif

        std::stringstream ss;
        ss << std::put_time(&tm_struct, "%Y-%m-%d %H:%M:%S");
        std::string timestamp = ss.str();

        logFile_ << timestamp << " " << message << std::endl;
    } else {
        std::cerr << "Error: Log file is not open." << std::endl;
    }
}

