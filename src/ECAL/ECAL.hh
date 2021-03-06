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
#include "../ecalchannels/ChannelInfo.hh"

namespace dqmcpp {
/**
 * @brief Namespace for ECAL specific data types
 *
 */
namespace ECAL {

constexpr unsigned int NEBChannels = 61200;
constexpr unsigned int NEEChannels = 14648;
constexpr unsigned int NTotalChannels = NEBChannels + NEEChannels;
/**
 * @brief Enum class to separate TT and CCU
 *
 */
enum class ECALElement : int { TT, CCU };

/**
 * @brief One-dimensional point
 *
 */
struct Point1D {
  double x;
  Point1D(const double x) : x(x){};
  inline friend std::ostream& operator<<(std::ostream& os, const Point1D& c) {
    os << "[" << c.x << "]";
    return os;
  }
};

/**
 * @brief Two-dimensional point
 *
 */
struct Point2D : public Point1D {
  double y;
  Point2D(const double x, const double y) : Point1D(x), y(y){};
  inline friend std::ostream& operator<<(std::ostream& os, const Point2D& c) {
    os << "[" << c.x << ", " << c.y << "]";
    return os;
  }
};

/**
 * @brief ECAL Channel struct
 *
 */
struct Channel {
  // unsigned long channel_id;
  int ix_iphi;
  int iy_ieta;
  int iz;
  Channel(const int x, const int y, const int z)
      : ix_iphi(x), iy_ieta(y), iz(z){};
  /**
   * @brief Get Channel info from in-memory database
   *
   * @return ECALChannels::ChannelInfo
   */
  ECALChannels::ChannelInfo info() const;
  /**
   * @brief True if channel in ECAL Barrel
   *
   * @return true
   * @return false
   */
  inline bool isEB() const { return iz == 0; };
  /**
   * @brief True if channel in ECAL EE+
   *
   * @return true
   * @return false
   */
  inline bool isEEP() const { return iz == 1; };
  /**
   * @brief True if channel in ECAL EE-
   *
   * @return true
   * @return false
   */
  inline bool isEEM() const { return iz == -1; };
  /**
   * @brief Returns channel as (ix, iy, iz)
   *
   * @return std::array<int, 3>
   */
  inline std::array<int, 3> asArray() const {
    std::array<int, 3> tmp = {ix_iphi, iy_ieta, iz};
    return tmp;
  }
  /**
   * @brief Convert channel to std::string
   *
   * @return std::string
   */
  operator std::string() const;
  /**
   * @brief Compare operator, used in sorting
   *
   * @param a
   * @param b
   * @return true
   * @return false
   */
  inline friend bool operator<(const Channel& a, const Channel& b) {
    if (a.iz == b.iz) {
      if (a.ix_iphi == b.ix_iphi) {
        return a.iy_ieta < b.iy_ieta;
      } else
        return a.ix_iphi < b.ix_iphi;
    }
    return a.iz < b.iz;
  }
  inline friend bool operator==(const Channel& a, const Channel& b) {
    return a.ix_iphi == b.ix_iphi && a.iy_ieta == b.iy_ieta && a.iz == b.iz;
  }
  inline friend std::ostream& operator<<(std::ostream& os, const Channel& c) {
    os << std::string(c);
    return os;
  }
};

/**
 * @brief TT class
 *
 */
struct TT {
  // TT number
  int tt;
  int tcc;
  // EB = 0 ; EE+ = 1; EE- = -1;
  int iz;
  TT(const int tt, const int tcc, const int iz) : tt(tt), tcc(tcc), iz(iz){};
  TT(const Channel& channel);
  TT(const std::array<int, 4>& vec4) : tt(vec4[0]), tcc(vec4[1]), iz(vec4[2]){};
  /**
   * @brief Get FED#, -1 if not found
   *
   * @return int
   */
  int fed() const;
  /**
   * @brief Convert TT to std::string
   *
   * @return std::string
   */
  operator std::string() const;
  /**
   * @brief Returns TT as (tt, tcc, iz, tt or ccu as int)
   *
   * @return std::array<int, 4>
   */
  inline std::array<int, 4> asArray() const {
    std::array<int, 4> tmp = {tt, tcc, iz, (int)ECALElement::TT};
    return tmp;
  }
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
    }
    return a.iz < b.iz;
  }
  inline friend std::ostream& operator<<(std::ostream& os, const TT& elem) {
    os << std::string(elem);
    return os;
  }
};

