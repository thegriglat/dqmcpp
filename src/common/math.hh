#ifndef COMMON_MATH_HH

#include <algorithm>
#include <cmath>

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
  for (auto it = begin + 1; it != end; ++it) {
    const auto _tmp = getfn(*it);
    if (max > _tmp) {
      max = _tmp;
    }
  }
  return max;
}

template <typename T, typename BinaryOp>
double maximum(std::vector<T>& list, BinaryOp getter) {
  return maximum(list.begin(), list.end(), getter);
}

template <typename T>
double maximum(std::vector<T>& list) {
  return maximum(list, [](const T& e) { return e; });
}

template <typename Iterator, typename BinaryOp>
double minimum(Iterator begin, Iterator end, BinaryOp getfn) {
  if (begin == end)
    return 0;
  double min = getfn(*begin);
  for (auto it = begin + 1; it != end; ++it) {
    const auto _tmp = getfn(*it);
    if (min > _tmp) {
      min = _tmp;
    }
  }
  return min;
}

template <typename T, typename BinaryOp>
double minimum(std::vector<T>& list, BinaryOp getter) {
  return minimum(list.begin(), list.end(), getter);
}

template <typename T>
double minimum(std::vector<T>& list) {
  return minimum(list, [](const T& e) { return e; });
}

template <typename T>
int sign(const T& value) {
  if (value == 0)
    return 0;
  return ((value > 0) ? 1 : -1);
}

// Mean
template <typename Iterator, typename BinaryOp>
double mean(Iterator begin, Iterator end, BinaryOp op) {
  return sum(begin, end, op) / std::distance(begin, end);
}

template <typename T, typename BinaryOp>
double mean(const std::vector<T>& list, BinaryOp op) {
  return mean(list.begin(), list.end(), op);
}

template <typename T>
double mean(const std::vector<T>& list) {
  return mean(list.begin(), list.end(), [](const T& e) { return e; });
}

// RMS
template <typename Iterator, typename BinaryOp>
double rms(Iterator begin, Iterator end, BinaryOp op) {
  double _mean = mean(begin, end, op);
  const int n = std::distance(begin, end);
  double _sum = 0;
  // TODO: Can it be replaced with sum() ?
  for (; begin != end; ++begin) {
    const auto _xi = op(*begin);
    _sum += (_xi - _mean) * (_xi - _mean);
  }
  return std::sqrt(_sum / n);
}

template <typename T, typename BinaryOp>
double rms(const std::vector<T>& list, BinaryOp op) {
  return rms(list.begin(), list.end(), op);
}

template <typename T>
double rms(const std::vector<T>& list) {
  return rms(list.begin(), list.end(), [](const T& e) { return e; });
}

// median
template <typename It, typename BinaryOp>
double median(It begin, It end, BinaryOp op) {
  if (std::distance(begin, end) == 0)
    return 0;
  std::vector<double> tmp;
  tmp.reserve(std::distance(begin, end));
  for (; begin != end; ++begin) {
    tmp.push_back(op(*begin));
  }
  std::sort(tmp.begin(), tmp.end());
  if (tmp.size() % 2 == 0) {
    const auto left = tmp.size() / 2 - 1;
    const auto right = tmp.size() / 2;
    return (tmp.at(left) + tmp.at(right)) / 2;
  }
  return tmp.at((tmp.size() - 1) / 2);
}

template <typename T, typename BinaryOp>
double median(const std::vector<T>& list, BinaryOp op) {
  return dqmcpp::common::median(list.begin(), list.end(), op);
}

template <typename T>
double median(const std::vector<T>& list) {
  return dqmcpp::common::median(list, [](const T& e) { return e; });
}

}  // namespace common
}  // namespace dqmcpp

#define COMMON_MATH_HH
#endif  // !COMMON_MATH_HH