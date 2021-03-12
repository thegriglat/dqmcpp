#ifndef ECALCHANNELS_HH
/**
 * @file ECALChannels.hh
 * @author Grigory Latyshev (thegriglat@gmail.com)
 * @brief Access to ECAL channels database
 */
#include <string>
#include "ECAL.hh"

namespace dqmcpp {
namespace ECAL {
namespace ECALChannels {

/**
 * @brief Static channel list structure
 * Dumped from DQM a long long time ago ...
 */
struct ChannelInfo {
  int fed;
  int tcc;
  int tower;
  int stripintower;
  int xtalinstrip;
  int ccu;
  int vfe;
  int xtalinvfe;
  int xtalinccu;
  int ieta;
  int iphi;
  int ix;
  int iy;
  int iz;
  int hashedid;
  int ic;
  int cmsswid;
  int dbid;
  int ietatt;
  int iphitt;
  int tccch;
  int tccslot;
  int slbch;
  int slbslot;
  int ietagct;
  int iphigct;
  // std::string crate;
  std::string det() const;
};

/**
 * @brief Find channel in static database. Returns nullptr if not found
 *
 * @param channel Channel to be matched
 * @return const ChannelInfo* | nullptr
 */
const ChannelInfo* find(const ECAL::Channel& channel);
const std::vector<ChannelInfo> list(void);
const std::string detByTTTTC(const int tt, const int tcc);

}  // namespace ECALChannels
}  // namespace ECAL
}  // namespace dqmcpp
#define ECALCHANNELS_HH
#endif