#include <iostream>
#include <fstream>
#include <string>
#include <ctime>
#include <iomanip>
#include <sstream>

class Logger {
public:
    Logger(const std::string& filename);

    ~Logger();

    void write(const std::string& message);

private:
    std::ofstream logFile_;
    std::string filename_;
};

