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