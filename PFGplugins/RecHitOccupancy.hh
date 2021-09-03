#ifndef RECHITOCCUPANCY_HH
#include <string>
#include <vector>
#include "ECAL/ECAL.hh"
#include "DigiOccupancy.hh"
#include "plugins/Plugins.hh"

namespace dqmcpp {
namespace plugins {

class RecHitOccupancy : public DigiOccupancy {
 private:
  std::vector<std::string> get_urls(const ECAL::Run& run) const;
  std::string getPrefix() const;
};

}  // namespace plugins
}  // namespace dqmcpp

#define RECHITOCCUPANCY_HH
#endif