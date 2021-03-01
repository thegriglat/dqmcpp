#include "ECALFilters.hh"
#include <algorithm>

namespace ECALFilters
{
std::vector<ECALHardware::ChannelData> filter_det(std::vector<ECALHardware::ChannelData> &channeldata, const ECALHardware::DETECTORS iz)
{
    std::vector<ECALHardware::ChannelData> result;
    if (iz == ECALHardware::DETECTORS::EB)
        result.reserve(ECALHardware::NEBChannels);
    else
        result.reserve(ECALHardware::NEEChannels);
    std::copy_if(channeldata.begin(), channeldata.end(), std::back_inserter(result),
    [iz](const ECALHardware::ChannelData &cd) {
        return cd.channel.iz == iz;
    });
    return result;
};
}