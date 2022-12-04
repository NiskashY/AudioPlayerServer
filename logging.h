#pragma once

#include <fstream>
#include <iostream>
#include <ctime>
#include <chrono>

class Log {
public:
    explicit Log(const std::string &kFileName = "logs.txt");

    bool getIsOk() const;

    template <class T>
    void Write(const T& kMsg) {
        if (isOk) {
            out << kMsg << '\n';
        } else {
            std::cerr << "Error occurred: file not opeN" << '\n';
        }
    }

    template <class T, class U>
    void Write(const T& kMsg, const U& kArg) {
        if (isOk) {
            out << kMsg << ' ' << kArg << '\n';
        } else {
            std::cerr << "Error occurred: file not opeN" << '\n';
        }
    }

    ~Log();

private:
    bool isOk = false;
    std::ofstream out;

    char* GetCurrentTime();
};
