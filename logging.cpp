#include "logging.h"

Log::Log(const std::string &kFileName) {
    out.open(kFileName, std::ios::app);
    isOk = out.is_open();
    if (isOk) {
        out << '\n' << GetCurrentTime();
    }
}

Log::~Log() {
    out.close();
}

bool Log::getIsOk() const  {
    return isOk;
}

char* Log::GetCurrentTime() {
    auto current_seconds = std::chrono::system_clock::now();
    std::time_t current_time = std::chrono::system_clock::to_time_t(current_seconds);

    return std::ctime(&current_time);
}
