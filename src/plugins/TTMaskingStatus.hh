#ifndef TTF4MASKINGSTATUS_HH
/**
 * @file TTMaskingStatus.hh
 * @author Grigory Latyshev (thegriglat@gmail.com)
 * @brief TT Masking status
 *
 * Can be used in other plugins
 *
 */
#include <vector>
#include "../dataclasses/ECAL.hh"
#include "Plugin.hh"

class TTMaskingStatus : public Plugin {
 private:
  std::vector<ECAL::TTRunData> Init(void);

 public:
  void Process();
  std::vector<ECAL::TTRunData> get(void);
};

#define TTF4MASKINGSTATUS_HH
#endif
