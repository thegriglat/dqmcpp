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

struct Point1D {
  double x;
  Point1D(const double x) : x(x){};
};

struct Point2D : public Point1D {
  double y;
  Point2D(const double x, const double y) : Point1D(x), y(y){};
};

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

struct TT {
  // TT number
  int tt;
  int tcc;
  // EB = 0 ; EE+ = 1; EE- = -1;
  int iz;
  TT(const int tt, const int tcc, const int iz) : tt(tt), tcc(tcc), iz(iz){};
  inline friend bool operator==(const TT& a, const TT& b) {
    return a.tt == b.tt && a.iz == b.iz && a.tcc == b.tcc;
  }
  inline friend bool operator<(const TT& a, const TT& b) {
    if (a.iz == b.iz) {
      if (a.tcc == b.tcc) {
        return a.tt < b.tt;
      } else {
        return a.tcc < b.tcc;
      }
    } else {
      return a.iz < b.iz;
    }
  }
  inline friend std::ostream& operator<<(std::ostream& os, const TT& elem) {
    os << "TT[" << elem.tt << "," << elem.tcc << "," << elem.iz << "]";
    return os;
  }
};

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

template <typename T>
struct RunData {
  Run run;
  T data;
  RunData(const Run& _run, const T& _data) : run(_run), data(_data){};
};

template <typename T>
struct Data {
  T base;
  double value;
  Data(const T& block, const double value) : base(block), value(value){};
  friend inline std::ostream& operator<<(std::ostream& os, const Data& d) {
    os << d.base << " value: " << d.value;
    return os;
  }
};

using ChannelData = Data<Channel>;
using TTData = Data<TT>;
using Data1D = Data<Point1D>;
using Data2D = Data<Point2D>;

using RunTTData = RunData<std::vector<TTData>>;
using RunChannelData = RunData<std::vector<ChannelData>>;
using RunData2D = RunData<std::vector<Data2D>>;

std::vector<RunTTData> filterZeroTT(std::vector<RunTTData>& rundata);
std::vector<TTData> channel2TT(
    const std::vector<ECAL::ChannelData>& channelData);

ChannelData Data2D2Channel(const Data2D& d2d, const int iz = 0);
std::vector<ChannelData> Data2D2ChannelData(const std::vector<Data2D>& d2d,
                                            const int iz = 0);
};  // namespace ECAL
}  // namespace dqmcpp
#define ECAL_HH
#endif