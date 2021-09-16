#include "TTid.hh"

#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include "colors/Colors.hh"
#include "common/common.hh"
#include "ecalchannels/ECALChannels.hh"
#include "net/DQMURLProvider.hh"
#include "net/URLCache.hh"
#include "readers/JSONReader.hh"
#include "writers/Gnuplot2DWriter.hh"
#include "writers/ProgressBar.hh"

REGISTER_PLUGIN(TTid)

using namespace std;
using namespace dqmcpp;

namespace {
using namespace std;
using namespace dqmcpp;
vector<string> get_urls(const ECAL::Run& run, const int iz) {
  vector<string> s;
  int low = -18;
  int high = 18;
  std::string formatstr = "EcalBarrel/EBIntegrityTask/TTId/EBIT TTId EB%+03d";
  if (iz != 0)
    formatstr = "EcalEndcap/EEIntegrityTask/TTId/EEIT TTId EE%+03d";
  if (iz == -1) {
    low = -9;
    high = -1;
  }
  if (iz == 1) {
    low = 1;
    high = 9;
  }
  s.reserve(high - low + 1);
  for (int i = low; i <= high; ++i) {
    if (i == 0)
      continue;
    s.push_back(net::DQMURL::dqmurl(run, common::string_format(formatstr, i)));
  }
  return s;
}

void plot(const vector<ECAL::RunTTCCUData>& rundata) {
  writers::Gnuplot2DWriter::Data2D data;
  double _max = -1;
  const auto xlabels = common::map<ECAL::RunTTCCUData, std::string>(
      rundata, [](const ECAL::RunTTCCUData& rtcd) {
        return to_string(rtcd.run.runnumber);
      });
  for (auto& rd : rundata) {
    const string xlabel = to_string(rd.run.runnumber);
    for (auto& d : rd.data) {
      string ylabel = std::string(d.base);
      data.insert({{xlabel, ylabel}, d.value});
      _max = std::max(_max, d.value);
    }
  }
  writers::Gnuplot2DWriter writer(data);
  writer.setXlabels(xlabels);
  writer.setOutput("TTid.png");
  writer.setPalette(colors::PaletteSets::Heatmap);
  if (_max > 0)
    writer.setZ(0, _max);
  writer.setZTick(10);
  writer.setTitle("TTId");
  writer.setLogscale("cb");
  ofstream out("TTid.plt");
  out << writer;
  out.close();
}

}  // namespace

void dqmcpp::plugins::TTid::Process() {
  writers::ProgressBar pb(runListReader->runs().size());
  std::vector<ECAL::RunTTCCUData> rundata;
  const auto channels = ECALChannels::list();
  for (auto& run : runListReader->runs()) {
    pb.setLabel(to_string(run.runnumber));
    pb.increment();
    ECAL::RunTTCCUData ttdata(run, {});
    for (int iz = -1; iz <= 1; ++iz) {
      const auto urls = get_urls(run, iz);
      const auto contents = net::URLCache::get(urls);
      for (auto& content : contents) {
        if (iz != 0) {
          const auto d2d = readers::JSONReader::parse2D(content);
          vector<ECAL::TTCCUData> _localtt;
          for (auto& d : d2d) {
            auto it = std::find_if(
                channels.begin, channels.end,
                [&d, iz](const ECALChannels::ChannelInfo& ci) {
                  return std::abs(ci.ix - d.base.x) < 2.5 &&
                         std::abs(ci.iy - d.base.y) < 2.5 && ci.iz == iz;
                });
            if (it == channels.end) {
              cout << endl << run << " cannot determine ccu" << d.base << endl;
            } else {
              _localtt.emplace_back(ECAL::CCU(it->ccu, it->tcc, iz), d.value);
            }
          }
          ttdata.data.insert(ttdata.data.end(), _localtt.begin(),
                             _localtt.end());

        } else {
          const auto ttd =
              ECAL::channel2TT(readers::JSONReader::parse(content));
          for (auto& e : ttd) {
            ttdata.data.emplace_back(e.base, e.value);
          }
        }
      }
    }
    rundata.push_back(ttdata);
  }
  plot(rundata);
}