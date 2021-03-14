#ifndef ECAL_HH
/**
 * @file ECAL.hh
 * @author Grigory Latyshev (thegriglat@gmail.com)
 * @brief ECAL data structure namespace
 */
#include <iostream>
#include <string>
#include <vector>

namespace dqmcpp {
namespace ECAL {

constexpr unsigned int NEBChannels = 61200;
constexpr unsigned int NEEChannels = 14648;
constexpr unsigned int NTotalChannels = NEBChannels + NEEChannels;
/**
 * @brief Explicit enum of ECAL parts
 * Usually shouldn't be casted to int
 */
enum DETECTORS { EB = 0, EEPLUS = 1, EEMINUS = -1 };
/**
 * @brief Common 2D data structure
 *
 */
struct Data2D {
  double x;
  double y;
  double value;
  Data2D(double x, double y, double value) : x(x), y(y), value(value){};
};

/**
 * @brief Basic channel struct
 * Trivial constructor is disabled
 */
struct Channel {
  // unsigned long channel_id;
  int ix_iphi;
  int iy_ieta;
  DETECTORS iz;
  Channel(int x, int y, DETECTORS z) : ix_iphi(x), iy_ieta(y), iz(z){};
  friend inline std::ostream& operator<<(std::ostream& os, const Channel& c) {
    os << "{x: " << c.ix_iphi << ", y: " << c.iy_ieta << ", z:" << c.iz << "}";
    return os;
  }

  friend inline bool operator==(const Channel& a, const Channel& b) {
    return a.ix_iphi == b.ix_iphi && a.iy_ieta == b.iy_ieta && a.iz == b.iz;
  }
};
/**
 * @brief Basic channel data struct
 * Trivial constructor is disabled
 */
struct ChannelData {
  Channel channel;
  double value;
  ChannelData(Channel _channel, double _value)
      : channel(_channel), value(_value){};
  friend inline std::ostream& operator<<(std::ostream& os,
                                         const ChannelData& cd) {
    os << cd.channel << " value: " << cd.value;
    return os;
  }
};
/**
 * @brief Basic run information
 * Trivial constructor is disabled
 */
struct Run {
  int runnumber;
  std::string dataset;
  Run(int _run, std::string _dataset) : runnumber(_run), dataset(_dataset){};
  friend inline bool operator==(const Run& a, const Run& b) {
    return a.dataset == b.dataset && a.runnumber == b.runnumber;
  }
};

struct TTData {
  // TT number
  int tt;
  // EB = 0 ; EE+ = 1; EE- = -1;
  int iz;
  int tcc;
  double value;
  TTData(int ttnum, int z, int tcc, double val)
      : tt(ttnum), iz(z), tcc(tcc), value(val){};
  inline friend bool operator>(const TTData& a, const TTData& b) {
    return a.value > b.value;
  };
  inline friend std::ostream& operator<<(std::ostream& os, const TTData& elem) {
    os << "tt[tt=" << elem.tt << ",iz=" << elem.iz << ",tcc=" << elem.tcc
       << "] = " << elem.value;
    return os;
  }
};

template <typename T>
struct RunData {
  Run run;
  T data;
  RunData(const Run& _run, const T& _data) : run(_run), data(_data){};
};

typedef RunData<std::vector<TTData>> TTRunData;
typedef RunData<std::vector<ChannelData>> RunChannelData;
typedef RunData<std::vector<Data2D>> RunData2D;

std::vector<TTRunData> filterZeroTT(std::vector<TTRunData>& rundata);
std::vector<TTData> channel2TT(
    const std::vector<ECAL::ChannelData>& channelData);

};  // namespace ECAL
}  // namespace dqmcpp
#define ECAL_HH
#endif