#ifndef PLUGINFACTORY_HH

#include <map>
#include <string>
#include <vector>
#include "Plugin.hh"

namespace Plugins {
Plugin* get(const std::string name);
std::vector<std::string> list();
};  // namespace Plugins

#define PLUGINFACTORY_HH
#endif