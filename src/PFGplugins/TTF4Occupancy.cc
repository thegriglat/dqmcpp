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
#include "TTMaskingStatus.hh"
#include "common/common.hh"
#include "ecalchannels/ECALChannels.hh"
#include "net/DQMURLProvider.hh"
#include "readers/JSONReader.hh"
#include "writers/Gnuplot2DWriter.hh"

using namespace std;
using namespace dqmcpp;

REGISTER_PLUGIN(TTF4Occupancy)

namespace {

struct URLType {
  std::string url;
  bool isEB = false;
  URLType(const std::string& _s, bool eb) : url(_s), isEB(eb){};
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

vector<ECAL::RunTTData> getMaskedChannels(
    dqmcpp::readers::RunListReader* runlistreader) {
  dqmcpp::plugins::TTMaskingStatus ttmasking;
  ttmasking.setRunListReader(runlistreader);
  return ttmasking.get();
};

}  // namespace

namespace dqmcpp {
namespace plugins {

std::vector<ECAL::RunTTData> TTF4Occupancy::readTT() {
  using namespace ECAL;
  vector<RunTTData> rundata;
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
        auto q =
            readers::JSONReader::parse2D(readers::JSONReader::get(url.url));

        for (auto& e : q) {
          // find tt by channel coord
          const int xch = (int)e.base.x;
          const int ych = (int)e.base.y;
          auto f =
              std::find_if(all_channels->begin(), all_channels->end(),
                           [xch, ych](const ECALChannels::ChannelInfo& ch) {
                             return ch.iphi == xch && ch.ieta == ych;
                           });
          if (f == all_channels->end()) {
            std::cout << "Cannot find channel !" << std::endl;
            std::cout << "x: " << xch << " y: " << ych << std::endl;
          }
          data_tt.push_back(TTData(TT(f->tower, f->tcc, 0), e.value));
        }
      } else {
        // EE+ or EE-
        // parse as usual channel json
        auto data_det =
            readers::JSONReader::parse(readers::JSONReader::get(url.url));
        data_tt = channel2TT(data_det);
      }
      for (auto& e : data_tt) {
        data.push_back(e);
      }
    }
    rundata.push_back(RunTTData(r, data));
  }
  return rundata;
};

void TTF4Occupancy::Process() {
  using namespace dqmcpp;
  auto maskedtt = getMaskedChannels(runListReader);
  auto occupancy_tt = readTT();
  occupancy_tt = filterZeroTT(occupancy_tt);
  // scale to max
  for (auto& ttrun : occupancy_tt) {
    auto maxe =
        std::max_element(ttrun.data.begin(), ttrun.data.end(),
                         [](const ECAL::TTData& a, const ECAL::TTData& b) {
                           return a.value < b.value;
                         });
    auto max = maxe->value;
    for (auto& ee : ttrun.data) {
      ee.value /= max;
    }
  }
  // remove all masked tt
  for (auto& occtt : occupancy_tt) {
    // find masked run
    auto masked_it = std::find_if(
        maskedtt.begin(), maskedtt.end(),
        [&occtt](const ECAL::RunTTData& rd) { return occtt.run == rd.run; });
    if (masked_it == maskedtt.end()) {
      // not found
      std::cout << "Masked run " << occtt.run.runnumber << " not found!"
                << std::endl;
      continue;
    }
    // iterate over tt and remove masked
    occtt.data.erase(
        std::remove_if(occtt.data.begin(), occtt.data.end(),
                       [masked_it](const ECAL::TTData& ttdata) {
                         auto pos = std::find_if(
                             masked_it->data.begin(), masked_it->data.end(),
                             [&ttdata](const ECAL::TTData& maskedtt) {
                               return maskedtt.base == ttdata.base;
                             });
                         return (pos != masked_it->data.end());
                       }),
        occtt.data.end());
  }

  // plot
  std::map<std::pair<std::string, std::string>, double> data;
  for (auto& r : occupancy_tt) {
    std::string xlabel = std::to_string(r.run.runnumber);
    for (auto& tt : r.data) {
      std::string ylabel = std::string(tt.base);
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
