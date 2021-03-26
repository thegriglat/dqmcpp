#ifndef CHANNELSTATUSMAP_HH

#include "Plugin.hh"

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