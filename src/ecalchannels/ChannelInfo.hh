#ifndef CHANNELINFO_HH
/**
 * @file ChannelInfo.hh
 * @author Grigory Latyshev (thegriglat@gmail.com)
 * @brief ChannelInfo struct definition
 *
 */
#include <cstdint>
#include <string>

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
  /**
   * @brief Get Channel detector like EB+03
   *
   * @return std::string
   */
  std::string det() const;
  /**
   * @brief Get Channel detector's iz. EB == 0; EE- == -1; EE+ == +1
   *
   * @return int
   */
  int det_iz() const;
  /**
   * @brief Get Channel detector: EB or EE
   *
   * @return std::string
   */
  std::string part() const;
  /**
   * @brief Get supermodule
   *
   * @return int
   */
  int sm() const;
};

}  // namespace ECALChannels
}  // namespace dqmcpp

#define CHANNELINFO_HH
#endif