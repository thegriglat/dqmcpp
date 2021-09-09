#ifndef RequestEx_HH
#include "plugins/Plugins.hh"

namespace dqmcpp {
namespace plugins {

class RequestEx : public Plugin {
 public:
  void Process();
};
}  // namespace plugins
}  // namespace dqmcpp

#define RequestEx_HH
#endif