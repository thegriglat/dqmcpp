#ifndef COMMON_LISTS_HH
/**
 * @brief File contains list specific templates
 *
 */
#include <algorithm>
#include <functional>
#include <vector>
#include "ThreadPool.hh"

namespace dqmcpp {
namespace common {

// index

/**
 * @brief Returns index of first matched element in list
 *
 * @tparam Iterator
 * @tparam BinaryOp
 * @param begin
 * @param end
 * @param index_fn
 * @return int
 */
template <typename Iterator, typename BinaryOp>
int index(Iterator begin, Iterator end, BinaryOp index_fn) {
  auto it = std::find_if(begin, end, index_fn);
  if (it == end)
    // element not found
    return -1;
  return std::distance(begin, it);
}

/**
 * @brief Returns index of first matched element by function in list
 *
 * @tparam Collection
 * @param data
 * @param index_fn
 * @return int
 */
template <typename Collection>
int index(
    const Collection& data,
    std::function<bool(const typename Collection::value_type&)> index_fn) {
  return index(data.begin(), data.end(), index_fn);
};

/**
 * @brief Returns index of first matched element in list
 *
 * @tparam Collection
 * @param data
 * @param element
 * @return int
 */
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

/**
 * @brief Returns true if list has element matching function
 *
 * @tparam Iterator
 * @tparam BinaryOp
 * @param begin
 * @param end
 * @param op
 * @return true
 * @return false
 */
template <typename Iterator, typename BinaryOp>
bool has(Iterator begin, Iterator end, BinaryOp op) {
  return (std::find_if(begin, end, op) != end);
}

/**
 * @brief Returns true if list has element
 *
 * @tparam Collection
 * @param data
 * @param element
 * @return true
 * @return false
 */
template <typename Collection>
bool has(const Collection& data,
         const typename Collection::value_type& element) {
  return (std::find(data.begin(), data.end(), element) != data.end());
}

/**
 * @brief Returns true if list has element matching function
 *
 * @tparam Collection
 * @param data
 * @param index_fn
 * @return true
 * @return false
 */
template <typename Collection>
bool has(const Collection& data,
         std::function<bool(const typename Collection::value_type&)> index_fn) {
  return has(data.begin(), data.end(), index_fn);
}

// count
/**
 * @brief Return number of matching elements in list
 *
 * @tparam It
 * @tparam BinaryOp
 * @param begin
 * @param end
 * @param op
 * @return int
 */
template <typename It, typename BinaryOp>
int count(It begin, It end, BinaryOp op) {
  return std::count(begin, end, op);
}

/**
 * @brief Return number of matching elements in list
 *
 * @tparam Collection
 * @param list
 * @param op
 * @return int
 */
template <typename Collection>
int count(const Collection& list,
          std::function<bool(const typename Collection::value_type&)> op) {
  return dqmcpp::common::count(list.begin(), list.end(), op);
}

/**
 * @brief Return number of matching elements
 *
 * @tparam Collection
 * @param list
 * @param element
 * @return int
 */
template <typename Collection>
int count(const Collection& list,
          const typename Collection::value_type& element) {
  return std::count(list.begin(), list.end(), element);
}

/**
 * @brief Split input list to chunks with given size n
 *
 * @tparam T
 * @param list
 * @param n
 * @return std::vector<std::vector<T>>
 */
template <typename T>
std::vector<std::vector<T>> chunks(const std::vector<T>& list,
                                   const unsigned int n) {
  std::vector<std::vector<T>> res;
  const auto size = list.size();
  unsigned int i = 0;
  for (i = 0; i < size / n; ++i) {
    const std::vector<T> slice(list.begin() + i * n,
                               list.begin() + (i + 1) * n);
    res.push_back(slice);
  }
  // add tail
  const std::vector<T> slicetail(list.begin() + (i)*n, list.end());
  res.push_back(slicetail);
  return res;
}

/**
 * @brief std::for_each clone with multithreading. Uses as much CPU as possible.
 *
 * @tparam It
 * @tparam BinaryOp
 * @param begin
 * @param end
 * @param op
 */
template <typename It, typename BinaryOp>
void foreach_mt(It begin, It end, BinaryOp op) {
  ThreadPool pool;
  while (begin != end) {
    pool.enqueue(op, *begin++);
  };
}

}  // namespace common
}  // namespace dqmcpp

#define COMMON_LISTS_HH
#endif  // !COMMON_LISTS_HH