#pragma once
#include <string>
#include <vector>
#include <regex>

class TokenScanner {
public:
    TokenScanner();
    std::vector<std::string> Scan(const std::string& body);

private:
    std::vector<std::regex> tokenPatterns;
};
