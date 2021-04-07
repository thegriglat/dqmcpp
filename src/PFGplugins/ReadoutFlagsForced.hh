#ifndef READOUTFLAGSFORCED_HH

#include "ReadoutFlags.hh"

namespace dqmcpp {
namespace plugins {

class ReadoutFlagsForced : public ReadoutFlags {
 private:
  inline std::string pluginName() const { return "ReadoutFlagsForced"; };

  std::string geturl(const ECAL::Run& run, const int iz) const;
   bool removeFn(const ECAL::TTData& ttd) const;
};

}  // namespace plugins
}  // namespace dqmcpp

#define READOUTFLAGSFORCED_HH
#endif