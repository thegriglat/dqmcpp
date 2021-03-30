#ifndef URLCACHE_HH
/**
 * @file URLCache.hh
 * @author Grigory Latyshev (thegriglat@gmail.com)
 * @brief Caches URL requests to filesystem
 */

#include <string>

namespace dqmcpp {
namespace net {

namespace URLCache {

std::string get(const std::string& url);

};  // namespace URLCache
}  // namespace net
}  // namespace dqmcpp
#define URLCACHE_HH

#endif