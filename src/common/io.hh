#ifndef COMMON_IO_HH

#include <string>

namespace dqmcpp {

namespace common {

/**
 * @brief Checks if dir exists
 *
 * @param path
 * @return true
 * @return false
 */
bool file_exists(const std::string& path);

/**
 * @brief Like mkdir -p. True if success
 *
 * @param path
 * @return true
 * @return false
 */
bool mkdir_p(const std::string& path);

/**
 * @brief Like dirname
 *
 * @param path
 * @return std::string
 */
std::string dirname(const std::string& path);

}  // namespace common
}  // namespace dqmcpp

#define COMMON_IO_HH
#endif  // !COMMON_IO_HH
