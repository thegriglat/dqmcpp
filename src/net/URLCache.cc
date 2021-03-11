/**
 * @file URLCache.cc
 * @author Grigory Latyshev (thegriglat@gmail.com)
 * @brief URLCache implementation
 */
#include "URLCache.hh"
#include <fstream>
#include <functional>
#include "../common/common.hh"

namespace {

using namespace dqmcpp::common;
/**
 * @brief Removes http://, https:// etc.
 *
 * @param url
 * @return std::string
 */
std::string removeProtocol(const std::string& url) {
  static const std::string protodelimeter = "://";
  auto pos = url.find(protodelimeter);
  return url.substr(pos + protodelimeter.length());
}

}  // namespace

namespace dqmcpp {
namespace net {

void URLCache::setCacheDir(const std::string& dir) {
  _cachedir = dir;
}

std::string URLCache::getCacheDir(void) const {
  auto env = std::getenv("CACHEDIR");
  if (env)
    return env;
  return _cachedir;
}

std::string URLCache::hash(const std::string& text) const {
  // return basename
  auto s = split(text, "/");
  return s.back();
}
std::string URLCache::cFileName(const std::string& url) const {
  auto dirpath = getCacheDir() + "/" + dirname(removeProtocol(url));
  mkdir_p(dirpath);
  return dirpath + "/" + hash(url);
}
std::string URLCache::getFromCache(const std::string& url) const {
  // assuming that file exists
  std::ifstream in(cFileName(url));
  std::string content;
  in.seekg(0, std::ios::end);
  content.reserve(in.tellg());
  in.seekg(0, std::ios::beg);
  // read entire file
  content.assign((std::istreambuf_iterator<char>(in)),
                 std::istreambuf_iterator<char>());
  in.close();
  return content;
}

void URLCache::add(const std::string& url, const std::string& content) {
  std::ofstream out(cFileName(url));
  out << content;
  out.close();
}

bool URLCache::has(const std::string& url) const {
  return file_exists(cFileName(url));
}

std::string URLCache::get(const std::string& url) {
  if (has(url)) {
    return getFromCache(url);
  }
  const auto content = _urlhandler->get(url);
  add(url, content);
  return content;
}

}  // namespace net
}  // namespace dqmcpp