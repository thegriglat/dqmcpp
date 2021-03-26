#ifndef COMMON_LISTS_HH
/**
 * @brief File contains list specific templates
 *
 */
#include <algorithm>
#include <functional>
#include <vector>

namespace dqmcpp {
namespace common {

// index

template <typename Iterator, typename BinaryOp>
int index(Iterator begin, Iterator end, BinaryOp index_fn) {
  auto it = std::find_if(begin, end, index_fn);
  if (it == end)
    // element not found
    return -1;
  return std::distance(begin, it);
}

template <typename Collection>
inline int index(
    const Collection& data,
    std::function<bool(const typename Collection::value_type&)> index_fn) {
  return index(data.begin(), data.end(), index_fn);
};

template <typename Collection>
int index(const Collection& data,
          const typename Collection::value_type& element) {
  auto it = std::find(data.begin(), data.end(), element);
  if (it == data.end())
    // element not found
    return -1;
  return std::distance(data.begin(), it);
}

// has

template <typename Iterator, typename BinaryOp>
inline bool has(Iterator begin, Iterator end, BinaryOp op) {
  return (std::find_if(begin, end, op) != end);
}

template <typename Collection>
inline bool has(const Collection& data,
                const typename Collection::value_type& element) {
  return (std::find(data.begin(), data.end(), element) != data.end());
}

template <typename Collection>
inline bool has(
    const Collection& data,
    std::function<bool(const typename Collection::value_type&)> index_fn) {
  return has(data.begin(), data.end(), index_fn);
}

// count
template <typename It, typename BinaryOp>
int count(It begin, It end, BinaryOp op) {
  int __count = 0;
  for (; begin != end; ++begin) {
    if (op(*begin))
      ++__count;
  }
  return __count;
}

template <typename Collection>
inline int count(
    const Collection& list,
    std::function<bool(const typename Collection::value_type&)> op) {
  return dqmcpp::common::count(list.begin(), list.end(), op);
}

template <typename Collection>
int count(const Collection& list,
          const typename Collection::value_type& element) {
  int __count = 0;
  for (auto& e : list) {
    if (e == element)
      ++__count;
  }
  return __count;
}

}  // namespace common
}  // namespace dqmcpp

#define COMMON_LISTS_HH
#endif  // !COMMON_LISTS_HH