#ifndef EELIGHTMONITORINGREGIONS_HH

#include <map>
#include <vector>

namespace dqmcpp {
namespace ECAL {

/**
 * @brief Return EE LightMonitoringRegion number
 *
 * @param x channel coordinate
 * @param y channel coordinate
 * @return uint8_t
 */
uint8_t EELightMR(const int x, const int y);
std::vector<std::pair<int, int>> EELightMRChannels(const uint8_t region);

}  // namespace ECAL
}  // namespace dqmcpp

#define EELIGHTMONITORINGREGIONS_HH
#endif  // !EELIGHTMONITORINGREGIONS_HH