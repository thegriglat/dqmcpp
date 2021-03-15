#ifndef READER_HH
/**
 * @file Reader.hh
 * @author Grigory Latyshev (thegriglat@gmail.com)
 * @brief Abstract class for all readers
 */
#include <string>
#include "../ECAL/ECAL.hh"
#include "../net/URLCache.hh"

namespace dqmcpp {
namespace readers {

class Reader {
  dqmcpp::net::URLCache* _cache = nullptr;

 public:
  Reader(dqmcpp::net::URLCache* cache = new dqmcpp::net::URLCache())
      : _cache(cache){};
  // std::string url(const unsigned int runnumber, const std::string &dataset);
  /**
   * @brief Returns HTTP content by url using cache.
   *
   * @param url URL
   * @return std::string
   */
  inline std::string get(const std::string& url) { return _cache->get(url); }
  /**
   * @brief Virtual function for parsing
   *
   * @param content Some std::string parameter, usually HTTP response content
   * @return std::vector<ECAL::ChannelData>
   */
  virtual std::vector<ECAL::ChannelData> parse(const std::string& content) = 0;
  virtual std::vector<ECAL::Data2D> parse2D(const std::string& content,
                                            bool skipZeroes = true) = 0;
};

}  // namespace readers
}  // namespace dqmcpp
#define READER_HH
#endif