#ifndef ECALCHANNELS_HH
/**
 * @file ECALChannels.hh
 * @author Grigory Latyshev (thegriglat@gmail.com)
 * @brief Access to ECAL channels database
 */
#include <array>
#include <string>
#include "../ECAL/ECAL.hh"

namespace dqmcpp {
namespace ECALChannels {

/**
 * @brief Static channel list structure
 * Dumped from DQM a long long time ago ...
 */
struct ChannelInfo {
  unsigned short fed;
  char tcc;
  unsigned char tower;
  unsigned short stripintower;
  unsigned short xtalinstrip;
  unsigned short ccu;
  unsigned short vfe;
  short xtalinvfe;
  unsigned short xtalinccu;
  short ieta;
  short iphi;
  short ix;
  short iy;
  short iz;
  unsigned short hashedid;
  unsigned short ic;
  long int cmsswid;
  long int dbid;
  short ietatt;
  unsigned short iphitt;
  unsigned short tccch;
  unsigned short tccslot;
  unsigned short slbch;
  unsigned short slbslot;
  unsigned short ietagct;
  unsigned short iphigct;
  // std::string crate;
  std::string det() const;
};

using ECALChannelsList = std::array<dqmcpp::ECALChannels::ChannelInfo, 75848>;

/**
 * @brief Find channel in static database. Returns nullptr if not found
 *
 * @param channel Channel to be matched
 * @return const ChannelInfo* | nullptr
 */
const ChannelInfo* find(const ECAL::Channel& channel);
const ECALChannelsList list(void);
const std::string detByTTTTC(const int tt, const int tcc);

}  // namespace ECALChannels
}  // namespace dqmcpp
#define ECALCHANNELS_HH
#endif