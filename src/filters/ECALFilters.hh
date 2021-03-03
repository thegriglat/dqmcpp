#ifndef ECALFILTERS_HH

#include "../dataclasses/ECALHardware.hh"

namespace ECALFilters
{
/**
 * @brief Keep only barrel channels
 *
 * @param channeldata
 * @return std::vector<ChannelData>
 */
std::vector<ECALHardware::ChannelData> filter_det(std::vector<ECALHardware::ChannelData> &channeldata, const ECALHardware::DETECTORS iz);
/**
 * @brief Returns only barrel channels
 *
 * @param channeldata
 * @return std::vector<ECALHardware::ChannelData>
 */
inline std::vector<ECALHardware::ChannelData> barrel(std::vector<ECALHardware::ChannelData> &channeldata)
{
    return filter_det(channeldata, ECALHardware::DETECTORS::EB);
}
/**
 * @brief Returns only EE+ channels
 *
 * @param channeldata
 * @return std::vector<ECALHardware::ChannelData>
 */
inline std::vector<ECALHardware::ChannelData> eeplus(std::vector<ECALHardware::ChannelData> &channeldata)
{
    return filter_det(channeldata, ECALHardware::DETECTORS::EEPLUS);
}
/**
 * @brief Returns only EE- channels
 *
 * @param channeldata
 * @return std::vector<ECALHardware::ChannelData>
 */
inline std::vector<ECALHardware::ChannelData> eeminus(std::vector<ECALHardware::ChannelData> &channeldata)
{
    return filter_det(channeldata, ECALHardware::DETECTORS::EEMINUS);
}
}

#define ECALFILTERS_HH
#endif