#include "EELightMonitoringRegions.hh"

#include <array>
#include <cmath>

namespace {
using EELightmonitoringRegions = std::array<std::array<uint8_t, 10>, 20>;
// mirrored against x == 50
const EELightmonitoringRegions EELightMonitoringRegions = {{

    {0, 0, 0, 0, 0, 0, 0, 18, 18, 18},
    {0, 0, 0, 0, 16, 16, 16, 18, 18, 18},
    {0, 0, 0, 15, 15, 15, 16, 18, 18, 18},
    {0, 0, 14, 15, 15, 15, 16, 16, 19, 19},
    {0, 14, 14, 14, 14, 15, 16, 16, 19, 19},
    {0, 12, 14, 14, 14, 15, 16, 17, 19, 19},
    {0, 12, 13, 13, 14, 15, 17, 17, 17, 19},
    {12, 12, 13, 13, 13, 11, 17, 17, 17, 19},
    {12, 12, 13, 10, 10, 11, 11, 17, 17, 0},
    {12, 9, 9, 10, 10, 11, 11, 11, 0, 0},
    {9, 9, 9, 10, 10, 11, 11, 7, 0, 0},
    {9, 9, 8, 6, 6, 7, 7, 7, 7, 0},
    {8, 8, 8, 6, 6, 7, 7, 7, 4, 4},
    {0, 8, 8, 5, 6, 6, 7, 4, 4, 4},
    {0, 8, 8, 5, 6, 6, 4, 4, 4, 3},
    {0, 8, 5, 5, 5, 3, 3, 3, 3, 3},
    {0, 0, 5, 5, 5, 3, 3, 2, 2, 2},
    {0, 0, 0, 5, 2, 2, 2, 2, 2, 1},
    {0, 0, 0, 0, 2, 1, 1, 1, 1, 1},
    {0, 0, 0, 0, 0, 0, 0, 1, 1, 1}

}};
}  // namespace

namespace dqmcpp {
namespace ECAL {

uint8_t EELightMR(const int x, const int y) {
  if (x < 0 || x > 100)
    return 0;
  if (y < 0 || y > 100)
    return 0;
  int xtt = x - 1;
  if (x > 50)
    xtt = 50 - std::abs(x - 50);
  xtt = xtt / 5;
  const int ytt = (y - 1) / 5;
  const auto region = EELightMonitoringRegions.at(ytt).at(xtt);
  return region;
};

std::vector<std::pair<int, int>> EELightMRChannels(const uint8_t region) {
  std::vector<std::pair<int, int>> channels;
  for (int y = 0; y <= 20; ++y) {
    for (int x = 0; x <= 10; ++x) {
      if (EELightMonitoringRegions.at(y).at(x) == region) {
        channels.push_back({x, y});
      }
    }
  }
  return channels;
}

}  // namespace ECAL
}  // namespace dqmcpp