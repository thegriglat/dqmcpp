#ifndef CHANNELSTATUSMAP_HH

#include "ECAL/ECAL.hh"
#include "plugins/Plugins.hh"

namespace dqmcpp {
namespace plugins {

class ChannelStatus : public Plugin {
 public:
  void Process();
};

}  // namespace plugins
}  // namespace dqmcpp

#define CHANNELSTATUSMAP_HH
#endif  // !CHANNELSTATUSMAP_HH