/**
 * @file string.cc
 * @author Grigory Latyshev (thegriglat@gmail.com)
 * @brief String functions
 *
 *
 */
#include "string.hh"

#include <algorithm>

namespace dqmcpp {
namespace common {
std::vector<std::string> split(const std::string& str,
                               const std::string& delimiter) {
  size_t pos;
  std::vector<std::string> tokens;
  std::string line = str;
  if (line.find(delimiter) == std::string::npos)
    return {line};
  do {
    pos = line.find(delimiter);
    std::string token = line.substr(0, pos);
    line.erase(0, pos + delimiter.length());
    tokens.push_back(token);
  } while (pos != std::string::npos);
  return tokens;
}

std::string join(const std::vector<std::string>& list,
                 const std::string& delimiter) {
  std::string result;
  if (list.size() == 0)
    return result;
  for (auto it = list.begin(); it != list.end() - 1; ++it) {
    result.append(*it);
    result.append(delimiter);
  }
  result.append(list.back());
  return result;
}

std::string rtrim(const std::string& str) {
  auto s = str;
  s.erase(std::find_if(s.rbegin(), s.rend(),
                       [](unsigned char ch) { return !std::isspace(ch); })
              .base(),
          s.end());
  return s;
}

std::string ltrim(const std::string& str) {
  auto s = str;
  s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](unsigned char ch) {
            return !std::isspace(ch);
          }));
  return s;
}

}  // namespace common
}  // namespace dqmcpp