/**
 * @file Vlasov.cc
 * @author Grigory Latyshev (thegriglat@gmail.com)
 * @brief Vlasov table implementation
 *
 */
#include "Vlasov.hh"
#include <algorithm>
#include "../ecalchannels/ECALChannels.hh"

namespace {}

namespace dqmcpp {
namespace filters {
namespace Vlasov {

void VlasovTable::init() {
  _table.clear();
  // dumped 13.09.2021
  _table = {{610, 12}, {611, 13}, {612, 61}, {612, 7},  {613, 1},  {615, 17},
            {615, 37}, {616, 42}, {618, 22}, {618, 34}, {618, 55}, {619, 67},
            {621, 5},  {622, 33}, {622, 52}, {623, 39}, {624, 70}, {625, 53},
            {626, 34}, {626, 66}, {630, 21}, {630, 47}, {633, 6},  {633, 14},
            {633, 43}, {634, 11}, {635, 12}, {636, 42}, {636, 67}, {637, 30},
            {637, 50}, {639, 68}, {643, 30}, {645, 60}, {645, 65}, {607, 6},
            {608, 32}, {601, 32}, {602, 6},  {602, 21}, {603, 16}, {651, 20},
            {652, 23}, {654, 12}, {654, 14}, {654, 19}, {654, 20}};
}

bool Channel::operator()(const ECAL::Channel& channel) const {
  const auto info = ECALChannels::find(channel);
  return std::any_of(_table.begin(), _table.end(), [info](const Item& item) {
    return item.ccu == info->ccu && item.fed == info->fed;
  });
}

bool CCU::operator()(const ECAL::CCU& ccu) const {
  const auto channels = ECALChannels::list();
  const auto any_channel_in_ccu = std::find_if(
      channels.begin, channels.end, [ccu](const ECALChannels::ChannelInfo& ci) {
        return ci.ccu == ccu.ccu && ci.tcc == ccu.tcc && ci.det_iz() == ccu.iz;
      });
  if (any_channel_in_ccu == channels.end)
    return false;
  return std::any_of(_table.begin(), _table.end(),
                     [any_channel_in_ccu](const Item& item) {
                       return item.ccu == any_channel_in_ccu->ccu &&
                              item.fed == any_channel_in_ccu->fed;
                     });
}

}  // namespace Vlasov
}  // namespace filters
}  // namespace dqmcpp