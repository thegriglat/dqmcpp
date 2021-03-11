#ifndef RMSCHANNELS_HH
/**
 * @file RMSChannels.hh
 * @author Grigory Latyshev (thegriglat@gmail.com)
 * @brief ECAL RMS G12 plugin
 */
#include "Plugin.hh"

namespace dqmcpp {
namespace plugins {

class RMSPlugin : public Plugin {
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
#define RMSCHANNELS_HH
#endif