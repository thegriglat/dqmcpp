#ifndef COMMON_MATH_HH

namespace dqmcpp {
namespace common {

template <typename ForwardIterator, typename BinaryOp>
double sum(ForwardIterator begin, ForwardIterator end, BinaryOp fn) {
  double s = 0;
  for (; begin != end; ++begin)
    s += fn(*begin);
  return s;
}

template <typename T, typename BinaryOp>
double sum(const std::vector<T>& d, BinaryOp fn) {
  return sum(d.begin(), d.end(), fn);
}

template <typename T>
double sum(const std::vector<T>& list) {
  return sum(list.begin(), list.end(), [](const T& e) { return e; });
}

template <typename Iterator, typename BinaryOp>
double maximum(Iterator begin, Iterator end, BinaryOp getfn) {
  if (begin == end)
    return 0;
  double max = getfn(*begin);
  for (auto it = begin + 1; it != end; ++it)
    max = std::max(max, getfn(*it));
  return max;
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
  return maximum(list.begin(), list.end(), getter);
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

template <typename T>
int sign(const T& value) {
  if (value == 0)
    return 0;
  return ((value > 0) ? 1 : -1);
}
}  // namespace common
}  // namespace dqmcpp

#define COMMON_MATH_HH
#endif  // !COMMON_MATH_HH