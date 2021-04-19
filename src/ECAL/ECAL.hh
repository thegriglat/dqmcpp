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
enum class ECALElement : int { TT, CCU };

struct Point1D {
  double x;
  Point1D(const double x) : x(x){};
  friend std::ostream& operator<<(std::ostream& os, const Point1D& c);
};

struct Point2D : public Point1D {
  double y;
  Point2D(const double x, const double y) : Point1D(x), y(y){};
  friend std::ostream& operator<<(std::ostream& os, const Point2D& c);
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
  std::array<int, 3> asArray() const;
  operator std::string() const;
  friend bool operator<(const Channel& a, const Channel& b);
  friend bool operator==(const Channel& a, const Channel& b);
  friend std::ostream& operator<<(std::ostream& os, const Channel& c);
};

struct TT {
  // TT number
  int tt;
  int tcc;
  // EB = 0 ; EE+ = 1; EE- = -1;
  int iz;
  TT(const int tt, const int tcc, const int iz) : tt(tt), tcc(tcc), iz(iz){};
  TT(const Channel& channel);
  TT(const std::array<int, 4>& vec4) : tt(vec4[0]), tcc(vec4[1]), iz(vec4[2]){};
  operator std::string() const;
  inline std::array<int, 4> asArray() const {
    std::array<int, 4> tmp = {tt, tcc, iz, (int)ECALElement::TT};
    return tmp;
  }
  friend bool operator==(const TT& a, const TT& b);
  friend bool operator<(const TT& a, const TT& b);
  friend std::ostream& operator<<(std::ostream& os, const TT& elem);
};

struct CCU {
  // CCU number
  int ccu;
  int tcc;
  // EB = 0 ; EE+ = 1; EE- = -1;
  int iz;
  CCU(const int ccu, const int tcc, const int iz)
      : ccu(ccu), tcc(tcc), iz(iz){};
  CCU(const Channel& channel);
  CCU(const std::array<int, 4>& vec4)
      : ccu(vec4[0]), tcc(vec4[1]), iz(vec4[2]){};
  operator std::string() const;
  inline std::array<int, 4> asArray() const {
    std::array<int, 4> tmp = {ccu, tcc, iz, (int)ECALElement::CCU};
    return tmp;
  }
  friend bool operator==(const CCU& a, const CCU& b);
  friend bool operator<(const CCU& a, const CCU& b);
  friend std::ostream& operator<<(std::ostream& os, const CCU& elem);
};

class TTCCU {
 private:
  std::array<int, 4> data;

 public:
  TTCCU(const TT& tt) : data(tt.asArray()){};
  TTCCU(const CCU& ccu) : data(ccu.asArray()){};
  inline bool isTT() const { return data.at(3) == (int)ECALElement::TT; }
  inline bool isCCU() const { return data.at(3) == (int)ECALElement::CCU; }
  inline operator std::string() const {
    if (isTT())
      return std::string(TT(data));
    if (isCCU())
      return std::string(CCU(data));
    return "unknown type";
  }
};

struct Run {
  int runnumber;
  std::string dataset;
  Run(int _run, const std::string& _dataset)
      : runnumber(_run), dataset(_dataset){};
  friend bool operator==(const Run& a, const Run& b);
  friend std::ostream& operator<<(std::ostream& os, const Run& run);
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
using CCUData = Data<CCU>;
using TTCCUData = Data<TTCCU>;
using Data1D = Data<Point1D>;
using Data2D = Data<Point2D>;

using RunTTData = RunData<std::vector<TTData>>;
using RunCCUData = RunData<std::vector<CCUData>>;
using RunTTCCUData = RunData<std::vector<TTCCUData>>;
using RunChannelData = RunData<std::vector<ChannelData>>;
using RunData2D = RunData<std::vector<Data2D>>;
using RunData1D = RunData<std::vector<Data1D>>;

std::vector<RunTTData> filterZeroTT(std::vector<RunTTData>& rundata);
std::vector<TTData> channel2TT(
    const std::vector<ECAL::ChannelData>& channelData);

Channel Point2D2Channel(const Point2D& p, const int iz);
ChannelData Data2D2Channel(const Data2D& d2d, const int iz = 0);
std::vector<ChannelData> Data2D2ChannelData(const std::vector<Data2D>& d2d,
                                            const int iz = 0);
};  // namespace ECAL
}  // namespace dqmcpp
#define ECAL_HH
#endif