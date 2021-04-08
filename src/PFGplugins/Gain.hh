#ifndef GAIN_HH
#include "plugins/Plugins.hh"

namespace dqmcpp {
namespace plugins {
class Gain : public Plugin {
 public:
  void Process();
};
}  // namespace plugins
}  // namespace dqmcpp

#define GAIN_HH
#endif