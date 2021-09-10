/**
 * @file URLHandlerMT.cc
 * @author Grigory Latyshev (thegriglat@gmail.com)
 * @brief Multithreaded cURL requests
 *
 *
 */
#include "URLHandlerMT.hh"

#include <string>
#include <thread>
#include <vector>
#include "../common/lists.hh"
#include "URLHandler.hh"

namespace {
void download(const std::vector<std::string>& urls,
              std::vector<std::string>& content) {
  dqmcpp::net::URLHandler hdlr;
  for (unsigned int i = 0; i < urls.size(); ++i) {
    const auto& url = urls.at(i);
    content.at(i) = hdlr.get(url);
  }
}

}  // namespace

namespace dqmcpp {
namespace net {
namespace URLHandlerMT {

std::vector<std::string> get(const std::vector<std::string>& urls) {
  const auto urlchunks = common::chunks(urls, 4);
  std::vector<std::thread> threads;
  std::vector<std::vector<std::string>> content;
  // prepare content vector
  std::transform(
      urlchunks.begin(), urlchunks.end(), std::back_inserter(content),
      [](const auto& c) { return std::vector<std::string>(c.size()); });

  for (unsigned int i = 0; i < urlchunks.size(); ++i) {
    threads.push_back(
        std::thread(download, urlchunks.at(i), std::ref(content.at(i))));
  }
  // awaiting result
  for (auto& t : threads)
    t.join();

  std::vector<std::string> result;
  result.reserve(urls.size());
  // unroll
  for (auto& s0 : content)
    std::transform(s0.begin(), s0.end(), std::back_inserter(result),
                   [](const auto& e) { return e; });

  return result;
}

}  // namespace URLHandlerMT
}  // namespace net
}  // namespace dqmcpp
