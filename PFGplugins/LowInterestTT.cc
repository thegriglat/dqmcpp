#include "LowInterestTT.hh"

#include <fstream>
#include <string>
#include <vector>
#include "colors/Colors.hh"
#include "common/common.hh"
#include "ecalchannels/ECALChannels.hh"
#include "net/DQMURLProvider.hh"
#include "readers/JSONReader.hh"
#include "writers/Gnuplot2DWriter.hh"
#include "writers/ProgressBar.hh"

#define LOWLIMIT (0.9)

REGISTER_PLUGIN(LowInterestTT)

using namespace std;
using namespace dqmcpp;

namespace {

using namespace std;
using namespace dqmcpp;

string url(const ECAL::Run& run) {
  return net::DQMURL::dqmurl(
      run, "EcalBarrel/EBSelectiveReadoutTask/EBSRT low interest TT Flags");
}

void plot(const vector<ECAL::RunTTData>& rundata) {
  writers::ProgressBar progress(rundata.size());
  progress.setLabel("plotting...");
  writers::Gnuplot2DWriter::Data2D data;
  vector<string> runss;
  for (auto rd : rundata) {
    runss.push_back(to_string(rd.run.runnumber));
  }
  for (auto& rd : rundata) {
    const auto xlabel = to_string(rd.run.runnumber);
    for (auto& tt : rd.data) {
      const auto ylabel = std::string(tt.base);
      data.insert({{xlabel, ylabel}, tt.value});
    }
    progress.increment();
  }
  writers::Gnuplot2DWriter writer(data);
  writer.setXlabels(runss);
  writer.setOutput("LowInterestTT.png");
  writer.setTitle("LowInterestTT");
  writer.setZ(0.0, 1.0);
  writer.setPalette(colors::PaletteSets::Rainbow);
  writer.setZTick(0.1);
  ofstream out("LowInterestTT.plt");
  out << writer;
  out.close();
}

}  // namespace

void dqmcpp::plugins::LowInterestTT::Process() {
  const auto runs = runListReader->runs();
  writers::ProgressBar pb(runs.size());
  vector<ECAL::RunTTData> rundata;
  for (auto& run : runs) {
    pb.setLabel(to_string(run.runnumber));
    auto content = ECAL::channel2TT(ECAL::Data2D2ChannelData(
        readers::JSONReader::parse2D(readers::JSONReader::get(url(run)))));
    auto removeit = std::remove_if(
        content.begin(), content.end(),
        [](const ECAL::TTData& ttd) { return ttd.value > LOWLIMIT; });
    content.erase(removeit, content.end());
    rundata.emplace_back(run, content);
    pb.increment();
  }
  pb.finish();
  plot(rundata);
}