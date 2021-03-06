#ifndef COMMON_MATH_HH
/**
 * @file math.hh
 * @author Grigory Latyshev (thegriglat@gmail.com)
 * @brief Math templated functions
 *
 *
 */
#include <algorithm>
#include <cmath>
#include <numeric>

namespace dqmcpp {
namespace common {

/**
 * @brief Returns sum of array elements with given function fn (T&)->double
 *
 * @tparam Iterator
 * @tparam BinaryOp
 * @param begin begin iterator
 * @param end end iterator
 * @param fn binary convertion to double function
 * @return double
 */
template <typename Iterator, typename BinaryOp>
double sum(Iterator begin, Iterator end, BinaryOp fn) {
  double s = 0;
  for (; begin != end; ++begin)
    s += fn(*begin);
  return s;
}

/**
 * @brief Returns sum of array elements with given function fn (T&)->double
 *
 * @tparam Collection
 * @tparam BinaryOp
 * @param d collection
 * @param fn binary convertion to double function
 * @return double
 */
template <typename Collection, typename BinaryOp>
inline double sum(const Collection& d, BinaryOp fn) {
  return sum(d.begin(), d.end(), fn);
}

/**
 * @brief Returns sum of array elements
 *
 * @tparam Collection
 * @param list input list
 * @return double
 */
template <typename Collection>
inline double sum(const Collection& list) {
  return std::accumulate(list.begin(), list.end(), 0.0);
}

/**
 * @brief Return sign of value
 *
 * @tparam T
 * @param value
 * @return int
 */
template <typename T>
int sign(const T& value) {
  if (value == 0)
    return 0;
  return ((value > 0) ? 1 : -1);
}

/**
 * @brief Return mean of list calculated by function op
 *
 * @tparam Iterator
 * @tparam BinaryOp
 * @param begin
 * @param end
 * @param op
 * @return double
 */
template <typename Iterator, typename BinaryOp>
inline double mean(Iterator begin, Iterator end, BinaryOp op) {
  return sum(begin, end, op) / std::distance(begin, end);
}

/**
 * @brief Return mean of list calculated by function
 *
 * @tparam Collection
 * @tparam BinaryOp
 * @param list
 * @param op
 * @return double
 */
template <typename Collection, typename BinaryOp>
inline double mean(const Collection& list, BinaryOp op) {
  return sum(list, op) / list.size();
}

/**
 * @brief Return mean of list
 *
 * @tparam Collection
 * @param list
 * @return double
 */
template <typename Collection>
inline double mean(const Collection& list) {
  return sum(list) / list.size();
}

/**
 * @brief Return RMS of list calculated by function
 *
 * @tparam Iterator
 * @tparam BinaryOp
 * @param begin
 * @param end
 * @param op
 * @return double
 */
template <typename Iterator, typename BinaryOp>
double rms(Iterator begin, Iterator end, BinaryOp op) {
  if (std::distance(begin, end) == 0)
    return 0;
  double _mean = mean(begin, end, op);
  const int n = std::distance(begin, end);
  // TODO: Can it be replaced with sum() ?
  auto _sum = sum(begin, end, [_mean, op](const Iterator& it) {
    const auto _xi = op(*it);
    return (_xi - _mean) * (_xi - _mean);
  });
  return std::sqrt(_sum / n);
}

/**
 * @brief Return RMS of list calculated by function
 *
 * @tparam Collection
 * @tparam BinaryOp
 * @param list
 * @param op
 * @return double
 */
template <typename Collection, typename BinaryOp>
inline double rms(const Collection& list, BinaryOp op) {
  return rms(list.begin(), list.end(), op);
}

/**
 * @brief Return RMS of list
 *
 * @tparam Collection
 * @param list
 * @return double
 */
template <typename Collection>
double rms(const Collection& list) {
  if (list.size() == 0)
    return 0;
  auto _mean = mean(list);
  auto n = list.size();
  auto _sum = sum(list, [_mean](const typename Collection::value_type& _xi) {
    return (_xi - _mean) * (_xi - _mean);
  });
  return std::sqrt(_sum / n);
}

/**
 * @brief Return median of list calculated by function
 *
 * @tparam It
 * @tparam BinaryOp
 * @param begin
 * @param end
 * @param op
 * @return double
 */
template <typename It, typename BinaryOp>
double median(It begin, It end, BinaryOp op) {
  if (std::distance(begin, end) == 0)
    return 0;
  std::vector<double> tmp(std::distance(begin, end));
  std::transform(begin, end, tmp.begin(), op);
  std::sort(tmp.begin(), tmp.end());
  if (tmp.size() % 2 == 0) {
    const auto left = tmp.size() / 2 - 1;
    const auto right = tmp.size() / 2;
    return (tmp.at(left) + tmp.at(right)) / 2;
  }
  return tmp.at((tmp.size() - 1) / 2);
}

/**
 * @brief Return median of list calculated by function
 *
 * @tparam Collection
 * @tparam BinaryOp
 * @param list
 * @param op
 * @return double
 */
template <typename Collection, typename BinaryOp>
inline double median(const Collection& list, BinaryOp op) {
  return dqmcpp::common::median(list.begin(), list.end(), op);
}

/**
 * @brief Return median of list
 *
 * @tparam Collection
 * @param list
 * @return double
 */
template <typename Collection>
inline double median(const Collection& list) {
  return dqmcpp::common::median(
      list, [](const typename Collection::value_type& e) { return e; });
}

/**
 * @brief For float/double types. Useful for comparing float/double with 0
 *
 * @tparam T
 * @param value
 * @return true
 * @return false
 */
template <typename T>
inline bool isNotZero(const T& value) {
  return value > 0 || value < 0;
}

/**
 * @brief Return true if value is zero. Useful for comparing float/double with 0
 *
 * @tparam T
 * @param value
 * @return true
 * @return false
 */
template <typename T>
inline bool isZero(const T& value) {
  return !isNotZero(value);
}

/**
 * @brief float equality
 *
 * @tparam T
 * @param a
 * @param b
 * @return true
 * @return false
 */
template <typename T>
inline bool equal(const T& a, const T& b) {
  return isZero(a - b);
}

}  // namespace common
}  // namespace dqmcpp

#define COMMON_MATH_HH
#endif  // !COMMON_MATH_HH