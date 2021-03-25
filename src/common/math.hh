#ifndef COMMON_MATH_HH

#include <algorithm>
#include <cmath>
#include <numeric>

namespace dqmcpp {
namespace common {

template <typename ForwardIterator, typename BinaryOp>
double sum(ForwardIterator begin, ForwardIterator end, BinaryOp fn) {
  double s = 0;
  for (; begin != end; ++begin)
    s += fn(*begin);
  return s;
}

template <typename Collection, typename BinaryOp>
inline double sum(const Collection& d, BinaryOp fn) {
  return sum(d.begin(), d.end(), fn);
}

template <typename Collection>
inline double sum(const Collection& list) {
  return std::accumulate(list.begin(), list.end(), 0.0);
}

template <typename Iterator, typename BinaryOp>
double maximum(Iterator begin, Iterator end, BinaryOp getfn) {
  if (begin == end)
    return 0;
  double max = getfn(*begin);
  for (auto it = begin + 1; it != end; ++it) {
    const auto _tmp = getfn(*it);
    if (_tmp > max) {
      max = _tmp;
    }
  }
  return max;
}

template <typename Collection, typename BinaryOp>
inline double maximum(const Collection& list, BinaryOp getter) {
  return maximum(list.begin(), list.end(), getter);
}

template <typename Collection>
double maximum(const Collection& list) {
  if (list.size() == 0)
    return 0;
  double m = list.at(0);
  for (auto it = list.begin() + 1; it != list.end(); ++it)
    m = std::max(m, *it);
  return m;
}

template <typename Iterator, typename BinaryOp>
double minimum(Iterator begin, Iterator end, BinaryOp getfn) {
  if (begin == end)
    return 0;
  double min = getfn(*begin);
  for (auto it = begin + 1; it != end; ++it) {
    const auto _tmp = getfn(*it);
    if (_tmp < min) {
      min = _tmp;
    }
  }
  return min;
}

template <typename T, typename BinaryOp>
inline double minimum(std::vector<T>& list, BinaryOp getter) {
  return minimum(list.begin(), list.end(), getter);
}

template <typename Collection>
double minimum(const Collection& list) {
  if (list.size() == 0)
    return 0;
  double m = list.at(0);
  for (auto it = list.begin() + 1; it != list.end(); ++it)
    m = std::min(m, *it);
  return m;
}

template <typename T>
int sign(const T& value) {
  if (value == 0)
    return 0;
  return ((value > 0) ? 1 : -1);
}

// Mean
template <typename Iterator, typename BinaryOp>
inline double mean(Iterator begin, Iterator end, BinaryOp op) {
  return sum(begin, end, op) / std::distance(begin, end);
}

template <typename Collection, typename BinaryOp>
inline double mean(const Collection& list, BinaryOp op) {
  return sum(list, op) / list.size();
}

template <typename Collection>
inline double mean(const Collection& list) {
  return sum(list) / list.size();
}

// RMS
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

template <typename Collection, typename BinaryOp>
inline double rms(const Collection& list, BinaryOp op) {
  return rms(list.begin(), list.end(), op);
}

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

template <typename Collection, typename BinaryOp>
inline double median(const Collection& list, BinaryOp op) {
  return dqmcpp::common::median(list.begin(), list.end(), op);
}

template <typename Collection>
inline double median(const Collection& list) {
  return dqmcpp::common::median(
      list, [](const typename Collection::value_type& e) { return e; });
}

}  // namespace common
}  // namespace dqmcpp

#define COMMON_MATH_HH
#endif  // !COMMON_MATH_HH