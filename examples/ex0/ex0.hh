#ifndef EX0_HH

// include plugin common header
#include "plugins/Plugins.hh"

// we prefer to use namespaces instead of writing dump dqmcpp::plugins:: everywhere

namespace dqmcpp {
namespace plugins {

/**
 * Your plugin class must be derived from dqmcpp::plugins::Plugin class
 * and have to implement one function
 *   void Process(void);
 * 
 * All extra functions can be safely added if needed.
 * 
 */
class Example0 : public Plugin {
 public:
  void Process();
};
}  // namespace plugins
}  // namespace dqmcpp

#define EX0_HH
#endif