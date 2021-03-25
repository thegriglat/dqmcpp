#ifndef TPOCCUPANCYLTAMPLITUDE_HH

#include <vector>
#include "../ECAL/ECAL.hh"
#include "Plugin.hh"

namespace dqmcpp {
namespace plugins {

class TPOccupancyLTAmplitude : public Plugin {
 public:
  using RunL1Data = ECAL::RunData<std::vector<ECAL::ChannelData>>;

 private:
  std::vector<RunL1Data> getRunData(const int sm, bool eb);

 public:
  void Process();
};

}  // namespace plugins
}  // namespace dqmcpp

#define TPOCCUPANCYLTAMPLITUDE_HH
#endif  // !TPOCCUPANCYLTAMPLITUDE_HH