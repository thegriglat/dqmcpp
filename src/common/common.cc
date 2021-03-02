#include "common.hh"
#include <vector>
#include <string>

std::vector<std::string> split(const std::string str, const std::string delimiter)
{
    size_t pos;
    std::vector<std::string> tokens;
    std::string line = str;
    while ((pos = line.find(delimiter)) != std::string::npos) {
        std::string token = line.substr(0, pos);
        line.erase(0, pos + delimiter.length());
        tokens.push_back(token);
    }
    return tokens;
}