#ifndef NOISECLUSTERS_HH

#include "plugins/Plugins.hh"

/**
 * 1. get rms w different EB/EE limits
 *  1.1 EB: > 3.5
 *  1.2 EE: > 5
 * 2. clusterise
 * 3. remove clusters with size <= 3
 */

namespace dqmcpp {
namespace plugins {

class NoiseClusters : public Plugin {
 public:
  void Process();
};

}  // namespace plugins
}  // namespace dqmcpp

#define NOISECLUSTERS_HH
#endif