#ifndef URLHANDLERMT_HH

#include <string>
#include <vector>

namespace dqmcpp {
namespace net {
namespace URLHandlerMT {

/**
 * @brief Multithreaded cURL
 *
 * @param urls
 * @return std::vector<std::string>
 */
std::vector<std::string> get(const std::vector<std::string>& urls);

}  // namespace URLHandlerMT
}  // namespace net
}  // namespace dqmcpp

#define URLHANDLERMT_HH
#endif  // !URLHANDLERMT_HH