/**
 * @file Plugins.cc
 * @author Grigory Latyshev (thegriglat@gmail.com)
 * @brief Functions to get Plugin* by name
 */
#include "Plugins.hh"
#include <iostream>

using namespace dqmcpp::plugins;
using namespace std;

namespace dqmcpp {
namespace plugins {

std::string Plugin::descrtiption() const {
  return "Empty description";
}

PluginFactory& PluginFactory::Instance() {
  static PluginFactory instance;
  return instance;
}

void PluginFactory::Register(IPluginRegistrar* registrar, std::string name) {
  registry_[name] = registrar;
}

std::unique_ptr<Plugin> PluginFactory::GetPlugin(std::string name) {
  /* throws out_of_range if plugin unknown */
  auto it = registry_.find(name);
  if (it == registry_.end()) {
    std::cerr << "Cannot find plugin '" << name << "'" << std::endl;
    exit(1);
  }
  IPluginRegistrar* registrar = registry_.at(name);
  return registrar->GetPlugin();
}

}  // namespace plugins
}  // namespace dqmcpp