#include "TTMaskingStatus.hh"

#include <algorithm>
#include <fstream>
#include <iostream>
#include <map>
#include <string>
#include <vector>
#include "../common/common.hh"
#include "../dataclasses/ECAL.hh"
#include "../dataclasses/ecalchannels.hh"
#include "../readers/DQMURLProvider.hh"
#include "../writers/Gnuplot2DWriter.hh"

using namespace ECAL;

using namespace std;
namespace {
struct URLType {
  std::string url;
  bool isEB = false;
  URLType(const std::string _s, bool eb) : url(_s), isEB(eb){};
};

std::vector<URLType> urls(const unsigned int runnumber,
                          const std::string& dataset) {
  std::vector<URLType> urls;
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

vector<TTRunData> analyze(vector<TTRunData>& rundata) {
  // filter tt which all have 0 in all runs
  rundata = filterZeroTT(rundata);
  // normalize TT value
  for (auto& e : rundata) {
    auto max =
        maximum<TTData>(e.ttdata, [](const TTData& e) { return e.value; });
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
  ofstream out("tt_masking_status.plt");
  out << writer.setZ(0, 1)
             .setPalette({{0, "white"}, {1, "red"}})
             .setZTick(0.1)
             .output("MaskingStatus.png")
             .title("TT Masking Status")
      << std::endl;
  out.close();
}

}  // namespace

std::vector<TTRunData> TTMaskingStatus::Init() {
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
          const auto xch = e.x + 1;
          const auto ych = e.y + 1;
          auto f = std::find_if(all_channels.begin(), all_channels.end(),
                                [xch, ych](const ChannelInfo& ch) {
                                  return ch.iphi == xch && ch.ieta == ych;
                                });
          if (f == all_channels.end()) {
            std::cout << "Cannot find channel !" << std::endl;
            std::cout << "x: " << e.x << " y: " << e.y << std::endl;
          }
          data_tt.push_back(TTData(f->tower, 0, f->tcc, e.value));
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
  return rundata;
}

void TTMaskingStatus::Process() {
  auto rundata = Init();
  plot(analyze(rundata));
}

std::vector<TTRunData> TTMaskingStatus::get() {
  auto rundata = Init();
  return rundata;
}