#ifndef MEANCHANNELS_HH

#include "ChannelPlugin.hh"

namespace dqmcpp {
namespace plugins {

class MeanChannels : public ChannelPlugin {
 private:
  std::vector<std::string> urls(const ECAL::Run& run);

 public:
  void Process(void);
};

}  // namespace plugins
}  // namespace dqmcpp

#define MEANCHANNELS_HH
#endif  // !MEANCHANNELS_HH