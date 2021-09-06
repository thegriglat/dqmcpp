#ifndef COMMONCLUSTERS_HH

#include <algorithm>
#include <array>
#include <vector>
#include "functional.hh"
#include "lists.hh"
#include "math.hh"

namespace {

template <typename It, typename BinaryOp>
std::vector<std::array<const typename It::value_type*, 2>>
get_pairs(It begin, It end, const double maxdistance, BinaryOp distance_fn) {
  using Ptr = const typename It::value_type*;
  std::vector<std::array<Ptr, 2>> points;
  if (std::distance(begin, end) == 0)
    return points;
  Ptr first = &(*begin);
  const auto len = std::distance(begin, end);
  for (auto i = 0; i < len; ++i) {
    Ptr pi = first + i;
    for (auto j = i + 1; j < len; ++j) {
      Ptr pj = first + j;
      const auto distance = distance_fn(*pi, *pj);
      if (distance > maxdistance)
        continue;
      points.push_back({pi, pj});
    }
  }
  return points;
}

template <typename T>
std::vector<std::vector<T>> clusterize(
    std::vector<std::array<const T*, 2>>& pairs) {
  std::vector<std::vector<T>> clusters;
  if (pairs.size() == 0)
    return clusters;
  auto pend = pairs.end();
  do {
    std::vector<const T*> current_cluster = {pairs.at(0).at(0),
                                             pairs.at(0).at(1)};
    for (auto it = pairs.begin() + 1; it != pend; ++it) {
      const T* p1 = it->at(0);
      const T* p2 = it->at(1);
      auto pos1 = std::find(current_cluster.begin(), current_cluster.end(), p1);
      auto pos2 = std::find(current_cluster.begin(), current_cluster.end(), p2);
      const bool match1 = pos1 != current_cluster.end();
      const bool match2 = pos2 != current_cluster.end();
      if (match1 || match2) {
        // some of point connected with current cluster
        if (!match1) {
          current_cluster.push_back(p1);
        }
        if (!match2) {
          current_cluster.push_back(p2);
        }
      }
    }
    for (auto& p : current_cluster) {
      pend = std::remove_if(pairs.begin(), pend,
                            [&p](const std::array<const T*, 2>& pts) {
                              return pts.at(0) == p || pts.at(1) == p;
                            });
    }
    clusters.push_back(dqmcpp::common::map<const T*, T>(
        current_cluster, [](const T* e) -> T { return *e; }));
  } while (pairs.begin() != pend);
  return clusters;
}

}  // namespace

namespace dqmcpp {
namespace common {

/**
 * @brief Groups input vector to "clusters" based on distance between elements
 *
 * @tparam T std::vector's element type
 * @tparam BinaryOp
 * @param data input std::vector<T>
 * @param maxdistance Distance which means that two elements are together
 * @param distance_fn Function to compute distance from T
 * @return std::vector<std::vector<const T*>> list of lists of const T*
 */

template <typename It, typename BinaryOp>
std::vector<std::vector<typename It::value_type>>
clusters(It begin, It end, const double maxdistance, BinaryOp distance_fn) {
  auto p = get_pairs(begin, end, maxdistance, distance_fn);
  return clusterize(p);
}

/**
 * @brief  Groups input vector to "clusters" based on distance between elements
 *
 * @tparam T
 * @tparam BinaryOp
 * @param data std::vector<T>
 * @param maxdistance   Distance which means that two elements are together
 * @param distance_fn  Function to compute distance from T
 * @return std::vector<std::vector<T>>
 */
template <typename T, typename BinaryOp>
std::vector<std::vector<T>> clusters(const std::vector<T>& data,
                                     const double maxdistance,
                                     BinaryOp distance_fn) {
  return clusters(data.begin(), data.end(), maxdistance, distance_fn);
}

}  // namespace common
}  // namespace dqmcpp

#define COMMONCLUSTERS_HH
#endif