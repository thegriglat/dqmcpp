#ifndef URLCACHE_HH
/**
 * @file URLCache.hh
 * @author Grigory Latyshev (thegriglat@gmail.com)
 * @brief Caches URL requests to filesystem
 */

#include <string>
#include <vector>

namespace dqmcpp {
namespace net {

namespace URLCache {

/**
 * @brief Get sync URL content with caching
 *
 * @param url URL to request
 * @return std::string
 */
std::string get(const std::string& url);

/**
 * @brief Get sync URLs content with caching
 *
 * @param urls URLs to request
 * @return std::vector<std::string>
 */
std::vector<std::string> get(const std::vector<std::string>& urls);

};  // namespace URLCache
}  // namespace net
}  // namespace dqmcpp
#define URLCACHE_HH

#endif