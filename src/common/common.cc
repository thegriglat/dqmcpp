#include "common.hh"
#include <string>
#include <vector>

std::vector<std::string> split(const std::string str,
                               const std::string delimiter) {
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

std::string join(const std::vector<std::string> list,
                 const std::string delimiter) {
  std::string result;
  const auto len = list.size();
  for (int i = 0; i < len; ++i) {
    result += list.at(i);
    if (i != len - 1)
      result += delimiter;
  }
  return result;
}