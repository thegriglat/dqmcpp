#ifndef COMMON_LISTS_HH
/**
 * @brief File contains list specific templates
 *
 */

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
template <typename T>
bool has(const std::vector<T>& data, const T& element) {
  return has(data.begin(), data.end(),
             [&element](const T& elem) { return elem == element; });
}
template <typename T, typename BinaryOp>
int has(const std::vector<T>& data, BinaryOp index_fn) {
  return has(data.begin(), data.end(), index_fn);
}

}  // namespace common
}  // namespace dqmcpp

#define COMMON_LISTS_HH
#endif  // !COMMON_LISTS_HH