#ifndef L1TEGAMMAISOPRE_HH
/**
 * @file L1TEGammaIsoPre.hh
 * @author Grigory Latyshev (thegriglat@gmail.com)
 * @brief L1TEGammaIsoPrePlugin
 */
#include "Plugins.hh"
namespace dqmcpp {
namespace plugins {

class L1TEGammaIsoPrePlugin : public Plugin {
 public:
  void Process();
};

}  // namespace plugins
}  // namespace dqmcpp

#define L1TEGAMMAISOPRE_HH
#endif  // !L1TEGAMMAISOPRE_HH
