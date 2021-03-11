#ifndef PLUGINFACTORY_HH
/**
 * @file Plugins.hh
 * @author Grigory Latyshev (thegriglat@gmail.com)
 * @brief Namespace to get Plugin* by name
 */
#include <map>
#include <string>
#include <vector>
#include "Plugin.hh"

namespace dqmcpp {
namespace plugins {

Plugin* get(const std::string name);
std::vector<std::string> list();

}  // namespace plugins
}  // namespace dqmcpp
#define PLUGINFACTORY_HH
#endif