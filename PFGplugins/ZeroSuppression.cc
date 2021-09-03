#include "ZeroSuppression.hh"
#include "net/DQMURLProvider.hh"

REGISTER_PLUGIN(ZeroSuppression)

namespace dqmcpp {
namespace plugins {

std::string ZeroSuppression::geturl(const ECAL::Run& run, const int iz) const {
  if (iz == 0)
    return net::DQMURL::dqmurl(
        run,
        "EcalBarrel/EBSelectiveReadoutTask/EBSRT zero suppression 1 SR Flags");
  if (iz == -1)
    return net::DQMURL::dqmurl(run,
                               "EcalEndcap/EESelectiveReadoutTask/EESRT zero "
                               "suppression 1 SR Flags EE -");
  if (iz == 1)
    return net::DQMURL::dqmurl(run,
                               "EcalEndcap/EESelectiveReadoutTask/EESRT zero "
                               "suppression 1 SR Flags EE +");
  return "";
}

bool ZeroSuppression::removeFn(const ECAL::TTCCUData& ttdata) const {
  return ttdata.value > 0.98;
}

}  // namespace plugins
}  // namespace dqmcpp