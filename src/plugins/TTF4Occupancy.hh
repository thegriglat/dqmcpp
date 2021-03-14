#ifndef TTF4OCCUPANCY_HH
/**
 * @file TTF4Occupancy.hh
 * @author Grigory Latyshev (thegriglat@gmail.com)
 * @brief TTF4 Occupancy plugin
 */
#include <vector>
#include "../ECAL/ECAL.hh"
#include "Plugin.hh"

namespace dqmcpp {
namespace plugins {

class TTF4Occupancy : public Plugin {
 private:
  std::vector<ECAL::RunTTData> readTT();

 public:
  void Process();
};

}  // namespace plugins
}  // namespace dqmcpp

#define TTF4OCCUPANCY_HH
#endif  // !TTF4OCCUPANCY_HH