/**
 * @file ECALChannels.cc
 * @author Grigory Latyshev (thegriglat@gmail.com)
 * @brief ECAL channels database functions
 */
#include "ECALChannels.hh"
#include "ecalchannels_db.hh"

using namespace std;

namespace dqmcpp {
namespace ECALChannels {
ECALChannelsList::const_iterator find(const ECAL::Channel& c) {
  return ChannelsDB::find(c.ix_iphi, c.iy_ieta, c.iz);
};

ChannelIteratorPair list() {
  auto l = ChannelsDB::channels();
  return {.begin = l->begin(), .end = l->end()};
}

}  // namespace ECALChannels
}  // namespace dqmcpp