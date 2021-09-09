
#include "plugin.hh"
#include <iostream>
#include <string>

// for non-cached requests
#include "net/URLHandler.hh"

// for cached requests (usual case)
#include "net/URLCache.hh"

// for parallel requests
#include "net/URLHandlerMT.hh"

// do registration
REGISTER_PLUGIN(RequestEx, requests)

using namespace std;
using namespace dqmcpp;

namespace dqmcpp {

namespace plugins {

void RequestEx::Process() {
  // change here to you preferred url
  string url = "https://google.com";

  // === Non-cached requests ===
  {
    // initialize non-cached handler
    // It's simple wrapper about cURL library
    net::URLHandler hldr;

    // do sync request
    auto content = hldr.get(url);

    cout << "Content of " << url << " by URLHandler is " << endl;
    cout << "======" << endl << content << endl << "======" << endl;

    // here "hldr" will be destroyed at scope ends
    // no need to clean resources
  }

  // === Cached requests ===
  {
    /**
     * By default cache uses $TMP directory (/tmp in most systems)
     * To change this -- set CACHEDIR environment variable
     */

    auto content = net::URLCache::get(url);

    cout << "Content of " << url << " by URLCache is " << endl;
    cout << "======" << endl << content << endl << "======" << endl;
  }

  // === Multiple requests ===
  {
    /**
     * There are two ways to process multiple urls by one call
     * 1: Use  dqmcpp::net::URLCache::get(std::vector<std::string>>)
     * 2: Use dqmcpp::net::URLHandlerMT::get(std::vector<std::string>>)
     *
     * Using URLHandlerMT is matter when you request large number of urls (more
     * than ~100) at one time. In other cases there are no differences in
     * performance (or it will be ever worse).
     * URLHandlerMT uses thread pool (default is max CPU as possible)
     */

    cout << "Multiple requests ..." << endl;
    std::vector<std::string> urls = {"https://google.com",
                                     "https://github.com"};
    auto content = net::URLCache::get(urls);

    auto content_mt = net::URLHandlerMT::get(urls);

    cout << "Size of contents: " << content.size() << endl;
    for (size_t i = 0; i < content.size(); ++i) {
      cout << "  size in bytes of content[" << i
           << "]  = " << content.at(i).size() << endl;
    }

    cout << "Size of contents (mt): " << content_mt.size() << endl;
    for (size_t i = 0; i < content_mt.size(); ++i) {
      cout << "  size in bytes of content_mt[" << i
           << "]  = " << content_mt.at(i).size() << endl;
    }
  }
}
}  // namespace plugins

}  // namespace dqmcpp
