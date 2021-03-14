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
  int fed;           // 601..654
  int tcc;           // 1..108
  int tower;         // 1..68
  int stripintower;  // 1..5
  int xtalinstrip;   // 1..5
  int ccu;           // 1..68
  int vfe;           // 1..5
  int xtalinvfe;     // 1..5
  int xtalinccu;     // 0..24
  int ieta;          //-999..85
  int iphi;          //-999..360
  int ix;            //-999..100
  int iy;            //-999..100
  int iz;            // -1..1
  int hashedid;      // 0..61199
  int ic;            // 1..1700
  int cmsswid;
  int dbid;
  int ietatt;   // -28..28
  int iphitt;   // 1..72
  int tccch;    // 1..68
  int tccslot;  // 2..17
  int slbch;    // 1..8
  int slbslot;  // 1..9
  int ietagct;  // 4..17
  int iphigct;  // 0..17
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