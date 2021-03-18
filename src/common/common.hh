#ifndef COMMON_HH
/**
 * @file common.hh
 * @author Grigory Latyshev (thegriglat@gmail.com)
 * @brief Common pure functions
 */
#include <assert.h>
#include <bits/stl_function.h>
#include <algorithm>
#include <memory>
#include <string>
#include <vector>

#include "functionalcpp.hh"

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
 * @brief Get element index in vector. Returns -1 if not found
 *
 * @tparam T
 * @param data
 * @param element
 * @return int
 */
template <typename T, typename BinaryOp>
int index(const std::vector<T>& data, BinaryOp index_fn) {
  auto it = std::find_if(data.begin(), data.end(), index_fn);
  if (it == data.end())
    // element not found
    return -1;
  return std::distance(data.begin(), it);
};
template <typename T>
int index(const std::vector<T>& data, const T& element) {
  auto it = std::find(data.begin(), data.end(), element);
  if (it == data.end())
    // element not found
    return -1;
  return std::distance(data.begin(), it);
}

/**
 * @brief Returns true if vector has element
 *
 * @param data
 * @param elem
 * @return true
 * @return false
 */
template <typename T>
bool has(const std::vector<T>& data, const T& element) {
  return (index(data, element) != -1);
}
template <typename T, typename BinaryOp>
int has(const std::vector<T>& data, BinaryOp index_fn) {
  return (index(data, index_fn) != -1);
}

/**
 * @brief Returns maximum value from vector of T with getter
 *
 * @tparam T
 * @param list
 * @param getter
 * @return double
 */
template <typename T, typename BinaryOp>
double maximum(std::vector<T>& list, BinaryOp getter) {
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

template <typename T, typename BinaryOp>
double sum(const std::vector<T>& d, BinaryOp fn) {
  double s = 0;
  for (const auto& e : d)
    s += fn(e);
  return s;
}

template <typename T>
int sign(const T& value) {
  if (value == 0)
    return 0;
  return ((value > 0) ? 1 : -1);
}

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
#define COMMON_HH
#endif