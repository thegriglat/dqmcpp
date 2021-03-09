#include "common.hh"
#include <iostream>
#include <string>
#include <vector>
#include "sys/dir.h"
#include "sys/stat.h"

std::vector<std::string> split(const std::string str,
                               const std::string delimiter) {
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

bool file_exists(const std::string& path) {
  struct stat buffer;
  return (stat(path.c_str(), &buffer) == 0);
}

bool _mkdir(const std::string& path) {
  bool success = true;
  if (!file_exists(path)) {
    auto r = mkdir(path.c_str(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
    success = (r != -1);
    if (!success) {
      std::cerr << "Cannot create directory " << path << std::endl;
    }
  }
  return success;
}

bool mkdir_p(const std::string& path) {
  const auto pathlist = split(path, "/");
  std::string tmp = "";
  bool success = true;
  for (auto ppart : pathlist) {
    tmp += ppart + "/";
    success = success && _mkdir(tmp);
  }
  return success;
}

std::string dirname(const std::string& path) {
  auto l = split(path, "/");
  std::vector<std::string> q(l.begin(), l.end() - 1);
  return join(q, "/");
}