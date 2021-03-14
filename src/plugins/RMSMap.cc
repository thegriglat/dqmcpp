/**
 * @file RMSMap.cc
 * @author Grigory Latyshev (thegriglat@gmail.com)
 * @brief RMS G12 ECAL map
 */

#include "RMSMap.hh"

#include <fstream>
#include <string>
#include "../colors/Colors.hh"
#include "../net/DQMURLProvider.hh"
#include "../writers/GnuplotECALWriter.hh"

using namespace dqmcpp;

namespace dqmcpp {
namespace plugins {

std::vector<std::string> RMSMap::urls(const unsigned int runnumber,
                                      const std::string& dataset) {
  std::vector<std::string> urls;
  char* plot = new char[72];
  for (int i = -18; i < 19; ++i) {
    if (i == 0)
      continue;
    sprintf(
        plot,
        "EcalBarrel/EBPedestalOnlineClient/EBPOT pedestal rms map G12 EB%+03d",
        i);
    urls.push_back(net::DQMURL::dqmurl(runnumber, dataset, plot));
  }
  for (int i = -9; i < 10; ++i) {
    if (i == 0)
      continue;
    sprintf(
        plot,
        "EcalEndcap/EEPedestalOnlineClient/EEPOT pedestal rms map G12 EE%+03d",
        i);
    urls.push_back(net::DQMURL::dqmurl(runnumber, dataset, plot));
  }
  delete[] plot;
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
  vector<ECAL::RunChannelData> rundata;
  for (auto& run : runListReader->runs()) {
    vector<ECAL::ChannelData> data;
    data.reserve(ECAL::NTotalChannels);
    for (auto& url : urls(run.runnumber, run.dataset)) {
      cout << url << endl;
      auto data_tt = reader->parse(reader->get(url));
      for (auto& e : data_tt)
        data.push_back(e);
    }
    ECAL::RunChannelData rd(run, data);
    rundata.push_back(rd);
  }
  plot(analyze(rundata));
}

}  // namespace plugins
}  // namespace dqmcpp