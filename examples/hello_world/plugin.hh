#ifndef HelloWorldEx_HH

// include plugin common header
#include "plugins/Plugins.hh"

// we prefer to use namespaces instead of writing dqmcpp::plugins:: everywhere

namespace dqmcpp {
namespace plugins {

/**
 * Plugin class must be derived from dqmcpp::plugins::Plugin class
 * and have to implement one function
 *   void Process(void);
 *
 * All extra functions/class members can be safely added if needed.
 *
 */
class HelloWorldEx : public Plugin {
public:
  void Process();
};
} // namespace plugins
} // namespace dqmcpp

#define HelloWorldEx_HH
#endif