#include "TTF4Plugin.hh"

#include <algorithm>
#include <fstream>
#include <iostream>
#include <map>
#include <string>
#include <vector>
#include "../common/common.hh"
#include "../dataclasses/ecalchannels.hh"
#include "../readers/DQMURLProvider.hh"
#include "../writers/Gnuplot2DWriter.hh"

using namespace std;
namespace {
struct URLType {
  std::string url;
  bool isEB = false;
  URLType(const std::string _s, bool eb) : url(_s), isEB(eb){};
};

struct TTData {
  // TT number
  int tt;
  // EB = 0 ; EE+ = 1; EE- = -1;
  int iz;
  double value;
  TTData(int ttnum, int z, double val) : tt(ttnum), iz(z), value(val){};
  inline friend bool operator>(const TTData& a, const TTData& b) {
    return a.value > b.value;
  };
  inline friend std::ostream& operator<<(std::ostream& os, const TTData& elem) {
    os << "tt[tt=" << elem.tt << ",iz=" << elem.iz << "] = " << elem.value;
    return os;
  }
};

struct TTRunData {
  int run;
  std::vector<TTData> ttdata;
  TTRunData(int _run, const std::vector<TTData>& ttd)
      : run(_run), ttdata(ttd){};
};

std::vector<URLType> urls(const unsigned int runnumber,
                          const std::string& dataset) {
  std::vector<URLType> urls;
  const std::string plot =
      "EcalBarrel/EBTriggerTowerTask/EBTTT TT Masking Status";
  for (int i = -1; i < 2; ++i) {
    string det = "Barrel";
    string d = "EB";
    string suf = "";
    if (i != 0) {
      // endcap
      det = "Endcap";
      d = "EE";
      suf = (i == 1) ? " EE +" : " EE -";
    }
    string eeplot = "Ecal";
    eeplot +=
        det + "/" + d + "TriggerTowerTask/" + d + "TTT TT Masking Status" + suf;
    urls.push_back(URLType(DQMURL::dqmurl(runnumber, dataset, eeplot), i == 0));
  }
  return urls;
};

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

vector<TTRunData> filterTT(vector<TTRunData>& rundata) {
  assert(rundata.size() != 0);
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
}

double maxim(std::vector<TTData>& ttdata) {
  if (ttdata.size() == 0)
    return -1;
  double max = ttdata.at(0).value;
  for (auto& e : ttdata)
    if (e.value > max)
      max = e.value;
  return max;
}

vector<TTRunData> analyze(vector<TTRunData>& rundata) {
  // filter tt which all have 0 in all runs
  rundata = filterTT(rundata);
  // normalize TT value
  for (auto& e : rundata) {
    auto max = maxim(e.ttdata);
    for (auto& ee : e.ttdata) {
      ee.value /= max;
    }
  }
  return rundata;
};

void plot(const vector<TTRunData>& rundata) {
  // output in Gnuplot
  std::map<std::pair<std::string, std::string>, double> data;
  for (auto& r : rundata) {
    std::string xlabel = std::to_string(r.run);
    for (auto& tt : r.ttdata) {
      std::string det =
          ((tt.iz == 0) ? "EB  " : ((tt.iz == 1) ? "EE+ " : "EE- "));
      std::string ylabel =
          det + "TT" + (tt.tt < 10 ? "0" : "") + std::to_string(tt.tt);
      data.insert({{xlabel, ylabel}, tt.value});
    }
  }
  Gnuplot2DWriter writer(data);
  ofstream out("tt_masking.dat");
  out << writer.setZ(0, 1).setPalette({{0, "white"}, {1, "red"}}) << std::endl;
  out.close();
}

}  // namespace

void TTF4Plugin::Process() {
  vector<TTRunData> rundata;
  const auto all_channels = ECALChannels::list();
  for (auto r : runListReader->runs()) {
    std::vector<TTData> data;
    data.reserve(2500);  // approx ~3k
    for (auto url : urls(r.runnumber, r.dataset)) {
      cout << url.url << endl;
      vector<TTData> data_tt;
      if (url.isEB) {
        // parse as tt
        /** .... uhh */
        auto q = reader->parse2D(reader->get(url.url));

        for (auto& e : q) {
          // find tt by channel coord
          const auto xch = e.x + 2;
          const auto ych = e.y + 2;
          auto f = std::find_if(all_channels.begin(), all_channels.end(),
                                [xch, ych](const ChannelInfo& ch) {
                                  return ch.iphi == xch && ch.ieta == ych;
                                });
          if (f == all_channels.end()) {
            std::cout << "Cannot find channel !" << std::endl;
            std::cout << "x: " << e.x << " y: " << e.y << std::endl;
          }
          data_tt.push_back(TTData(f->tower, 0, e.value));
        }
      } else {
        // EE+ or EE-
        // parse as usual channel json
        auto data_det = reader->parse(reader->get(url.url));
        data_tt = channel2TT(data_det);
      }
      for (auto& e : data_tt) {
        data.push_back(e);
      }
    }
    rundata.push_back(TTRunData(r.runnumber, data));
  }

  plot(analyze(rundata));
}