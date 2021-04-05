#ifndef TTID_HH
#include "plugins/Plugins.hh"

namespace dqmcpp {
namespace plugins {
class TTid : public Plugin {
 public:
  void Process();
};
}  // namespace plugins
}  // namespace dqmcpp

#define TTID_HH
#endif