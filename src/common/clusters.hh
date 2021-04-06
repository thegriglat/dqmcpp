#ifndef COMMONCLUSTERS_HH

#include <algorithm>
#include <array>
#include <vector>
#include "../common/lists.hh"
#include "../common/math.hh"

namespace {

template <typename T, typename BinaryOp>
std::vector<std::array<T, 2>> get_pairs(const std::vector<T>& data,
                                        const double maxdistance,
                                        BinaryOp distance_fn) {
  std::vector<std::array<T, 2>> points;
  if (data.size() == 0)
    return points;
  const auto len = data.size();
  const T* first = &(data[0]);
  for (unsigned int i = 0; i < len; ++i)
    for (unsigned int j = i + 1; j < len; ++j) {
      const T* pi = first + i;
      const T* pj = first + j;
      const auto distance = distance_fn(*pi, *pj);
      if (distance > maxdistance)
        continue;
      points.push_back({*pi, *pj});
    }
  return points;
}

template <typename T, typename BinaryOp>
std::vector<std::vector<T>> clusterize(std::vector<std::array<T, 2>>& pairs,
                                       BinaryOp distance_fn) {
  std::vector<std::vector<T>> clusters;
  if (pairs.size() == 0)
    return clusters;
  do {
    std::vector<T> current_cluster = {pairs.at(0).at(0), pairs.at(0).at(1)};
    for (auto it = pairs.begin() + 1; it != pairs.end(); ++it) {
      const T& p1 = it->at(0);
      const T& p2 = it->at(1);
      auto pos1 =
          std::find_if(current_cluster.begin(), current_cluster.end(),
                       [&p1, distance_fn](const T& e) {
                         return dqmcpp::common::isZero(distance_fn(e, p1));
                       });
      auto pos2 =
          std::find_if(current_cluster.begin(), current_cluster.end(),
                       [&p2, distance_fn](const T& e) {
                         return dqmcpp::common::isZero(distance_fn(p2, e));
                       });
      const bool match1 = pos1 != current_cluster.end();
      const bool match2 = pos2 != current_cluster.end();
      if (match1 || match2) {
        // some of point connected with current cluster
        if (!match1) {
          current_cluster.push_back(p1);
        }
        if (!match2)
          current_cluster.push_back(p2);
      }
    }
    clusters.push_back(current_cluster);
    // remove added points
    auto removeit = std::remove_if(
        pairs.begin(), pairs.end(),
        [&current_cluster, distance_fn](const std::array<T, 2>& pts) {
          return dqmcpp::common::has(
              current_cluster, [&pts, distance_fn](const T& e) {
                return dqmcpp::common::isZero(distance_fn(pts.at(0), e) *
                                              distance_fn(pts.at(1), e));
              });
        });
    pairs.erase(removeit, pairs.end());
  } while (pairs.size() > 0);
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
 * @param distance_fn Function to compute distance from T, 0 means that elements
 * are equal
 * @return std::vector<std::vector<const T*>> list of lists of const T*
 */
template <typename T, typename BinaryOp>
std::vector<std::vector<T>> clusters(const std::vector<T>& data,
                                     const double maxdistance,
                                     BinaryOp distance_fn) {
  auto pairs = get_pairs(data, maxdistance, distance_fn);
  return clusterize(pairs, distance_fn);
}

}  // namespace common
}  // namespace dqmcpp

#define COMMONCLUSTERS_HH
#endif