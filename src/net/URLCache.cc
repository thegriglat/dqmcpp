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
  if (!file_exists(dirpath))
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

struct URLMTInfo {
  bool cached;
  std::string url;
};

/** Commented as cause fail with signal 11 on heavy usage **/

/*
std::vector<std::string> _getMT(const std::vector<std::string>& urls) {
  std::vector<std::string> contents(urls.size());
  std::vector<std::pair<std::string, std::string>> to_cache;
  common::foreach_mt(urls.begin(), urls.end(),
                     [&urls, &contents, &to_cache](const std::string& url) {
                       // in theory should be thread safe
                       const auto index = common::index(urls, url);
                       if (has(url)) {
                         contents.at(index) = getFromCache(url);
                         return;
                       }
                       net::URLHandler hldr;
                       const auto content = hldr.get(url);
                       contents.at(index) = content;
                       to_cache.push_back({url, content});
                     });
  // sync flush to disk
  for (auto& pair : to_cache) {
    add(pair.first, pair.second);
  }
  return contents;
}
*/

std::vector<std::string> _getST(const std::vector<std::string>& urls) {
  std::vector<std::string> contents;
  contents.reserve(urls.size());
  std::transform(
      urls.begin(), urls.end(), std::back_inserter(contents),
      [](const std::string& url) { return dqmcpp::net::URLCache::get(url); });
  return contents;
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

std::vector<std::string> URLCache::get(const std::vector<std::string>& urls) {
  return _getST(urls);
}

}  // namespace net
}  // namespace dqmcpp
