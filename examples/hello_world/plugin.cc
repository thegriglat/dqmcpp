#include "plugin.hh"
#include <iostream>

/**
 * Plugin registration:

 * REGISTER_PLUGIN(Example0)
 * in this case plugin name is equal plugin C++ class name
 *
 * REGISTER_PLUGIN(Example0, ex0)
 * in this case plugin will have name ex0 (available from command line)
*/

// do registration
REGISTER_PLUGIN(HelloWorldEx, hello_world)

// change line below to see how plugin name changes
// REGISTER_PLUGIN(HelloWorldEx)

using namespace std;
using namespace dqmcpp;

namespace dqmcpp {

namespace plugins {

void HelloWorldEx::Process() { cout << "Hello, DQMCpp!" << endl; }
} // namespace plugins

} // namespace dqmcpp
