#ifndef DIGIOCCUPANCY_HH

#include "plugins/Plugins.hh"

namespace dqmcpp {
namespace plugins {

class DigiOccupancy : public Plugin {
 public:
  void Process();
};

}  // namespace plugins
}  // namespace dqmcpp

#define DIGIOCCUPANCY_HH
#endif