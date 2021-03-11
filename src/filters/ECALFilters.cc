/**
 * @file ECALFilters.cc
 * @author Grigory Latyshev (thegriglat@gmail.com)
 * @brief Implementation of common ECAL filters
 */
#include "ECALFilters.hh"
#include <algorithm>

namespace dqmcpp {
namespace filters {

std::vector<ECAL::ChannelData> filter_det(
    std::vector<ECAL::ChannelData>& channeldata,
    const ECAL::DETECTORS iz) {
  std::vector<ECAL::ChannelData> result;
  if (iz == ECAL::DETECTORS::EB)
    result.reserve(ECAL::NEBChannels);
  else
    result.reserve(ECAL::NEEChannels);
  std::copy_if(
      channeldata.begin(), channeldata.end(), std::back_inserter(result),
      [iz](const ECAL::ChannelData& cd) { return cd.channel.iz == iz; });
  return result;
};

}  // namespace filters
}  // namespace dqmcpp