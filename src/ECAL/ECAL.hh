#ifndef ECAL_HH
/**
 * @file ECAL.hh
 * @author Grigory Latyshev (thegriglat@gmail.com)
 * @brief ECAL data structure namespace
 */
#include <array>
#include <ostream>
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
/**
 * @brief Common 2D data structure
 *
 */
struct Data2D {
  double x;
  double y;
  double value;
  Data2D(double x, double y, double value) : x(x), y(y), value(value){};
  friend std::ostream& operator<<(std::ostream& os, const Data2D& d2d) {
    os << "[" << d2d.x << ", " << d2d.y << "] = " << d2d.value;
    return os;
  }
};

/**
 * @brief Common 1D data structure
 *
 */
struct Data1D {
  double x;
  double value;
  Data1D(double x, double value) : x(x), value(value){};
  friend std::ostream& operator<<(std::ostream& os, const Data1D& d1d) {
    os << "[" << d1d.x << "] = " << d1d.value;
    return os;
  }
};

/**
 * @brief Basic channel struct
 * Trivial constructor is disabled
 */
struct Channel {
  // unsigned long channel_id;
  int ix_iphi;
  int iy_ieta;
  int iz;
  Channel(const int x, const int y, const int z)
      : ix_iphi(x), iy_ieta(y), iz(z){};
  inline bool isEB() const { return iz == 0; };
  inline bool isEEP() const { return iz == 1; };
  inline bool isEEM() const { return iz == -1; };
  inline std::array<int, 3> asArray() const {
    std::array<int, 3> tmp = {ix_iphi, iy_ieta, iz};
    return tmp;
  }
  inline friend bool operator<(const Channel& a, const Channel& b) {
    if (a.iz == b.iz) {
      if (a.ix_iphi == b.ix_iphi) {
        return a.iy_ieta < b.iy_ieta;
      } else
        return a.ix_iphi < b.ix_iphi;
    } else
      return a.iz < b.iz;
  }
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
  friend std::ostream& operator<<(std::ostream& os, const Run& run) {
    os << run.runnumber << " " << run.dataset;
    return os;
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

using RunTTData = RunData<std::vector<TTData>>;
using RunChannelData = RunData<std::vector<ChannelData>>;
using RunData2D = RunData<std::vector<Data2D>>;

std::vector<RunTTData> filterZeroTT(std::vector<RunTTData>& rundata);
std::vector<TTData> channel2TT(
    const std::vector<ECAL::ChannelData>& channelData);

ChannelData Data2D2Channel(const Data2D& d2d, const int iz = 0);
};  // namespace ECAL
}  // namespace dqmcpp
#define ECAL_HH
#endif