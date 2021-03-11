#ifndef COMMON_HH
/**
 * @file common.hh
 * @author Grigory Latyshev (thegriglat@gmail.com)
 * @brief Common pure functions
 */
#include <assert.h>
#include <bits/stl_function.h>
#include <algorithm>
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
std::vector<std::string> split(const std::string str,
                               const std::string delimiter = " ");
/**
 * @brief Joins vector of strings with delimiter
 *
 * @param list
 * @param delimiter
 * @return std::string
 */
std::string join(const std::vector<std::string> list,
                 const std::string delimiter = ",");

/**
 * @brief Returns true if vector has element
 *
 * @param data
 * @param elem
 * @return true
 * @return false
 */
template <typename T>
bool has(const std::vector<T>& data, const T& elem) {
  return std::find(data.begin(), data.end(), elem) != data.end();
}

/**
 * @brief Returns maximum value from vector of T with getter
 *
 * @tparam T
 * @param list
 * @param getter
 * @return double
 */
template <typename T>
double maximum(std::vector<T>& list, double (*getter)(const T&)) {
  if (list.size() == 0)
    return -1;
  double max = getter(list.at(0));
  for (auto& e : list) {
    auto _val = getter(e);
    if (_val > max)
      max = _val;
  }
  return max;
}

/**
 * @brief Returns maximum value from vector of T
 *
 * @tparam T
 * @param list
 * @return double
 */
template <typename T>
double maximum(std::vector<T>& list) {
  return maximum(list, [](const T& e) { return e; });
}

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

}  // namespace common
}  // namespace dqmcpp
#define COMMON_HH
#endif