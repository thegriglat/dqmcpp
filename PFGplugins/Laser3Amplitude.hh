#ifndef Laser3Amplitude_HH

#include <vector>
#include "ECAL/ECAL.hh"
#include "plugins/Plugins.hh"

namespace dqmcpp {
namespace plugins {

class Laser3Amplitude : public Plugin {
 public:
  using RunL1Data = ECAL::RunData<std::vector<ECAL::ChannelData>>;

 private:
  std::vector<RunL1Data> getRunData(const int sm, bool eb);

 public:
  void Process();
};

}  // namespace plugins
}  // namespace dqmcpp

#define Laser3Amplitude_HH
#endif  // !Laser3Amplitude_HH