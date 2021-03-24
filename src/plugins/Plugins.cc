/**
 * @file Plugins.cc
 * @author Grigory Latyshev (thegriglat@gmail.com)
 * @brief Functions to get Plugin* by name
 */
#include "Plugins.hh"

#include "FEErrorClusterSize.hh"
#include "FEStatusBits.hh"
#include "L1TEGammaIsoPre.hh"
#include "MeanChannels.hh"
#include "RMSChannels.hh"
#include "RMSMap.hh"
#include "TPOccupancyL1.hh"
#include "TPOccupancyLTAmplitude.hh"
#include "TTF4Occupancy.hh"
#include "TTMaskingStatus.hh"
#include "Timing.hh"

namespace {

using namespace dqmcpp::plugins;

const std::map<std::string, Plugin*> _pluginmap = {
    {"RMSChannels", new RMSChannels()},
    {"MeanChannels", new MeanChannels()},
    {"RMSMap", new RMSMap()},
    {"TPOccupancyL1", new TPOccupancyL1()},
    {"TPOccupancyLTAmp", new TPOccupancyLTAmplitude()},
    {"TTMaskingStatus", new TTMaskingStatus()},
    {"TTF4Occupancy", new TTF4Occupancy()},
    {"L1TEGammaIsoPre", new L1TEGammaIsoPrePlugin()},
    {"Timing", new Timing()},
    {"FEErrorClusterSize", new FEErrorClusterSize()},
    {"FEStatusBits", new FEStatusBits()}

};

}  // namespace

namespace dqmcpp {
namespace plugins {

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
};

}  // namespace plugins
}  // namespace dqmcpp