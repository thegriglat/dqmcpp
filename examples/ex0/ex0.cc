#include "ex0.hh"

#include <iostream>

REGISTER_PLUGIN(Example0, ex0)

using namespace std;
using namespace dqmcpp;

namespace dqmcpp {

namespace plugins {

void Example0::Process() { cout << "Hello, DQMCpp!" << endl; }
} // namespace plugins

} // namespace dqmcpp
