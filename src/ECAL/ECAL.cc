/**
 * @file ECAL.cc
 * @author Grigory Latyshev (thegriglat@gmail.com)
 * @brief Various functions to convert ECAL data
 */
#include "ECAL.hh"
#include "../common/math.hh"
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

std::vector<TTData> channel2TT(
    const std::vector<ECAL::ChannelData>& channelData) {
  vector<TTData> ttdata;
  // get list of TT from channels
  for (auto& channel : channelData) {
    auto c_info = ECALChannels::find(channel.channel);
    if (!c_info) {
      std::cerr << "Cannot find channel!" << channel.channel << std::endl;
      continue;
    }
    // determine det
    int iz = 0;  // EB
    if (c_info->ix != -999) {
      // EE+-
      iz = c_info->iz;
    }
    auto tt = TTData(c_info->tower, iz, c_info->tcc, channel.value);
    ttdata.push_back(tt);
  }
  // make TT unique
  std::sort(ttdata.begin(), ttdata.end(), [](const TTData& a, const TTData& b) {
    if (a.iz == b.iz) {
      if (a.tcc == b.tcc)
        return a.tt < b.tt;
      return a.tcc < b.tcc;
    }
    return a.iz < b.iz;
  });
  ttdata.erase(std::unique(ttdata.begin(), ttdata.end(),
                           [](const TTData& a, const TTData& b) {
                             return a.tt == b.tt && a.iz == b.iz &&
                                    a.tcc == b.tcc;
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
          std::find_if(rd.data.begin(), rd.data.end(), [tt](const TTData& e) {
            return tt.tt == e.tt && tt.iz == e.iz && tt.tcc == e.tcc;
          });
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
      tt_to_remove.push_back({tt.tt, tt.iz, tt.tcc});
    }
  }
  for (auto& ttremove : tt_to_remove) {
    for (auto& run : rundata) {
      auto it = std::find_if(
          run.data.begin(), run.data.end(), [ttremove](const TTData& e) {
            return ttremove[0] == e.tt && ttremove[1] == e.iz &&
                   ttremove[2] == e.tcc;
          });
      if (it == run.data.end())
        continue;
      run.data.erase(it);
    }
  }
  return rundata;
};

ChannelData Data2D2Channel(const Data2D& d2d, const int iz) {
  const int x = std::trunc(d2d.x);
  const int y = std::trunc(d2d.y);
  const int iphi = std::abs(x) + 1;
  const int ieta = y + 1 * common::sign(d2d.y);
  // std::cout << "y " << d2d.y << " ieta " << ieta << std::endl;
  return ChannelData(Channel(iphi, ieta, iz), d2d.value);
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

};  // namespace ECAL
};  // namespace dqmcpp