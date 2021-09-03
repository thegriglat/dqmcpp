#include "ReadoutFlagsDropped.hh"
#include "net/DQMURLProvider.hh"

REGISTER_PLUGIN(ReadoutFlagsDropped)

namespace dqmcpp {
namespace plugins {

std::string ReadoutFlagsDropped::geturl(const ECAL::Run& run,
                                        const int iz) const {
  if (iz == 0)
    return net::DQMURL::dqmurl(
        run,
        "EcalBarrel/EBSelectiveReadoutTask/EBSRT FR Flagged Dropped Readout");
  if (iz == -1)
    return net::DQMURL::dqmurl(run,
                               "EcalEndcap/EESelectiveReadoutTask/EESRT FR "
                               "Flagged Dropped Readout EE -");
  if (iz == 1)
    return net::DQMURL::dqmurl(run,
                               "EcalEndcap/EESelectiveReadoutTask/EESRT FR "
                               "Flagged Dropped Readout EE +");
  return "";
}

bool ReadoutFlagsDropped::removeFn(const ECAL::TTCCUData& ttdata) const {
  return ttdata.value < 0.05;
}

}  // namespace plugins
}  // namespace dqmcpp