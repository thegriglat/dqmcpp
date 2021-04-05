/**
 * @file RMSMap.cc
 * @author Grigory Latyshev (thegriglat@gmail.com)
 * @brief RMS G12 ECAL map
 */

#include "RMSMap.hh"

#include <fstream>
#include <string>
#include "colors/Colors.hh"
#include "common/common.hh"
#include "net/DQMURLProvider.hh"
#include "net/URLCache.hh"
#include "readers/JSONReader.hh"
#include "writers/GnuplotECALWriter.hh"
#include "writers/ProgressBar.hh"

using namespace dqmcpp;

REGISTER_PLUGIN(RMSMap)

namespace dqmcpp {
namespace plugins {

std::vector<std::string> RMSMap::urls(const unsigned int runnumber,
                                      const std::string& dataset) {
  std::vector<std::string> urls;
  for (int i = -18; i < 19; ++i) {
    if (i == 0)
      continue;
    urls.push_back(net::DQMURL::dqmurl(
        runnumber, dataset,
        common::string_format("EcalBarrel/EBPedestalOnlineClient/EBPOT "
                              "pedestal rms map G12 EB%+03d",
                              i)));
  }
  for (int i = -9; i < 10; ++i) {
    if (i == 0)
      continue;
    urls.push_back(net::DQMURL::dqmurl(
        runnumber, dataset,
        common::string_format("EcalEndcap/EEPedestalOnlineClient/EEPOT "
                              "pedestal rms map G12 EE%+03d",
                              i)));
  }
  return urls;
}
std::vector<ECAL::RunChannelData> RMSMap::analyze(
    const std::vector<ECAL::RunChannelData>& rundata) {
  return rundata;
}
void RMSMap::plot(const std::vector<ECAL::RunChannelData>& rundata) {
  for (auto& r : rundata) {
    auto run = r.run.runnumber;
    std::string outfile = std::to_string(run) + ".plt";
    std::ofstream out(outfile);
    std::vector<ECAL::RunChannelData> rd = {r};
    writers::GnuplotECALWriter writer(rd);
    writer.setPalette(colors::PaletteSets::RMSHeatMap);
    writer.setZ(0, 10);
    writer.setZTick(1);
    out << writer << std::endl;
    out.close();
  }
}

void RMSMap::Process() {
  using namespace std;
  writers::ProgressBar progress(runListReader->runs().size());
  vector<ECAL::RunChannelData> rundata;
  for (auto& run : runListReader->runs()) {
    progress.setLabel(to_string(run.runnumber));
    vector<ECAL::ChannelData> data;
    data.reserve(ECAL::NTotalChannels);
    const auto contents = net::URLCache::get(urls(run.runnumber, run.dataset));
    for (auto& content : contents) {
      auto data_tt = readers::JSONReader::parse(content);
      data.insert(data.end(), data_tt.begin(), data_tt.end());
    }
    ECAL::RunChannelData rd(run, data);
    rundata.push_back(rd);
    progress.increment();
  }
  plot(analyze(rundata));
}

}  // namespace plugins
}  // namespace dqmcpp