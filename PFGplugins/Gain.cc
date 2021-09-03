#include "Gain.hh"

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

#define MINVALUETOSKIP (5.0);
REGISTER_PLUGIN(Gain)

using namespace std;
using namespace dqmcpp;

namespace {
using namespace std;
using namespace dqmcpp;
vector<string> get_urls(const ECAL::Run& run, const int iz) {
  vector<string> s;
  int low = -18;
  int high = 18;
  std::string formatstr = "EcalBarrel/EBIntegrityTask/Gain/EBIT gain EB%+03d";
  if (iz != 0)
    formatstr = "EcalEndcap/EEIntegrityTask/Gain/EEIT gain EE%+03d";
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

void plot(const vector<ECAL::RunChannelData>& rundata) {
  writers::Gnuplot2DWriter::Data2D data;
  for (auto& rd : rundata) {
    const string xlabel = to_string(rd.run.runnumber);
    for (auto& d : rd.data) {
      const string ylabel = std::string(d.base);
      data.insert({{xlabel, ylabel}, d.value});
    }
  }
  writers::Gnuplot2DWriter writer(data);
  writer.setOutput("Gain.png");
  writer.setPalette(colors::PaletteSets::Heatmap);
  writer.setZ(0, 1);
  writer.setZTick(0.1);
  writer.setTitle("Gain");
  ofstream out("Gain.plt");
  out << writer;
  out.close();
}

bool skipFn(const ECAL::ChannelData& cd) {
  return cd.value < MINVALUETOSKIP;
}

void normalise(std::vector<ECAL::ChannelData>& cdv) {
  const auto maxe = std::max_element(
      cdv.begin(), cdv.end(),
      [](const ECAL::ChannelData& a, const ECAL::ChannelData& b) {
        return a.value < b.value;
      });
  for (auto& e : cdv) {
    e.value /= maxe->value;
  }
}

}  // namespace

void dqmcpp::plugins::Gain::Process() {
  writers::ProgressBar progress(runListReader->runs().size());
  std::vector<ECAL::RunChannelData> rundata;
  for (auto& run : runListReader->runs()) {
    progress.setLabel(to_string(run.runnumber));
    progress.increment();
    ECAL::RunChannelData chdata(run, {});
    for (int iz = -1; iz <= 1; ++iz) {
      const auto urls = get_urls(run, iz);
      const auto contents = net::URLCache::get(urls);
      for (auto& content : contents) {
        const auto chd = readers::JSONReader::parse(content);
        chdata.data.insert(chdata.data.end(), chd.begin(), chd.end());
      }
    }
    chdata.data.erase(
        std::remove_if(chdata.data.begin(), chdata.data.end(), skipFn),
        chdata.data.end());
    normalise(chdata.data);
    rundata.push_back(chdata);
  }
  plot(rundata);
}