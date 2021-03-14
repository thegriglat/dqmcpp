#ifndef ECALCHANNELS_DB_HH

#include "ECALChannels.hh"

namespace dqmcpp {
namespace ECALChannels {

struct ChannelsDB {
  static const ECALChannelsList channels;
};

}  // namespace ECALChannels
}  // namespace dqmcpp

#define ECALCHANNELS_DB_HH
#endif