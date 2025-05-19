#include "TokenScanner.h"

TokenScanner::TokenScanner() {
    tokenPatterns.emplace_back(R"(sk-[a-zA-Z0-9]{20,})");
    tokenPatterns.emplace_back(R"(pk-[a-zA-Z0-9]{20,})");
    tokenPatterns.emplace_back(R"(Bearer\s+[a-zA-Z0-9\-_\.=]+)");
}

std::vector<std::string> TokenScanner::Scan(const std::string& body) {
    std::vector<std::string> matches;
    for (const auto& re : tokenPatterns) {
        auto begin = std::sregex_iterator(body.begin(), body.end(), re);
        auto end = std::sregex_iterator();
        for (auto it = begin; it != end; ++it) {
            matches.push_back(it->str());
        }
    }
    return matches;
}
