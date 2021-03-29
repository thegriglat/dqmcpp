#ifndef TPOCCUPANCYL1_HH

#include <vector>
#include "../ECAL/ECAL.hh"
#include "Plugins.hh"

namespace dqmcpp {
namespace plugins {

class TPOccupancyL1 : public Plugin {
 public:
  using RunL1Data = ECAL::RunData<std::vector<ECAL::Data2D>>;

 private:
  std::vector<RunL1Data> getRunData(void);

 public:
  void Process();
};

}  // namespace plugins
}  // namespace dqmcpp

#define TPOCCUPANCYL1_HH
#endif  // !TPOCCUPANCYL1_HH