/**
 * @brief CCU class
 *
 */
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
  /**
   * @brief Get FED#, -1 if not found
   *
   * @return int
   */
  int fed() const;
  /**
   * @brief Convert to std::string
   *
   * @return std::string
   */
  operator std::string() const;
  /**
   * @brief Returns CCU as (ccu, tcc, iz, tt or ccu as int)
   *
   * @return std::array<int, 4>
   */
  inline std::array<int, 4> asArray() const {
    std::array<int, 4> tmp = {ccu, tcc, iz, (int)ECALElement::CCU};
    return tmp;
  }
  inline friend bool operator==(const CCU& a, const CCU& b) {
    return a.ccu == b.ccu && a.iz == b.iz && a.tcc == b.tcc;
  }
  inline friend bool operator<(const CCU& a, const CCU& b) {
    if (a.iz == b.iz) {
      if (a.tcc == b.tcc) {
        return a.ccu < b.ccu;
      } else {
        return a.tcc < b.tcc;
      }
    }
    return a.iz < b.iz;
  }
  inline friend std::ostream& operator<<(std::ostream& os, const CCU& elem) {
    os << std::string(elem);
    return os;
  }
};

/**
 * @brief Class for storing TT and CCU in STL lists
 *
 */
class TTCCU {
 private:
  std::array<int, 4> data;

 public:
  TTCCU(const TT& tt) : data(tt.asArray()){};
  TTCCU(const CCU& ccu) : data(ccu.asArray()){};
  /**
   * @brief Is object TT
   *
   * @return true
   * @return false
   */
  inline bool isTT() const { return data.at(3) == (int)ECALElement::TT; }
  /**
   * @brief Is object CCU
   *
   * @return true
   * @return false
   */
  inline bool isCCU() const { return data.at(3) == (int)ECALElement::CCU; }
  /**
   * @brief Get FED #, -1 if not found
   *
   * @return int
   */
  inline int fed() const {
    if (isTT())
      return TT(data).fed();
    if (isCCU())
      return CCU(data).fed();
    return -1;
  }
  /**
   * @brief Convert to std::string
   *
   * @return std::string
   */
  inline operator std::string() const {
    if (isTT())
      return std::string(TT(data));
    if (isCCU())
      return std::string(CCU(data));
    return "unknown type";
  }
};

/**
 * @brief Run class
 *
 */
struct Run {
  int runnumber;
  std::string dataset;
  Run(int _run, const std::string& _dataset)
      : runnumber(_run), dataset(_dataset){};
  inline friend bool operator==(const Run& a, const Run& b) {
    return a.runnumber == b.runnumber && a.dataset == b.dataset;
  }
  inline friend std::ostream& operator<<(std::ostream& os, const Run& run) {
    os << run.runnumber << " " << run.dataset;
    return os;
  }
};

/**
 * @brief Class to keep templated run data
 *
 * @tparam T
 */
template <typename T>
struct RunData {
  Run run;
  T data;
  RunData(const Run& _run, const T& _data) : run(_run), data(_data){};
};

/**
 * @brief Templated data class.
 *
 * @tparam T
 */
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

/**
 * @brief Removes TT with all values == 0
 *
 * @param rundata RunTTData input vector
 * @return std::vector<RunTTData>
 */
std::vector<RunTTData> filterZeroTT(std::vector<RunTTData>& rundata);

/**
 * @brief Convects list of channels to list of TT
 *
 * @param channelData ECAL::ChannelData input vector
 * @return std::vector<TTData>
 */
std::vector<TTData> channel2TT(
    const std::vector<ECAL::ChannelData>& channelData);

/**
 * @brief Converts Point2D to channel with given iz
 *
 * @param p Point2D input
 * @param iz (0, +1, -1) for (EB, EE+, EE-)
 * @return Channel
 */
Channel Point2D2Channel(const Point2D& p, const int iz);

/**
 * @brief Converts Data2D to channel with given iz
 *
 * @param d2d Data2D input
 * @param iz (0, +1, -1) for (EB, EE+, EE-)
 * @return ChannelData
 */
ChannelData Data2D2Channel(const Data2D& d2d, const int iz = 0);

/**
 * @brief Data2D to channelData
 *
 * @param d2d Data2D input vector
 * @param iz (0, +1, -1) for (EB, EE+, EE-)
 * @return std::vector<ChannelData>
 */
std::vector<ChannelData> Data2D2ChannelData(const std::vector<Data2D>& d2d,
                                            const int iz = 0);
};  // namespace ECAL
}  // namespace dqmcpp
#define ECAL_HH
#endif