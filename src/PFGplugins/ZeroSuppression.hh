#ifndef ZEROSUPPRESSION_HH

#include "ReadoutFlags.hh"

namespace dqmcpp {
namespace plugins {

class ZeroSuppression : public ReadoutFlags {
 private:
  inline std::string pluginName() const { return "ZeroSuppression"; };
  std::string geturl(const ECAL::Run& run, const int iz) const;
  bool removeFn(const ECAL::TTCCUData& ttd) const;
};

}  // namespace plugins
}  // namespace dqmcpp

#define ZEROSUPPRESSION_HH
#endif