#ifndef COMMONCLUSTERS_HH

#include <array>
#include <vector>
#include "../common/lists.hh"

namespace {

template <typename T, typename BinaryOp>
std::vector<std::array<T, 2>> get_pairs(const std::vector<T>& data,
                                        const double maxdistance,
                                        BinaryOp distance_fn) {
  std::vector<std::array<T, 2>> points;
  for (auto it = data.begin(); it != data.end(); ++it) {
    for (auto jit = it + 1; jit != data.end(); ++jit) {
      const auto distance = distance_fn(*it, *jit);
      if (distance > maxdistance)
        continue;
      T p1 = *it;
      T p2 = *jit;
      points.push_back({p1, p2});
    }
  }
  return points;
}

template <typename T>
std::vector<std::vector<T>> clusterize(std::vector<std::array<T, 2>>& pairs) {
  std::vector<std::vector<T>> clusters;
  if (pairs.size() == 0)
    return clusters;
  do {
    std::vector<T> current_cluster = {pairs.at(0).at(0), pairs.at(0).at(1)};
    for (auto it = pairs.begin() + 1; it != pairs.end(); ++it) {
      const auto& p1 = it->at(0);
      const auto& p2 = it->at(1);
      auto pos1 = std::find(current_cluster.begin(), current_cluster.end(), p1);
      auto pos2 = std::find(current_cluster.begin(), current_cluster.end(), p2);
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
        [&current_cluster](const std::array<T, 2>& pts) {
          return dqmcpp::common::has(current_cluster, pts.at(0)) ||
                 dqmcpp::common::has(current_cluster, pts.at(1));
        });
    pairs.erase(removeit, pairs.end());
  } while (pairs.size() > 0);
  return clusters;
}

}  // namespace

namespace dqmcpp {
namespace common {

template <typename T, typename BinaryOp>
std::vector<std::vector<T>> clusters(const std::vector<T>& data,
                                     const double maxdistance,
                                     BinaryOp distance_fn) {
  auto pairs = get_pairs(data, maxdistance, distance_fn);
  return clusterize(pairs);
}

}  // namespace common
}  // namespace dqmcpp

#define COMMONCLUSTERS_HH
#endif