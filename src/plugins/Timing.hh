#ifndef TIMING_HH

#include "Plugin.hh"

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