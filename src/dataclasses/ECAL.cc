#include "ECAL.hh"
#include "ecalchannels.hh"

#include <algorithm>
#include <vector>
using namespace ECAL;
using namespace std;

namespace ECAL {
std::vector<TTData> channel2TT(
    const std::vector<ECAL::ChannelData>& channelData) {
  vector<TTData> ttdata;
  // get list of TT from channels
  for (auto& channel : channelData) {
    auto c_info = ECALChannels::find(channel.channel);
    if (!c_info) {
      std::cerr << "Cannot find channel!" << std::endl;
      continue;
    }
    // determine det
    int iz = 0;  // EB
    if (c_info->ix != -999) {
      // EE+-
      iz = c_info->iz;
    }
    auto tt = TTData(c_info->tower, iz, channel.value);
    ttdata.push_back(tt);
  }
  // make TT unique
  std::sort(ttdata.begin(), ttdata.end(), [](const TTData& a, const TTData& b) {
    if (a.iz == b.iz)
      return a.tt < b.tt;
    return a.iz < b.iz;
  });
  ttdata.erase(std::unique(ttdata.begin(), ttdata.end(),
                           [](const TTData& a, const TTData& b) {
                             return a.tt == b.tt && a.iz == b.iz;
                           }),
               ttdata.end());
  return ttdata;
}

vector<TTRunData> filterZeroTT(vector<TTRunData>& rundata) {
  if (rundata.size() == 0)
    return vector<TTRunData>();
  auto& ttlist = rundata.at(0).ttdata;
  vector<pair<int, int>> tt_to_remove;
  for (auto& tt : ttlist) {
    bool all_is_zero = true;
    for (auto& rd : rundata) {
      // find
      auto it = std::find_if(
          rd.ttdata.begin(), rd.ttdata.end(),
          [tt](const TTData& e) { return tt.tt == e.tt && tt.iz == e.iz; });
      if (it == rd.ttdata.end()) {
        // tt not found;
        continue;
      };
      if (it->value != 0) {
        all_is_zero = false;
        break;
      }
    }
    if (all_is_zero) {
      tt_to_remove.push_back({tt.tt, tt.iz});
    }
  }
  for (auto& ttremove : tt_to_remove) {
    for (auto& run : rundata) {
      auto it = std::find_if(
          run.ttdata.begin(), run.ttdata.end(), [ttremove](const TTData& e) {
            return ttremove.first == e.tt && ttremove.second == e.tt;
          });
      if (it == run.ttdata.end())
        continue;
      run.ttdata.erase(it);
    }
  }
  return rundata;
};
};  // namespace ECAL