#ifndef CHID_HH
#include "plugins/Plugins.hh"

namespace dqmcpp {
namespace plugins {
class ChId : public Plugin {
 public:
  void Process();
};
}  // namespace plugins
}  // namespace dqmcpp

#define CHID_HH
#endif