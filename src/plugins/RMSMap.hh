#ifndef TESTPLUGIN_HH
/**
 * @file RMSMap.hh
 * @author Grigory Latyshev (thegriglat@gmail.com)
 * @brief ECAL RMS G12 map plugin
 */
#include "Plugin.hh"

namespace dqmcpp {
namespace plugins {

class RMSMap : public Plugin {
 private:
  std::vector<std::string> urls(const unsigned int runnumber,
                                const std::string& dataset);
  std::vector<ECAL::RunChannelData> analyze(
      const std::vector<ECAL::RunChannelData>& rundata);
  void plot(const std::vector<ECAL::RunChannelData>& rundata);

 public:
  void Process(void);
};

}  // namespace plugins
}  // namespace dqmcpp

#define TESTPLUGIN_HH
#endif