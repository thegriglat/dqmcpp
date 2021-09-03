#include "RecHitOccupancy.hh"

#include "common/common.hh"
#include "net/DQMURLProvider.hh"

REGISTER_PLUGIN(RecHitOccupancy);

namespace dqmcpp {
namespace plugins {
std::string dqmcpp::plugins::RecHitOccupancy::getPrefix() const {
  return "rechit_";
}

std::vector<std::string> dqmcpp::plugins::RecHitOccupancy::get_urls(
    const ECAL::Run& run) const {
  std::vector<std::string> s;
  for (int i = -18; i <= 18; ++i) {
    if (i == 0)
      continue;
    s.push_back(net::DQMURL::dqmurl(
        run, common::string_format(
                 "EcalBarrel/EBOccupancyTask/EBOT rec hit energy EB%+03d", i)));
  }
  for (int i = -9; i <= 9; ++i) {
    if (i == 0)
      continue;
    s.push_back(net::DQMURL::dqmurl(
        run, common::string_format(
                 "EcalEndcap/EEOccupancyTask/EEOT rec hit energy EE%+03d", i)));
  }
  return s;
}
}  // namespace plugins
}  // namespace dqmcpp