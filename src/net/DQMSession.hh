#ifndef DQMSESSION_HH
/**
 * @file DQMSession.hh
 * @author Grigory Latyshev (thegriglat@gmail.com)
 * @brief Manage DQM sessions
 */
#include <string>

namespace dqmcpp {
namespace net {
namespace DQMSession {

/**
 * @brief Returns DQM session ID. Keeps session between calls.
 *
 * @return std::string
 */
std::string get();

}  // namespace DQMSession
}  // namespace net
}  // namespace dqmcpp
#define DQMSESSION_HH
#endif  // !DQMSESSION_HH