#ifndef CHANNELPLUGIN_HH
/**
 * @file RMSChannels.hh
 * @author Grigory Latyshev (thegriglat@gmail.com)
 * @brief ECAL RMS G12 plugin
 */
#include "Plugins.hh"

#include <functional>
#include <string>
#include <vector>
#include "../ECAL/ECAL.hh"
#include "../writers/Gnuplot2D.hh"

namespace dqmcpp {
namespace plugins {

class ChannelPlugin : public Plugin {
 private:
  virtual std::vector<std::string> urls(const ECAL::Run& run);

 public:
  static std::vector<ECAL::RunChannelData> analyze(
      const std::vector<ECAL::RunChannelData>& rundata,
      std::function<bool(double)> checkfn);

  static void plot(const std::vector<ECAL::RunChannelData>& rundata,
                   const std::string& filename,
                   const std::string& title,
                   const writers::Axis& zaxis,
                   const writers::Palette& palette);
  std::vector<ECAL::RunChannelData> getRunData(void);

 public:
  void Process(void);
};

}  // namespace plugins
}  // namespace dqmcpp
#define CHANNELPLUGIN_HH
#endif