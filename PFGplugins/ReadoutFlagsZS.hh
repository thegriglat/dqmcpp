#ifndef READOUTFLAGZS_HH

#include "ReadoutFlags.hh"

namespace dqmcpp {
namespace plugins {

class ReadoutFlagsZS : public ReadoutFlags {
 private:
  inline std::string pluginName() const { return "ReadoutFlagsZS"; };
  std::string geturl(const ECAL::Run& run, const int iz) const;
  bool removeFn(const ECAL::TTCCUData& ttd) const;
};

}  // namespace plugins
}  // namespace dqmcpp

#define READOUTFLAGZS_HH
#endif