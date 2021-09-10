#ifndef URLHANDLERMT_HH
/**
 * @file URLHandlerMT.hh
 * @author Grigory Latyshev (thegriglat@gmail.com)
 * @brief Multithreaded cURL requests
 *
 *
 */
#include <string>
#include <vector>

namespace dqmcpp {
namespace net {
/**
 * @brief Multithreaded cURL requests
 *
 */
namespace URLHandlerMT {

/**
 * @brief Multithreaded cURL
 *
 * @param urls URLs to request
 * @return std::vector<std::string>
 */
std::vector<std::string> get(const std::vector<std::string>& urls);

}  // namespace URLHandlerMT
}  // namespace net
}  // namespace dqmcpp

#define URLHANDLERMT_HH
#endif  // !URLHANDLERMT_HH