#ifndef COMMON_LISTS_HH
/**
 * @brief File contains list specific templates
 *
 */
#include <vector>

namespace dqmcpp {
namespace common {

template <typename Iterator, typename BinaryOp>
int index(Iterator begin, Iterator end, BinaryOp index_fn) {
  auto it = std::find_if(begin, end, index_fn);
  if (it == end)
    // element not found
    return -1;
  return std::distance(begin, it);
}

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
  return index(data.begin(), data.end(), index_fn);
};

template <typename T>
int index(const std::vector<T>& data, const T& element) {
  return index(data.begin(), data.end(),
               [&element](const T& elem) { return elem == element; });
}

template <typename Iterator, typename BinaryOp>
bool has(Iterator begin, Iterator end, BinaryOp op) {
  return (index(begin, end, op) != -1);
}

/**
 * @brief Returns true if vector has element
 *
 * @param data
 * @param elem
 * @return true
 * @return false
 */
template <typename T, typename BinaryOp>
int has(const std::vector<T>& data, BinaryOp index_fn) {
  return has(data.begin(), data.end(), index_fn);
}
template <typename T>
bool has(const std::vector<T>& data, const T& element) {
  return has(data.begin(), data.end(),
             [&element](const T& elem) { return elem == element; });
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

template <typename T, typename BinaryOp>
int count(const std::vector<T>& list, BinaryOp op) {
  return dqmcpp::common::count(list.begin(), list.end(), op);
}

template <typename T>
int count(const std::vector<T>& list, const T& element) {
  return dqmcpp::common::count(list.begin(), list.end(),
                               [&element](const T& e) { return e == element; });
}

}  // namespace common
}  // namespace dqmcpp

#define COMMON_LISTS_HH
#endif  // !COMMON_LISTS_HH