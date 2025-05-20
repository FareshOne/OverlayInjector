#pragma once
#include <string>
#include <vector>
#include <mutex>

class LogSystem {
public:
    void AddLog(const std::string& msg);
    std::vector<std::string> GetRecentLogs();

private:
    std::vector<std::string> logs;
    std::mutex mtx;
    const size_t maxSize = 100;
};
