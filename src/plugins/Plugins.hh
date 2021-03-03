#ifndef PLUGINFACTORY_HH

#include <string>
#include <vector>
#include <map>
#include "Plugin.hh"

namespace Plugins
{
Plugin *get(const std::string name);
std::vector<std::string> list();
};

#define PLUGINFACTORY_HH
#endif