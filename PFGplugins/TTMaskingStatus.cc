/**
 * @file TTMaskingStatus.cc
 * @author Grigory Latyshev (thegriglat@gmail.com)
 * @brief TTMaskingStatus implementation
 */
#include "TTMaskingStatus.hh"

#include <algorithm>
#include <fstream>
#include <iostream>
#include <map>
#include <string>
#include <vector>
#include "ECAL/ECAL.hh"
#include "common/common.hh"
#include "ecalchannels/ECALChannels.hh"
#include "net/DQMURLProvider.hh"
#include "readers/JSONReader.hh"
#include "writers/Gnuplot2DWriter.hh"
#include "writers/ProgressBar.hh"

using namespace dqmcpp::ECAL;
using namespace dqmcpp;

using namespace std;

REGISTER_PLUGIN(TTMaskingStatus)

namespace {

struct URLType {
  std::string url;
  bool isEB = false;
  URLType(const std::string& _s, bool eb) : url(_s), isEB(eb){};
};

std::vector<URLType> urls(const int runnumber, const std::string& dataset) {
  std::vector<URLType> urls;
  for (int i = -1; i < 2; ++i) {
    if (i == 0) {
      // eb
      urls.push_back(
          URLType(net::DQMURL::dqmurl(
                      runnumber, dataset,
                      "EcalBarrel/EBTriggerTowerTask/EBTTT TT Masking Status"),
                  true));
    } else {
      const std::string pm = (i == 1) ? "EE +" : "EE -";
      urls.push_back(URLType(
          net::DQMURL::dqmurl(
              runnumber, dataset,
              "EcalEndcap/EETriggerTowerTask/EETTT TT Masking Status " + pm),
          false));
    }
  }
  return urls;
};

vector<RunTTData> analyze(vector<RunTTData>& rundata) {
  // filter tt which all have 0 in all runs
  rundata = filterZeroTT(rundata);
  // normalize TT value
  for (auto& e : rundata) {
    auto maxe = std::max_element(
        e.data.begin(), e.data.end(),
        [](const TTData& a, const TTData& b) { return a.value < b.value; });
    for (auto& ee : e.data) {
      ee.value /= maxe->value;
    }
  }
  return rundata;
};

void plot(const vector<RunTTData>& rundata) {
  // output in Gnuplot
  std::map<std::pair<std::string, std::string>, double> data;
  for (auto& r : rundata) {
    std::string xlabel = std::to_string(r.run.runnumber);
    for (auto& tt : r.data) {
      std::string ylabel = std::string(tt.base);
      data.insert({{xlabel, ylabel}, tt.value});
    }
  }
  writers::Gnuplot2DWriter writer(data);
  ofstream out("tt_masking_status.plt");
  writer.setZ(0, 1);
  writer.setZTick(0.1);
  writer.setOutput("MaskingStatus.png");
  writer.setTitle("TT Masking Status");
  out << writer << std::endl;
  out.close();
}

}  // namespace

namespace dqmcpp {
namespace plugins {

using namespace dqmcpp;
std::vector<RunTTData> TTMaskingStatus::Init() const {
  vector<RunTTData> rundata;
  writers::ProgressBar pb(runListReader->runs().size());
  const auto all_channels = ECALChannels::list();
  for (auto r : runListReader->runs()) {
    pb.setLabel(r.runnumber);
    pb.increment();
    std::vector<TTData> data;
    data.reserve(2500);  // approx ~3k
    for (auto url : urls(r.runnumber, r.dataset)) {
      vector<TTData> data_tt;
      if (url.isEB) {
        // parse as tt
        /** .... uhh */
        auto q =
            readers::JSONReader::parse2D(readers::JSONReader::get(url.url));

        for (auto& e : q) {
          // find tt by channel coord
          const int xch = e.base.x;
          const int ych = e.base.y;
          auto f =
              std::find_if(all_channels.begin, all_channels.end,
                           [xch, ych](const ECALChannels::ChannelInfo& ch) {
                             return ch.iphi == xch && ch.ieta == ych;
                           });
          if (f == all_channels.end) {
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
}

void TTMaskingStatus::Process() {
  auto rundata = Init();
  plot(analyze(rundata));
}

std::vector<RunTTData> TTMaskingStatus::get() {
  auto rundata = Init();
  return rundata;
}

}  // namespace plugins
}  // namespace dqmcpp
