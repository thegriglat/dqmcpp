#ifndef ReadChannelsEx_HH

#include "plugins/Plugins.hh"

namespace dqmcpp {
namespace plugins {

class ReadingChannels : public Plugin {
 public:
  void Process();
};
}  // namespace plugins
}  // namespace dqmcpp

#define ReadChannelsEx_HH
#endif