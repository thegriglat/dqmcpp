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
  const auto info = channel.info();
  iz = 0;
  if (info.ix != -999)
    iz = info.iz;
  tt = info.tower;
  tcc = info.tcc;
}

int TT::fed() const {
  const auto be = ECALChannels::list();
  const auto it = std::find_if(
      be.begin, be.end, [this](const ECALChannels::ChannelInfo& ci) {
        return ci.tcc == this->tcc && ci.tower == this->tt &&
               ci.det_iz() == this->iz;
      });
  if (it == be.end)
    return -1;
  return it->fed;
}

CCU::CCU(const Channel& channel) {
  const auto info = channel.info();
  iz = 0;
  if (info.ix != -999)
    iz = info.iz;
  ccu = info.ccu;
  tcc = info.tcc;
}

int CCU::fed() const {
  const auto be = ECALChannels::list();
  const auto it = std::find_if(
      be.begin, be.end, [this](const ECALChannels::ChannelInfo& ci) {
        return ci.tcc == this->tcc && ci.ccu == this->ccu &&
               ci.det_iz() == this->iz;
      });
  if (it == be.end)
    return -1;
  return it->fed;
}

ECALChannels::ChannelInfo Channel::info() const {
  auto cinfo = ECALChannels::find(*this);
  if (cinfo)
    return *cinfo;
  cout << "BAD CHANNEL. Check x = " << ix_iphi << " y = " << iy_ieta
       << " z = " << iz << endl;
  exit(1);
}

std::vector<TTData> channel2TT(
    const std::vector<ECAL::ChannelData>& channelData) {
  vector<TTData> ttdata;
  ttdata.reserve(channelData.size());
  // get list of TT from channels
  std::transform(channelData.begin(), channelData.end(),
                 std::back_inserter(ttdata), [](const auto& channel) {
                   return TTData(TT(channel.base), channel.value);
                 });
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
  std::transform(d2d.begin(), d2d.end(), std::back_inserter(chd),
                 [iz](const auto& d) { return Data2D2Channel(d, iz); });
  return chd;
}

// operators

Channel::operator std::string() const {
  auto i = info();
  return common::string_format("%s [%+03d,%+03d]", i.det().c_str(), ix_iphi,
                               iy_ieta);
}

TT::operator std::string() const {
  const auto _l = ECALChannels::list();
  auto it = std::find_if(
      _l.begin, _l.end, [this](const ECALChannels::ChannelInfo& ci) {
        return ci.tower == tt && ci.tcc == tcc && ci.det_iz() == iz;
      });
  const std::string det = (it != _l.end) ? it->det() : "unknown";
  return common::string_format("%s TCC%02d TT%02d", det.c_str(), tcc, tt);
}

CCU::operator std::string() const {
  const auto _l = ECALChannels::list();
  auto it = std::find_if(
      _l.begin, _l.end, [this](const ECALChannels::ChannelInfo& ci) {
        return ci.ccu == ccu && ci.tcc == tcc && ci.det_iz() == iz;
      });
  const std::string det = (it != _l.end) ? it->det() : "unknown";
  return common::string_format("%s TCC%02d CCU%02d", det.c_str(), tcc, ccu);
}

};  // namespace ECAL
};  // namespace dqmcpp