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
  unsigned short fed;          // 601..654
  unsigned char tcc;           // 1..108
  unsigned char tower;         // 1..68
  unsigned char stripintower;  // 1..5
  unsigned char xtalinstrip;   // 1..5
  unsigned char ccu;           // 1..68
  unsigned char vfe;           // 1..5
  unsigned char xtalinvfe;     // 1..5
  unsigned char xtalinccu;     // 0..24
  signed short ieta;           //-999..85
  signed short iphi;           //-999..360
  signed short ix;             //-999..100
  signed short iy;             //-999..100
  signed char iz;              // -1..1
  unsigned short hashedid;     // 0..61199
  unsigned short ic;           // 1..1700
  unsigned int cmsswid;
  unsigned int dbid;
  signed char ietatt;     // -28..28
  unsigned char iphitt;   // 1..72
  unsigned char tccch;    // 1..68
  unsigned char tccslot;  // 2..17
  unsigned char slbch;    // 1..8
  unsigned char slbslot;  // 1..9
  unsigned char ietagct;  // 4..17
  unsigned char iphigct;  // 0..17
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