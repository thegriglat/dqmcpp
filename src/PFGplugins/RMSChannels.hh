#ifndef RMSCHANNELS_HH

#include "ChannelPlugin.hh"

namespace dqmcpp {
namespace plugins {

class RMSChannels : public ChannelPlugin {
 private:
  std::vector<std::string> urls(const ECAL::Run& run);

 public:
  void Process(void);
};

}  // namespace plugins
}  // namespace dqmcpp

#define RMSCHANNELS_HH
#endif  // !RMSCHANNELS_HH