#include "URLHandlerMT.hh"

#include <string>
#include <thread>
#include <vector>
#include "../common/lists.hh"
#include "URLHandler.hh"

namespace {
const uint NThreads = 6;

void download(const std::vector<std::string>& urls,
              std::vector<std::string>& content) {
  dqmcpp::net::URLHandler hdlr;
  for (uint i = 0; i < urls.size(); ++i) {
    const auto& url = urls.at(i);
    content.at(i) = hdlr.get(url);
  }
}

}  // namespace

namespace dqmcpp {
namespace net {
namespace URLHandlerMT {

std::vector<std::string> get(const std::vector<std::string>& urls) {
  const auto urlchunks = common::chunks(urls, NThreads);
  std::vector<std::thread> threads;
  std::vector<std::vector<std::string>> content;
  // prepare content vector
  for (auto& c : urlchunks)
    content.push_back(std::vector<std::string>(c.size()));

  for (uint i = 0; i < urlchunks.size(); ++i) {
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
    for (auto& s : s0)
      result.push_back(s);

  return result;
}

}  // namespace URLHandlerMT
}  // namespace net
}  // namespace dqmcpp
