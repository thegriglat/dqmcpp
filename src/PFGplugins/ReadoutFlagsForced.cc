#include "ReadoutFlagsForced.hh"
#include "net/DQMURLProvider.hh"

REGISTER_PLUGIN(ReadoutFlagsForced)

namespace dqmcpp {
namespace plugins {

std::string ReadoutFlagsForced::geturl(const ECAL::Run& run,
                                       const int iz) const {
  if (iz == 0)
    return net::DQMURL::dqmurl(
        run,
        "EcalBarrel/EBSelectiveReadoutTask/EBSRT readout unit with SR forced");
  if (iz == -1)
    return net::DQMURL::dqmurl(run,
                               "EcalEndcap/EESelectiveReadoutTask/EESRT "
                               "readout unit with SR forced EE -");
  if (iz == 1)
    return net::DQMURL::dqmurl(run,
                               "EcalEndcap/EESelectiveReadoutTask/EESRT "
                               "readout unit with SR forced EE +");
  return "";
}

bool ReadoutFlagsForced::removeFn(const ECAL::TTData& ttdata) const {
  return ttdata.value < 0.05;
}

}  // namespace plugins
}  // namespace dqmcpp