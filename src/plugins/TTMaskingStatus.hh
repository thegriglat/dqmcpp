#ifndef TTF4MASKINGSTATUS_HH
/**
 * @file TTMaskingStatus.hh
 * @author Grigory Latyshev (thegriglat@gmail.com)
 * @brief TT Masking status plugin
 *
 * Can be used in other plugins
 *
 */
#include <vector>
#include "../ECAL/ECAL.hh"
#include "Plugin.hh"

namespace dqmcpp {
namespace plugins {

class TTMaskingStatus : public Plugin {
 private:
  std::vector<ECAL::RunTTData> Init(void);

 public:
  void Process();
  std::vector<ECAL::RunTTData> get(void);
};

}  // namespace plugins
}  // namespace dqmcpp
#define TTF4MASKINGSTATUS_HH
#endif
