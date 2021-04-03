#ifndef COMMON_STRING_HH

#include <memory>
#include <stdexcept>
#include <string>
#include <vector>

namespace dqmcpp {
namespace common {

/**
 * @brief Splits std::string by delimiter
 *
 * @param str
 * @param delimiter
 * @return std::vector<std::string>
 */
std::vector<std::string> split(const std::string& str,
                               const std::string& delimiter = " ");
/**
 * @brief Joins vector of strings with delimiter
 *
 * @param list
 * @param delimiter
 * @return std::string
 */
std::string join(const std::vector<std::string>& list,
                 const std::string& delimiter = ",");

/**
 * @brief Remove trailing whitespaces
 *
 * @param text
 * @return std::string
 */
std::string rtrim(const std::string& text);

/**
 * @brief Remove leading whitespaces
 *
 * @param text
 * @return std::string
 */
std::string ltrim(const std::string& text);

/**
 * @brief Remove trailing and leading whitespaces
 *
 * @param text
 * @return std::string
 */
inline std::string trim(const std::string& text) {
  return rtrim(ltrim(text));
};

template <typename... Args>
std::string string_format(const std::string& format, Args... args) {
  // https://stackoverflow.com/questions/2342162/stdstring-formatting-like-sprintf
  int size = snprintf(nullptr, 0, format.c_str(), args...) +
             1;  // Extra space for '\0'
  if (size <= 0) {
    throw std::runtime_error("Error during formatting.");
  }
  std::unique_ptr<char[]> buf(new char[size]);
  snprintf(buf.get(), size, format.c_str(), args...);
  return std::string(buf.get(),
                     buf.get() + size - 1);  // We don't want the '\0' inside
}

}  // namespace common
}  // namespace dqmcpp

#define COMMON_STRING_HH
#endif