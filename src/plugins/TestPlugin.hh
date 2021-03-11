#ifndef TESTPLUGIN_HH
/**
 * @file TestPlugin.hh
 * @author Grigory Latyshev (thegriglat@gmail.com)
 * @brief ECAL RMS G12 map plugin
 */
#include "Plugin.hh"

namespace dqmcpp {
namespace plugins {

class TestPlugin : public Plugin {
 private:
  std::vector<std::string> urls(const unsigned int runnumber,
                                const std::string& dataset);
  std::vector<ECAL::RunData> analyze(const std::vector<ECAL::RunData>& rundata);
  void plot(const std::vector<ECAL::RunData>& rundata);

 public:
  void Process(void);
};

}  // namespace plugins
}  // namespace dqmcpp

#define TESTPLUGIN_HH
#endif