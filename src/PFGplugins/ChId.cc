#include "ChId.hh"

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

// TODO: plugins are disabled as all DQM plots are empty
// REGISTER_PLUGIN(ChId)

using namespace std;
using namespace dqmcpp;

namespace {
using namespace std;
using namespace dqmcpp;
vector<string> get_urls(const ECAL::Run& run) {
  vector<string> s;
  s.reserve(18 * 2);
  for (int i = -18; i <= 18; ++i) {
    if (i == 0)
      continue;
    s.push_back(net::DQMURL::dqmurl(
        run, common::string_format(
                 "EcalBarrel/EBIntegrityTask/ChId/EBIT ChId EB%+03d", i)));
  }
  return s;
}

void plot(const vector<ECAL::RunChannelData>& rundata) {
  writers::Gnuplot2DWriter::Data2D data;
  double _max = -1;
  for (auto& rd : rundata) {
    const string xlabel = to_string(rd.run.runnumber);
    for (auto& d : rd.data) {
      const auto info = ECALChannels::find(d.channel);
      if (!info) {
        cout << "cannot find channel " << d.channel << endl;
        continue;
      }
      const auto arr3 = d.channel.asArray();
      const string ylabel = common::string_format(
          "%s [%+03d,%+03d]", info->det(), arr3[0], arr3[1]);
      data.insert({{xlabel, ylabel}, d.value});
      _max = std::max(_max, d.value);
    }
  }
  writers::Gnuplot2DWriter writer(data);
  writer.setOutput("ChId.png");
  writer.setPalette(colors::PaletteSets::Heatmap);
  if (_max > 0)
    writer.setZ(0, _max);
  writer.setZTick(10);
  writer.setTitle("ChId");
  writer.setLogscale("cb");
  ofstream out("ChId.plt");
  out << writer;
  out.close();
}

}  // namespace

void dqmcpp::plugins::ChId::Process() {
  writers::ProgressBar progress(runListReader->runs().size());
  std::vector<ECAL::RunChannelData> rundata;
  for (auto& run : runListReader->runs()) {
    progress.setLabel(to_string(run.runnumber));
    progress.increment();
    const auto urls = get_urls(run);
    const auto contents = net::URLCache::get(urls);
    ECAL::RunChannelData chdata(run, {});
    for (auto& content : contents) {
      const auto chd = readers::JSONReader::parse(content);
      chdata.data.insert(chdata.data.end(), chd.begin(), chd.end());
    }
    rundata.push_back(chdata);
  }
  plot(rundata);
}