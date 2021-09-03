#ifndef READOUTFLAGS_HH

#include <string>
#include <vector>

#include "plugins/Plugins.hh"

namespace dqmcpp {
namespace plugins {

class ReadoutFlags : public Plugin {
 private:
  virtual inline std::string pluginName() const { return "ReadoutFlags"; };
  std::vector<ECAL::RunTTCCUData> getRundata() const;
  virtual std::string geturl(const ECAL::Run& run, const int iz) const;
  static void plot(const std::vector<ECAL::RunTTCCUData>& rundata,
                   const std::string& name);

  virtual bool removeFn(const ECAL::TTCCUData& ttd) const;

 public:
  void Process();
};
}  // namespace plugins
}  // namespace dqmcpp

#define READOUTFLAGS_HH
#endif
