#ifndef COMMON_FUNCTIONAL_HH
/**
 * @file functional.hh
 * @author Grigory Latyshev (thegriglat@gmail.com)
 * @brief C++ helpers for functional programming
 *
 *
 */
#include <algorithm>
#include <vector>

// copied from https://gist.github.com/phatak-dev/766eccf8c72484ad623b

namespace dqmcpp {
/**
 * @brief Common functions
 *
 */
namespace common {

/**
 * @brief Simple std::for_each for std collections
 *
 * @tparam Collection
 * @tparam unop
 * @param col
 * @param op
 */
template <typename Collection, typename unop>
void for_each(Collection col, unop op) {
  std::for_each(col.begin(), col.end(), op);
}

/**
 * @brief Map function which converts std::vector<T> to std::vector<K>
 *
 * @tparam T
 * @tparam K
 * @tparam unop
 * @param col
 * @param op
 * @return std::vector<K>
 */
template <typename T, typename K, typename unop>
std::vector<K> map(const std::vector<T>& col, unop op) {
  std::vector<K> _tmp;
  _tmp.reserve(col.size());
  std::transform(col.begin(), col.end(), std::back_inserter(_tmp), op);
  return _tmp;
}

/**
 * @brief Zip function
 *
 * @tparam Collection
 * @tparam binop
 * @param fc
 * @param sc
 * @param op
 * @return Collection
 */
template <typename Collection, typename binop>
Collection zip(Collection fc, Collection sc, binop op) {
  std::transform(fc.begin(), fc.end(), sc.begin(), fc.begin(), op);
  return fc;
}

/**
 * @brief Negative filter, needed for filter function
 *
 * @tparam Collection
 * @tparam Predicate
 * @param col Input collection
 * @param predicate filtering function
 * @return Collection
 */
template <typename Collection, typename Predicate>
Collection filterNot(Collection col, Predicate predicate) {
  auto returnIterator = std::remove_if(col.begin(), col.end(), predicate);
  col.erase(returnIterator, std::end(col));
  return col;
}

/**
 * @brief Filter function
 *
 * @tparam Collection
 * @tparam Predicate
 * @param col  Input collection
 * @param predicate filtering function
 * @return Collection
 */
template <typename Collection, typename Predicate>
Collection filter(Collection col, Predicate predicate) {
  auto fnCol = filterNot(col, [predicate](typename Collection::value_type i) {
    return !predicate(i);
  });
  return fnCol;
}

}  // namespace common
}  // namespace dqmcpp

#define COMMON_FUNCTIONAL_HH
#endif