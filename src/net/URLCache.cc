/**
 * @file URLCache.cc
 * @author Grigory Latyshev (thegriglat@gmail.com)
 * @brief URLCache implementation
 */
#include "URLCache.hh"
#include <fstream>
#include <functional>
#include "../common/common.hh"
#include "URLHandler.hh"

namespace {

using namespace dqmcpp;
using namespace dqmcpp::common;
net::URLHandler urlhandler;
const std::string CACHEDIR = "/tmp";

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

std::string getCacheDir(void) {
  auto env = std::getenv("CACHEDIR");
  if (env)
    return env;
  return CACHEDIR;
}

std::string hash(const std::string& text) {
  // return basename
  auto s = split(text, "/");
  return s.back();
}
std::string cFileName(const std::string& url) {
  auto dirpath = getCacheDir() + "/" + dirname(removeProtocol(url));
  mkdir_p(dirpath);
  return dirpath + "/" + hash(url);
}
std::string getFromCache(const std::string& url) {
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

void add(const std::string& url, const std::string& content) {
  std::ofstream out(cFileName(url));
  out << content;
  out.close();
}

bool has(const std::string& url) {
  return file_exists(cFileName(url));
}

}  // namespace

namespace dqmcpp {
namespace net {

std::string URLCache::get(const std::string& url) {
  if (has(url)) {
    return getFromCache(url);
  }
  const auto content = urlhandler.get(url);
  add(url, content);
  return content;
}

}  // namespace net
}  // namespace dqmcpp