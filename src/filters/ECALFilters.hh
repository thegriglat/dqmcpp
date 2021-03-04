#ifndef ECALFILTERS_HH

#include "../dataclasses/ECAL.hh"

namespace ECALFilters
{
/**
 * @brief Keep only barrel channels
 *
 * @param channeldata
 * @return std::vector<ChannelData>
 */
std::vector<ECAL::ChannelData> filter_det(std::vector<ECAL::ChannelData> &channeldata, const ECAL::DETECTORS iz);
/**
 * @brief Returns only barrel channels
 *
 * @param channeldata
 * @return std::vector<ECAL::ChannelData>
 */
inline std::vector<ECAL::ChannelData> barrel(std::vector<ECAL::ChannelData> &channeldata)
{
    return filter_det(channeldata, ECAL::DETECTORS::EB);
}
/**
 * @brief Returns only EE+ channels
 *
 * @param channeldata
 * @return std::vector<ECAL::ChannelData>
 */
inline std::vector<ECAL::ChannelData> eeplus(std::vector<ECAL::ChannelData> &channeldata)
{
    return filter_det(channeldata, ECAL::DETECTORS::EEPLUS);
}
/**
 * @brief Returns only EE- channels
 *
 * @param channeldata
 * @return std::vector<ECAL::ChannelData>
 */
inline std::vector<ECAL::ChannelData> eeminus(std::vector<ECAL::ChannelData> &channeldata)
{
    return filter_det(channeldata, ECAL::DETECTORS::EEMINUS);
}
}

#define ECALFILTERS_HH
#endif