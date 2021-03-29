#ifndef FEERRORCLUSTERSIZE_HH

#include "Plugins.hh"

namespace dqmcpp {
namespace plugins {

class FEErrorClusterSize : public Plugin {
 public:
  void Process();
};

}  // namespace plugins
}  // namespace dqmcpp

#define FEERRORCLUSTERSIZE_HH
#endif  // !FEERRORCLUSTERSIZE_HH