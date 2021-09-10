#ifndef EELIGHTMONITORINGREGIONS_HH
/**
 * @file EELightMonitoringRegions.hh
 * @author Grigory Latyshev (thegriglat@gmail.com)
 * @brief Light Monitoring regions
 *
 *
 */
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
unsigned int EELightMR(const int x, const int y);
std::vector<std::pair<int, int>> EELightMRChannels(const unsigned int region);

}  // namespace ECAL
}  // namespace dqmcpp

#define EELIGHTMONITORINGREGIONS_HH
#endif  // !EELIGHTMONITORINGREGIONS_HH