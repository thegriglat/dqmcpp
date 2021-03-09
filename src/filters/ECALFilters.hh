#ifndef ECALFILTERS_HH
/**
 * @file ECALFilters.hh
 * @author Grigory Latyshev (thegriglat@gmail.com)
 * @brief Common used filters for ECAL
 */
#include "../dataclasses/ECAL.hh"

namespace ECALFilters {
/**
 * @brief Keep only barrel channels
 *
 * @param channeldata vector of channel's data
 * @return std::vector<ChannelData>
 */
std::vector<ECAL::ChannelData> filter_det(
    std::vector<ECAL::ChannelData>& channeldata,
    const ECAL::DETECTORS iz);
/**
 * @brief Returns only barrel channels
 *
 * @param channeldata vector of channel's data
 * @return std::vector<ECAL::ChannelData>
 */
inline std::vector<ECAL::ChannelData> barrel(
    std::vector<ECAL::ChannelData>& channeldata) {
  return filter_det(channeldata, ECAL::DETECTORS::EB);
}
/**
 * @brief Returns only EE+ channels
 *
 * @param channeldata vector of channel's data
 * @return std::vector<ECAL::ChannelData>
 */
inline std::vector<ECAL::ChannelData> eeplus(
    std::vector<ECAL::ChannelData>& channeldata) {
  return filter_det(channeldata, ECAL::DETECTORS::EEPLUS);
}
/**
 * @brief Returns only EE- channels
 *
 * @param channeldata vector of channel's data
 * @return std::vector<ECAL::ChannelData>
 */
inline std::vector<ECAL::ChannelData> eeminus(
    std::vector<ECAL::ChannelData>& channeldata) {
  return filter_det(channeldata, ECAL::DETECTORS::EEMINUS);
}
}  // namespace ECALFilters

#define ECALFILTERS_HH
#endif