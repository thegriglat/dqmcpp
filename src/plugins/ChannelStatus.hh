#ifndef CHANNELSTATUSMAP_HH

#include "../ECAL/ECAL.hh"
#include "Plugin.hh"

namespace dqmcpp {
namespace plugins {

class ChannelStatus : public Plugin {
 private:
  std::vector<ECAL::ChannelData> getChannelStatus(const ECAL::Run& run,
                                                  const int iz) const;

 public:
  void Process();
  std::vector<ECAL::ChannelData> getChannelStatus(const ECAL::Run& run) const;
  int getChannelStatus(const ECAL::Run& run,
                       const ECAL::Channel& channel) const;
};
}  // namespace plugins
}  // namespace dqmcpp

#define CHANNELSTATUSMAP_HH
#endif  // !CHANNELSTATUSMAP_HH