#include "RMSChannels.hh"

#include "../colors/Colors.hh"
#include "../common/common.hh"
#include "../net/DQMURLProvider.hh"
#include "../writers/Gnuplot2D.hh"

using namespace std;
using namespace dqmcpp;

namespace dqmcpp {
namespace plugins {

struct Analysis {
  std::string filename;
  std::string title;
  std::function<bool(double)> func;
};

std::vector<std::string> RMSChannels::urls(const ECAL::Run& run) {
  std::vector<std::string> urls;
  for (int i = -18; i < 19; ++i) {
    if (i == 0)
      continue;
    const auto plot = common::string_format(
        "EcalBarrel/EBPedestalOnlineClient/EBPOT pedestal rms map "
        "G12 EB%+03d",
        i);
    urls.push_back(net::DQMURL::dqmurl(run.runnumber, run.dataset, plot));
  }
  for (int i = -9; i < 10; ++i) {
    if (i == 0)
      continue;
    const auto plot = common::string_format(
        "EcalEndcap/EEPedestalOnlineClient/EEPOT pedestal rms map G12 EE%+03d",
        i);
    urls.push_back(net::DQMURL::dqmurl(run.runnumber, run.dataset, plot));
  }
  return urls;
};

void RMSChannels::Process() {
  const auto rundata = getRunData();
  vector<Analysis> analyses = {
      {"rms_channels_greater_8", "ECAL G12 RMS > 8",
       [](const double rms) { return rms > 8.0; }},
      {"rms_channels_less_1.5", "ECAL G12 RMS < 1.5",
       [](const double rms) { return rms < 1.5 && rms > 0; }}};
  for (auto& analysis : analyses) {
    std::cout << "## Run subanalysis " << analysis.title << std::endl;
    plot(analyze(rundata, analysis.func), analysis.filename, analysis.title,
         writers::Axis(0, 10), colors::PaletteSets::RMSHeatMap);
  }
}

}  // namespace plugins
}  // namespace dqmcpp