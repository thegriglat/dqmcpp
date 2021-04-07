#include "ReadoutFlagsZS.hh"
#include "net/DQMURLProvider.hh"

REGISTER_PLUGIN(ReadoutFlagsZS)

namespace dqmcpp {
namespace plugins {

std::string ReadoutFlagsZS::geturl(const ECAL::Run& run, const int iz) const {
  if (iz == 0)
    return net::DQMURL::dqmurl(
        run,
        "EcalBarrel/EBSelectiveReadoutTask/EBSRT ZS Flagged Fully Readout");
  if (iz == -1)
    return net::DQMURL::dqmurl(run,
                               "EcalEndcap/EESelectiveReadoutTask/EESRT ZS "
                               "Flagged Fully Readout EE -");
  if (iz == 1)
    return net::DQMURL::dqmurl(run,
                               "EcalEndcap/EESelectiveReadoutTask/EESRT ZS "
                               "Flagged Fully Readout EE +");
  return "";
}

bool ReadoutFlagsZS::removeFn(const ECAL::TTData& ttdata) const {
  return ttdata.value < 0.05;
}

}  // namespace plugins
}  // namespace dqmcpp