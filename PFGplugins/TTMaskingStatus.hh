#ifndef TTMASKINGSTATUS_HH
/**
 * @file TTMaskingStatus.hh
 * @author Grigory Latyshev (thegriglat@gmail.com)
 * @brief TT Masking status plugin
 *
 * Can be used in other plugins
 *
 */
#include <vector>
#include "ECAL/ECAL.hh"
#include "plugins/Plugins.hh"

namespace dqmcpp {
namespace plugins {

class TTMaskingStatus : public Plugin {
 private:
  std::vector<ECAL::RunTTData> Init(void) const;

 public:
  void Process();
  std::vector<ECAL::RunTTData> get(void);
};

}  // namespace plugins
}  // namespace dqmcpp

#define TTMASKINGSTATUS_HH
#endif
