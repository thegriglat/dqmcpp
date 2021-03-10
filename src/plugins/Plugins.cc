/**
 * @file Plugins.cc
 * @author Grigory Latyshev (thegriglat@gmail.com)
 * @brief Functions to get Plugin* by name
 */
#include "Plugins.hh"

#include "L1TEGammaIsoPre.hh"
#include "RMSChannels.hh"
#include "TTF4Occupancy.hh"
#include "TTMaskingStatus.hh"
#include "TestPlugin.hh"

namespace Plugins {

static const std::map<std::string, Plugin*> _pluginmap = {
    {"RMS", new RMSPlugin()},
    {"Test", new TestPlugin()},
    {"TTMaskingStatus", new TTMaskingStatus()},
    {"TTF4Occupancy", new TTF4Occupancy()},
    {"L1TEGammaIsoPre", new L1TEGammaIsoPrePlugin()}

};

Plugin* get(const std::string name) {
  auto it = _pluginmap.find(name);
  if (it != _pluginmap.end())
    return it->second;
  std::cerr << "Plugin '" << name << "' not found!" << std::endl;
  return nullptr;
}

std::vector<std::string> list() {
  std::vector<std::string> list;
  for (auto& e : _pluginmap) {
    list.push_back(e.first);
  }
  return list;
}
}  // namespace Plugins