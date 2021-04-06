#ifndef READOUTFLAGDROPPED_HH

#include "plugins/Plugins.hh"

namespace dqmcpp {
namespace plugins {

class ReadoutFlagDropped : public Plugin {
 public:
  void Process();
};
}  // namespace plugins
}  // namespace dqmcpp

#define READOUTFLAGDROPPED_HH
#endif