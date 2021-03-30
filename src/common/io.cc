#include "io.hh"

#include "string.hh"
#include "sys/dir.h"
#include "sys/stat.h"

namespace {
bool _mkdir(const std::string& path) {
  int r = 0;
  if (!dqmcpp::common::file_exists(path)) {
    r = mkdir(path.c_str(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
  }
  return (r != -1);
}
}  // namespace

namespace dqmcpp {
namespace common {

bool file_exists(const std::string& path) {
  struct stat buffer;
  return (stat(path.c_str(), &buffer) == 0);
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