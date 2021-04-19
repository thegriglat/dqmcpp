#ifndef READOUTFLAGSDROPPED_HH

#include "ReadoutFlags.hh"

namespace dqmcpp {
namespace plugins {

class ReadoutFlagsDropped : public ReadoutFlags {
 private:
  inline std::string pluginName() const { return "ReadoutFlagsDropped"; };
  std::string geturl(const ECAL::Run& run, const int iz) const;
  bool removeFn(const ECAL::TTCCUData& ttd) const;
};

}  // namespace plugins
}  // namespace dqmcpp

#define READOUTFLAGSDROPPED_HH
#endif