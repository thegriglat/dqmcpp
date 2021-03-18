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
 public:
  void Process(void);
};

}  // namespace plugins
}  // namespace dqmcpp
#define RMSCHANNELS_HH
#endif