#ifndef COMMON_IO_HH

#include <string>

namespace dqmcpp {

namespace common {

/**
 * @brief Checks if dir exists
 *
 * @param path Path to check
 * @return true
 * @return false
 */
bool file_exists(const std::string& path);

/**
 * @brief Like mkdir -p. True if success
 *
 * @param path Path to create
 * @return true
 * @return false
 */
bool mkdir_p(const std::string& path);

/**
 * @brief Like dirname
 *
 * @param path Path
 * @return std::string
 */
std::string dirname(const std::string& path);

}  // namespace common
}  // namespace dqmcpp

#define COMMON_IO_HH
#endif  // !COMMON_IO_HH
