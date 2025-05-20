#include "LogSystem.h"

void LogSystem::AddLog(const std::string& msg) {
    std::lock_guard<std::mutex> lock(mtx);
    if (logs.size() >= maxSize) logs.erase(logs.begin());
    logs.push_back(msg);
}

std::vector<std::string> LogSystem::GetRecentLogs() {
    std::lock_guard<std::mutex> lock(mtx);
    return logs;
}
