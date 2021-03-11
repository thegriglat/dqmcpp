/**
 * @file TestPlugin.cc
 * @author Grigory Latyshev (thegriglat@gmail.com)
 * @brief RMS G12 ECAL map
 */

#include "TestPlugin.hh"

#include <fstream>
#include <string>
#include "../net/DQMURLProvider.hh"
#include "../writers/GnuplotECALWriter.hh"

std::vector<std::string> TestPlugin::urls(const unsigned int runnumber,
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
    urls.push_back(DQMURL::dqmurl(runnumber, dataset, plot));
  }
  for (int i = -9; i < 10; ++i) {
    if (i == 0)
      continue;
    sprintf(
        plot,
        "EcalEndcap/EEPedestalOnlineClient/EEPOT pedestal rms map G12 EE%+03d",
        i);
    urls.push_back(DQMURL::dqmurl(runnumber, dataset, plot));
  }
  delete[] plot;
  return urls;
}
std::vector<ECAL::RunData> TestPlugin::analyze(
    const std::vector<ECAL::RunData>& rundata) {
  return rundata;
}
void TestPlugin::plot(const std::vector<ECAL::RunData>& rundata) {
  for (auto& r : rundata) {
    auto run = r.run.runnumber;
    std::string outfile = std::to_string(run) + ".plt";
    std::ofstream out(outfile);
    std::vector<ECAL::RunData> rd = {r};
    out << GnuplotECALWriter(rd) << std::endl;
    out.close();
  }
}

void TestPlugin::Process() {
  using namespace std;
  vector<ECAL::RunData> rundata;
  for (auto& run : runListReader->runs()) {
    vector<ECAL::ChannelData> data;
    data.reserve(ECAL::NTotalChannels);
    for (auto& url : urls(run.runnumber, run.dataset)) {
      cout << url << endl;
      auto data_tt = reader->parse(reader->get(url));
      for (auto& e : data_tt)
        data.push_back(e);
    }
    ECAL::RunData rd(run, data);
    rundata.push_back(rd);
  }
  plot(analyze(rundata));
}