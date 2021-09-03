#ifndef RECHITOCCUPANCYDIFF_HH

#include "plugins/Plugins.hh"

namespace dqmcpp {
namespace plugins {

class RecHitOccupancyDiff : public Plugin {
 public:
  void Process();
};

}  // namespace plugins
}  // namespace dqmcpp

#define RECHITOCCUPANCYDIFF_HH
#endif