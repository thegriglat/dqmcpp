/**
 * @file TTF4Occupancy.cc
 * @author Grigory Latyshev (thegriglat@gmail.com)
 * @brief TTF4Occupancy implementation
 */
#include "TTF4Occupancy.hh"
#include <algorithm>
#include <fstream>
#include <map>
#include <string>
#include "../ECAL/ECALChannels.hh"
#include "../common/common.hh"
#include "../net/DQMURLProvider.hh"
#include "../writers/Gnuplot2DWriter.hh"
#include "TTMaskingStatus.hh"

using namespace std;
using namespace dqmcpp;

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
        det + "/" + d + "TriggerTowerTask/" + d + "TTT TTF4 Occupancy" + suf;
    urls.push_back(
        URLType(net::DQMURL::dqmurl(runnumber, dataset, eeplot), i == 0));
  }
  return urls;
};

vector<ECAL::TTRunData> getMaskedChannels(
    dqmcpp::readers::Reader* reader,
    dqmcpp::readers::RunListReader* runlistreader) {
  dqmcpp::plugins::TTMaskingStatus ttmasking;
  ttmasking.setReader(reader);
  ttmasking.setRunListReader(runlistreader);
  return ttmasking.get();
};

}  // namespace

namespace dqmcpp {
namespace plugins {

std::vector<ECAL::TTRunData> TTF4Occupancy::readTT() {
  using namespace ECAL;
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
          const int xch = (int)e.x;
          const int ych = (int)e.y;
          auto f = std::find_if(
              all_channels.begin(), all_channels.end(),
              [xch, ych](const ECAL::ECALChannels::ChannelInfo& ch) {
                return ch.iphi == xch && ch.ieta == ych;
              });
          if (f == all_channels.end()) {
            std::cout << "Cannot find channel !" << std::endl;
            std::cout << "x: " << xch << " y: " << ych << std::endl;
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
};

void TTF4Occupancy::Process() {
  using namespace dqmcpp;
  auto maskedtt = getMaskedChannels(reader, runListReader);
  auto occupancy_tt = readTT();
  occupancy_tt = filterZeroTT(occupancy_tt);
  // scale to max
  for (auto& ttrun : occupancy_tt) {
    auto max = common::maximum<ECAL::TTData>(
        ttrun.ttdata, [](const ECAL::TTData& e) { return e.value; });
    for (auto& ee : ttrun.ttdata) {
      ee.value /= max;
    }
  }
  // remove all masked tt
  for (auto& occtt : occupancy_tt) {
    // find masked run
    auto masked_it = std::find_if(
        maskedtt.begin(), maskedtt.end(),
        [&occtt](const ECAL::TTRunData& rd) { return occtt.run == rd.run; });
    if (masked_it == maskedtt.end()) {
      // not found
      std::cout << "Masked run " << occtt.run << " not found!" << std::endl;
      continue;
    }
    // iterate over tt and remove masked
    occtt.ttdata.erase(
        std::remove_if(occtt.ttdata.begin(), occtt.ttdata.end(),
                       [masked_it](const ECAL::TTData& ttdata) {
                         auto pos = std::find_if(
                             masked_it->ttdata.begin(), masked_it->ttdata.end(),
                             [&ttdata](const ECAL::TTData& maskedtt) {
                               return maskedtt.iz == ttdata.iz &&
                                      maskedtt.tt == ttdata.tt &&
                                      maskedtt.tcc == ttdata.tcc;
                             });
                         return (pos != masked_it->ttdata.end());
                       }),
        occtt.ttdata.end());
  }

  // plot
  std::map<std::pair<std::string, std::string>, double> data;
  for (auto& r : occupancy_tt) {
    std::string xlabel = std::to_string(r.run);
    for (auto& tt : r.ttdata) {
      std::string det = ECAL::ECALChannels::detByTTTTC(tt.tt, tt.tcc);
      std::string ylabel = det + " TCC" + std::to_string(tt.tcc) + " TT" +
                           (tt.tt < 10 ? "0" : "") + std::to_string(tt.tt);
      data.insert({{xlabel, ylabel}, tt.value});
    }
  }
  writers::Gnuplot2DWriter writer(data);
  ofstream out("ttf4_occupancy.plt");
  writer.setZ(0, 1);
  writer.setZTick(0.1);
  writer.setOutput("ttf4_occupancy.png");
  writer.setTitle("TTF4 Occupancy");
  out << writer << std::endl;
  out.close();
}

}  // namespace plugins
}  // namespace dqmcpp