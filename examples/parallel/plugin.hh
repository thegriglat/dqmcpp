#ifndef ParallelEx_HH

#include "plugins/Plugins.hh"

namespace dqmcpp {
namespace plugins {

class ParallelEx : public Plugin {
public:
  void Process();
};
} // namespace plugins
} // namespace dqmcpp

#define ParallelEx_HH
#endif