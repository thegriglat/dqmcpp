#ifndef DIGIOCCUPANCY_HH
#include <string>
#include <vector>
#include "../ECAL/ECAL.hh"
#include "plugins/Plugins.hh"

namespace dqmcpp {
namespace plugins {

class DigiOccupancy : public Plugin {
 private:
  virtual std::vector<std::string> get_urls(const ECAL::Run& run) const;
  virtual std::string getPrefix() const;

 public:
  void Process();
};

}  // namespace plugins
}  // namespace dqmcpp

#define DIGIOCCUPANCY_HH
#endif