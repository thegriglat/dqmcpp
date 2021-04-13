#include "DigiOccupancy.hh"

#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include "common/common.hh"
#include "net/DQMURLProvider.hh"
#include "net/URLCache.hh"
#include "readers/JSONReader.hh"
#include "writers/Gnuplot2DWriter.hh"
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
         * 3. get channels > 20% of median
         */
        for (int iphi = 1; iphi <= 360; ++iphi) {
          // get iphi channels (first part of -> std::partition)
          const auto it = std::partition(cd.begin(), cd.end(),
                                         [iphi](const ECAL::ChannelData& c) {
                                           return c.base.ix_iphi == iphi;
                                         });
          // get median
          const auto median = common::median(
              cd.begin(), it,
              [](const ECAL::ChannelData& c) { return c.value; });
          // scale
          std::for_each(cd.begin(), it,
                        [median](ECAL::ChannelData& c) { c.value /= median; });
        }
        // filter
        auto rmit = std::remove_if(
            cd.begin(), cd.end(),
            [](const ECAL::ChannelData& c) { return c.value < 2.0; });
        cd.erase(rmit, cd.end());
        rundata.emplace_back(run, cd);
        pb.increment();
      });
  plot(rundata);
}