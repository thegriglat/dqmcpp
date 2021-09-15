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

Vlasov::Vlasov() {
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

bool Vlasov::operator()(const ECAL::Channel& channel) const {
  const auto info = ECALChannels::find(channel);
  if (!info)
    return false;
  return std::any_of(_table.begin(), _table.end(), [info](const Item& item) {
    return item.ccu == info->ccu && item.fed == info->fed;
  });
}

bool Vlasov::operator()(const ECAL::CCU& ccu) const {
  const int _ccu = ccu.ccu;
  const int _fed = ccu.fed();
  return std::any_of(_table.begin(), _table.end(),
                     [_ccu, _fed](const Item& item) {
                       return item.ccu == _ccu && item.fed == _fed;
                     });
}

}  // namespace filters
}  // namespace dqmcpp