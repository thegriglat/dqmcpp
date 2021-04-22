/**
 * @file ECAL.cc
 * @author Grigory Latyshev (thegriglat@gmail.com)
 * @brief Various functions to convert ECAL data
 */
#include "ECAL.hh"
#include "../common/common.hh"
#include "../ecalchannels/ECALChannels.hh"

#include <algorithm>
#include <array>
#include <cmath>
#include <iostream>
#include <vector>

using namespace dqmcpp::ECAL;
using namespace std;

namespace dqmcpp {
namespace ECAL {

TT::TT(const Channel& channel) {
  const auto info = ECALChannels::find(channel);
  if (!info) {
    std::cerr << std::endl << "Cannot find channel!" << channel << std::endl;
    exit(1);
  }
  iz = 0;
  if (info->ix != -999)
    iz = info->iz;
  tt = info->tower;
  tcc = info->tcc;
}

CCU::CCU(const Channel& channel) {
  const auto info = ECALChannels::find(channel);
  if (!info) {
    std::cerr << std::endl << "Cannot find channel!" << channel << std::endl;
    exit(1);
  }
  iz = 0;
  if (info->ix != -999)
    iz = info->iz;
  ccu = info->ccu;
  tcc = info->tcc;
}

std::vector<TTData> channel2TT(
    const std::vector<ECAL::ChannelData>& channelData) {
  vector<TTData> ttdata;
  ttdata.reserve(channelData.size());
  // get list of TT from channels
  for (auto& channel : channelData) {
    ttdata.emplace_back(TT(channel.base), channel.value);
  }
  // make TT unique
  std::sort(ttdata.begin(), ttdata.end(),
            [](const TTData& a, const TTData& b) { return a.base < b.base; });
  ttdata.erase(std::unique(ttdata.begin(), ttdata.end(),
                           [](const TTData& a, const TTData& b) {
                             return a.base == b.base;
                           }),
               ttdata.end());
  return ttdata;
}

vector<RunTTData> filterZeroTT(vector<RunTTData>& rundata) {
  if (rundata.size() == 0)
    return vector<RunTTData>();
  auto& ttlist = rundata.at(0).data;
  vector<array<int, 3>> tt_to_remove;
  for (auto& tt : ttlist) {
    bool all_is_zero = true;
    for (auto& rd : rundata) {
      // find
      auto it =
          std::find_if(rd.data.begin(), rd.data.end(),
                       [tt](const TTData& e) { return tt.base == e.base; });
      if (it == rd.data.end()) {
        // tt not found;
        continue;
      };
      if (!common::isZero(it->value)) {
        all_is_zero = false;
        break;
      }
    }
    if (all_is_zero) {
      tt_to_remove.push_back({tt.base.tt, tt.base.iz, tt.base.tcc});
    }
  }
  for (auto& ttremove : tt_to_remove) {
    for (auto& run : rundata) {
      auto it = std::find_if(
          run.data.begin(), run.data.end(), [ttremove](const TTData& e) {
            return ttremove[0] == e.base.tt && ttremove[1] == e.base.iz &&
                   ttremove[2] == e.base.tcc;
          });
      if (it == run.data.end())
        continue;
      run.data.erase(it);
    }
  }
  return rundata;
};

Channel Point2D2Channel(const Point2D& p, const int iz) {
  const int x = std::trunc(p.x);
  const int y = std::trunc(p.y);
  const int iphi = std::abs(x) + 1;
  const int ieta = y + 1 * common::sign(p.y);
  return Channel(iphi, ieta, iz);
}

ChannelData Data2D2Channel(const Data2D& d2d, const int iz) {
  return ChannelData(Point2D2Channel(d2d.base, iz), d2d.value);
}

std::vector<ChannelData> Data2D2ChannelData(const std::vector<Data2D>& d2d,
                                            const int iz) {
  std::vector<ChannelData> chd;
  chd.reserve(d2d.size());
  for (auto& d : d2d) {
    chd.push_back(Data2D2Channel(d, iz));
  }
  return chd;
}

std::array<int, 3> Channel::asArray() const {
  std::array<int, 3> tmp = {ix_iphi, iy_ieta, iz};
  return tmp;
}

// operators

std::ostream& operator<<(std::ostream& os, const Point1D& c) {
  os << "[" << c.x << "]";
  return os;
}

std::ostream& operator<<(std::ostream& os, const Point2D& c) {
  os << "[" << c.x << ", " << c.y << "]";
  return os;
}

bool operator<(const Channel& a, const Channel& b) {
  if (a.iz == b.iz) {
    if (a.ix_iphi == b.ix_iphi) {
      return a.iy_ieta < b.iy_ieta;
    } else
      return a.ix_iphi < b.ix_iphi;
  } else
    return a.iz < b.iz;
}

std::ostream& operator<<(std::ostream& os, const Channel& c) {
  os << std::string(c);
  return os;
}

Channel::operator std::string() const {
  auto it = ECALChannels::find(*this);
  const std::string det = (it != nullptr) ? it->det() : "UNKNOWN";
  return common::string_format("%s [%+03d,%+03d]", det.c_str(), ix_iphi,
                               iy_ieta);
}

bool operator==(const Channel& a, const Channel& b) {
  return a.ix_iphi == b.ix_iphi && a.iy_ieta == b.iy_ieta && a.iz == b.iz;
}

bool operator==(const TT& a, const TT& b) {
  return a.tt == b.tt && a.iz == b.iz && a.tcc == b.tcc;
}

bool operator<(const TT& a, const TT& b) {
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

std::ostream& operator<<(std::ostream& os, const TT& elem) {
  os << std::string(elem);
  return os;
}

TT::operator std::string() const {
  const auto _l = ECALChannels::list();
  auto it = std::find_if(
      _l.first, _l.second, [this](const ECALChannels::ChannelInfo& ci) {
        return ci.tower == tt && ci.tcc == tcc && ci.det_iz() == iz;
      });
  const std::string det = (it) ? it->det() : "unknown";
  return common::string_format("%s TCC%02d TT%02d", det.c_str(), tcc, tt);
}

bool operator==(const CCU& a, const CCU& b) {
  return a.ccu == b.ccu && a.iz == b.iz && a.tcc == b.tcc;
}

bool operator<(const CCU& a, const CCU& b) {
  if (a.iz == b.iz) {
    if (a.tcc == b.tcc) {
      return a.ccu < b.ccu;
    } else {
      return a.tcc < b.tcc;
    }
  } else {
    return a.iz < b.iz;
  }
}

std::ostream& operator<<(std::ostream& os, const CCU& elem) {
  os << std::string(elem);
  return os;
}

CCU::operator std::string() const {
  const auto _l = ECALChannels::list();
  auto it = std::find_if(
      _l.first, _l.second, [this](const ECALChannels::ChannelInfo& ci) {
        return ci.ccu == ccu && ci.tcc == tcc && ci.det_iz() == iz;
      });
  const std::string det = (it) ? it->det() : "unknown";
  return common::string_format("%s TCC%02d CCU%02d", det.c_str(), tcc, ccu);
}

bool operator==(const Run& a, const Run& b) {
  return a.dataset == b.dataset && a.runnumber == b.runnumber;
}

std::ostream& operator<<(std::ostream& os, const Run& run) {
  os << run.runnumber << " " << run.dataset;
  return os;
}

};  // namespace ECAL
};  // namespace dqmcpp