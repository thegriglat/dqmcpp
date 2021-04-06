#ifndef ECALCHANNELS_DB_HH

#include "ECALChannels.hh"

namespace dqmcpp {
namespace ECALChannels {

namespace ChannelsDB {
const ECALChannelsList* channels();
const ChannelInfo* find(const int ix_iphi, const int iy_ieta, const int iz);

inline const ChannelInfo* find(const ECAL::Channel& c) {
  return find(c.ix_iphi, c.iy_ieta, c.iz);
};
};  // namespace ChannelsDB

}  // namespace ECALChannels
}  // namespace dqmcpp

#define ECALCHANNELS_DB_HH
#endif