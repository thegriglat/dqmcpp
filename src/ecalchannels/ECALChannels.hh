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
  uint16_t fed;          // 601..654
  uint8_t tcc;           // 1..108
  uint8_t tower;         // 1..68
  uint8_t stripintower;  // 1..5
  uint8_t xtalinstrip;   // 1..5
  uint8_t ccu;           // 1..68
  uint8_t vfe;           // 1..5
  uint8_t xtalinvfe;     // 1..5
  uint8_t xtalinccu;     // 0..24
  int16_t ieta;          //-999..85
  int16_t iphi;          //-999..360
  int16_t ix;            //-999..100
  int16_t iy;            //-999..100
  int8_t iz;             // -1..1
  uint16_t hashedid;     // 0..61199
  uint16_t ic;           // 1..1700
  uint32_t cmsswid;
  uint32_t dbid;
  int8_t ietatt;    // -28..28
  uint8_t iphitt;   // 1..72
  uint8_t tccch;    // 1..68
  uint8_t tccslot;  // 2..17
  uint8_t slbch;    // 1..8
  uint8_t slbslot;  // 1..9
  uint8_t ietagct;  // 4..17
  uint8_t iphigct;  // 0..17
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
inline const std::string det(const ECAL::TT& tt) {
  return detByTTTTC(tt.tt, tt.tcc);
}

}  // namespace ECALChannels
}  // namespace dqmcpp
#define ECALCHANNELS_HH
#endif