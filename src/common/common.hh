#ifndef COMMON_HH

#include <assert.h>
#include <bits/stl_function.h>
#include <algorithm>
#include <string>
#include <vector>

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

#define COMMON_HH
#endif