#include "DigiOccupancy.hh"

#include <algorithm>
#include <array>
#include <fstream>
#include <iostream>
#include <map>
#include <set>
#include <string>
#include <vector>
#include "common/clusters.hh"
#include "common/common.hh"
#include "ecalchannels/ECALChannels.hh"
#include "net/DQMURLProvider.hh"
#include "net/URLCache.hh"
#include "readers/JSONReader.hh"
#include "writers/Gnuplot2DWriter.hh"
#include "writers/GnuplotECALWriter.hh"
#include "writers/ProgressBar.hh"

REGISTER_PLUGIN(DigiOccupancy);

using namespace std;
using namespace dqmcpp;

namespace {
using namespace std;
using namespace dqmcpp;

vector<string> get_urls(const ECAL::Run& run) {
  vector<string> s;
  for (int i = -18; i <= 18; ++i) {
    if (i == 0)
      continue;
    s.push_back(net::DQMURL::dqmurl(
        run, common::string_format(
                 "EcalBarrel/EBOccupancyTask/EBOT digi occupancy EB%+03d", i)));
  }
  return s;
}

void plot(const vector<ECAL::RunChannelData>& rundata) {
  writers::Gnuplot2DWriter::Data2D data;
  std::for_each(
      rundata.begin(), rundata.end(), [&data](const ECAL::RunChannelData& rd) {
        const auto xlabel = to_string(rd.run.runnumber);
        std::for_each(rd.data.begin(), rd.data.end(),
                      [&data, &xlabel](const ECAL::ChannelData& cd) {
                        const auto ylabel = common::string_format(
                            "EB [%d,%d]", cd.base.ix_iphi, cd.base.iy_ieta);
                        data.insert({{xlabel, ylabel}, cd.value});
                      });
      });
  vector<string> allruns(rundata.size());
  std::transform(rundata.begin(), rundata.end(), allruns.begin(),
                 [](const ECAL::RunChannelData& rd) {
                   return to_string(rd.run.runnumber);
                 });
  writers::Gnuplot2DWriter writer(data);
  writer.setOutput("DigiOccupancy.png");
  writer.setXlabels(allruns);
  writer.setTitle("DigiOccupancy");
  writer.setZ(0, 5);
  writer.setZTick(0.1);
  ofstream out("DigiOccupancy.plt");
  out << writer;
  out.close();
}

int LinearSquare(const std::vector<ECAL::ChannelData>& cd) {
  int maxx = -1000;
  int maxy = -1000;
  int minx = 1000;
  int miny = 1000;
  for (auto& c : cd) {
    maxx = std::max(maxx, c.base.ix_iphi);
    maxy = std::max(maxy, c.base.iy_ieta);
    minx = std::min(minx, c.base.ix_iphi);
    miny = std::min(miny, c.base.iy_ieta);
  }
  const int dx = std::abs(maxx - minx) + 1;
  const int dy = std::abs(maxy - miny) + 1;
  return dx * dy;
}

double LinearDensity(const std::vector<ECAL::ChannelData>& cd) {
  return static_cast<double>(cd.size()) / LinearSquare(cd);
}

}  // namespace

void dqmcpp::plugins::DigiOccupancy::Process() {
  auto runs = runListReader->runs();
  vector<ECAL::RunChannelData> rundata;
  writers::ProgressBar pb(runs.size());
  std::for_each(
      runs.begin(), runs.end(), [&rundata, &pb](const ECAL::Run& run) {
        pb.setLabel(run.runnumber);
        const auto content = net::URLCache::get(get_urls(run));
        vector<ECAL::ChannelData> cd;
        cd.reserve(ECAL::NEBChannels);
        std::for_each(content.begin(), content.end(),
                      [&cd](const std::string& con) {
                        auto _c1 = readers::JSONReader::parse(con);
                        cd.insert(cd.end(), _c1.begin(), _c1.end());
                      });
        // now we have all EB for run
        /* 1. get iphi median
         * 2. scale to median
         */
        for (int ieta = -85; ieta <= 85; ++ieta) {
          if (ieta == 0)
            continue;
          // get iphi channels (first part of -> std::partition)
          const auto it = std::partition(cd.begin(), cd.end(),
                                         [ieta](const ECAL::ChannelData& c) {
                                           return c.base.iy_ieta == ieta;
                                         });
          const auto median = common::median(
              cd.begin(), it,
              [](const ECAL::ChannelData& c) { return c.value; });
          std::for_each(cd.begin(), it,
                        [median](ECAL::ChannelData& c) { c.value /= median; });
        }
        rundata.emplace_back(run, cd);
        pb.increment();
      });
  // clusters
  pb.finish();
  std::for_each(rundata.begin(), rundata.end(), [](ECAL::RunChannelData& rd) {
    rd.data.erase(std::remove_if(
                      rd.data.begin(), rd.data.end(),
                      [](const ECAL::ChannelData& c) { return c.value < 1.2; }),
                  rd.data.end());
    {
      vector<ECAL::RunChannelData> _tmp = {rd};
      writers::GnuplotECALWriter writer(_tmp);
      writer.setZ(0.8, 2);
      ofstream out(to_string(rd.run.runnumber) + ".plt");
      out << writer;
      out.close();
    }
    auto clusters = common::clusters(
        rd.data, 1, [](const ECAL::ChannelData& a, const ECAL::ChannelData& b) {
          const auto dx = a.base.ix_iphi - b.base.ix_iphi;
          const auto dy = a.base.iy_ieta - b.base.iy_ieta;
          return dx * dx + dy * dy;
        });
    clusters.erase(std::remove_if(clusters.begin(), clusters.end(),
                                  [](const vector<ECAL::ChannelData>& cv) {
                                    return cv.size() < 20 ||
                                           LinearDensity(cv) < 20.0 / 25;
                                  }),
                   clusters.end());
    for (auto& c : clusters) {
      const int mx = common::mean(
          c, [](const ECAL::ChannelData& c) { return c.base.iy_ieta; });
      const int my = common::mean(
          c, [](const ECAL::ChannelData& c) { return c.base.ix_iphi; });
      cout << rd.run.runnumber << "\tsize = " << c.size() << "\tcenter[x,y] = ["
           << mx << ", " << my << "]" << endl;
    }
  });
  // remove for ordinar plot
  std::for_each(rundata.begin(), rundata.end(), [](ECAL::RunChannelData& rd) {
    rd.data.erase(
        std::remove_if(rd.data.begin(), rd.data.end(),
                       [](const ECAL::ChannelData& c) { return c.value < 2.; }),
        rd.data.end());
  });
  plot(rundata);
}