#include "MeanChannels.hh"

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

std::vector<std::string> MeanChannels::urls(const ECAL::Run& run) {
  std::vector<std::string> urls;
  for (int i = -18; i < 19; ++i) {
    if (i == 0)
      continue;
    const auto plot = common::string_format(
        "EcalBarrel/EBPedestalOnlineTask/Gain12/EBPOT pedestal EB%+03d G12", i);
    urls.push_back(net::DQMURL::dqmurl(run.runnumber, run.dataset, plot));
  }
  for (int i = -9; i < 10; ++i) {
    if (i == 0)
      continue;
    const auto plot = common::string_format(
        "EcalEndcap/EEPedestalOnlineTask/Gain12/EEPOT pedestal EE%+03d G12", i);
    urls.push_back(net::DQMURL::dqmurl(run.runnumber, run.dataset, plot));
  }
  return urls;
};

void MeanChannels::Process() {
  const auto rundata = getRunData();
  vector<Analysis> analyses = {
      {"mean_channels_greater_300", "ECAL G12 Mean > 300",
       [](const double mean) { return mean > 300.0; }},
      {"mean_channels_less_100", "ECAL G12 Mean < 100",
       [](const double mean) { return mean < 100 && mean > 0; }}};
  for (auto& analysis : analyses) {
    std::cout << "## Run subanalysis " << analysis.title << std::endl;
    plot(analyze(rundata, analysis.func), analysis.filename, analysis.title,
         writers::Axis(0, 300), colors::PaletteSets::MeanHeatMap);
  }
}

}  // namespace plugins
}  // namespace dqmcpp