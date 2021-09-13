/**
 * @file filters.hh
 * @author Grigory Latyshev (thegriglat@gmail.com)
 * @brief All predefined filters filters
 *
 */
#ifndef FILTERS_HH
#define FILTERS_HH

#include "Vlasov.hh"

namespace dqmcpp {
/**
 * @brief Predefined filters for TT/CCU/channels
 *
 */
namespace filters {

/**
 * @brief Function to filter vector by function, lambda function or functor
 *
 * @tparam T
 * @tparam Op
 * @param list
 * @param op
 */
template <typename T, typename Op>
void filter(std::vector<T>& list, Op op) {
  list.erase(std::remove_if(list.begin(), list.end(),
                            [op](const auto e) { return !op(e); }),
             list.end());
}
}  // namespace filters
}  // namespace dqmcpp
#endif