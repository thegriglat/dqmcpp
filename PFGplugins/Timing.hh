#ifndef TIMING_HH

#include "plugins/Plugins.hh"

namespace dqmcpp {
namespace plugins {

class Timing : public Plugin {
 public:
  void Process();
};

}  // namespace plugins
}  // namespace dqmcpp

#define TIMING_HH
#endif