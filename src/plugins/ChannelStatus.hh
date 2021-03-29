#ifndef CHANNELSTATUSMAP_HH

#include "../ECAL/ECAL.hh"
#include "Plugins.hh"

namespace dqmcpp {
namespace plugins {

class ChannelStatus : public Plugin {
 public:
  void Process();
  static std::vector<ECAL::ChannelData> getChannelStatus(const ECAL::Run& run);
  static int getChannelStatus(const ECAL::Run& run,
                              const ECAL::Channel& channel);
};

}  // namespace plugins
}  // namespace dqmcpp

#define CHANNELSTATUSMAP_HH
#endif  // !CHANNELSTATUSMAP_HH