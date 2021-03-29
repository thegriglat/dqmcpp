/**
 * @file Plugins.cc
 * @author Grigory Latyshev (thegriglat@gmail.com)
 * @brief Functions to get Plugin* by name
 */
#include "Plugins.hh"

using namespace dqmcpp::plugins;

namespace dqmcpp {
namespace plugins {

PluginFactory& PluginFactory::Instance() {
  static PluginFactory instance;
  return instance;
}

void PluginFactory::Register(IPluginRegistrar* registrar, std::string name) {
  registry_[name] = registrar;
}

std::unique_ptr<Plugin> PluginFactory::GetPlugin(std::string name) {
  /* throws out_of_range if plugin unknown */
  IPluginRegistrar* registrar = registry_.at(name);
  return registrar->GetPlugin();
}

}  // namespace plugins
}  // namespace dqmcpp