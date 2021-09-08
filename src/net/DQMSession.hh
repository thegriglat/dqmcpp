#ifndef DQMSESSION_HH
/**
 * @file DQMSession.hh
 * @author Grigory Latyshev (thegriglat@gmail.com)
 * @brief Manage DQM sessions
 */
#include <string>

namespace dqmcpp {
namespace net {
/**
 * @brief Functions to operate with DQM sessions
 *
 */
namespace DQMSession {

/**
 * @brief Returns DQM session ID
 *
 * @param offline true if offline session is needed
 * @return std::string
 */
std::string get(const bool offline = true);

}  // namespace DQMSession
}  // namespace net
}  // namespace dqmcpp
#define DQMSESSION_HH
#endif  // !DQMSESSION_HH