#include "io.hh"

#include <iostream>
#include "string.hh"
#include "sys/dir.h"
#include "sys/stat.h"

namespace dqmcpp {
namespace common {

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

}  // namespace common
}  // namespace dqmcpp