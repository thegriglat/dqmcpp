#include "ChannelPlugin.hh"
/**
 * @file RMSChannels.cc
 * @author Grigory Latyshev (thegriglat@gmail.com)
 * @brief Heatmap with noisy RMS channels
 */
// #include <cmath>
#include <array>
#include <fstream>
#include <functional>
#include <map>
#include <set>
#include <string>
#include "common/common.hh"
#include "ecalchannels/ECALChannels.hh"
#include "filters/ChannelStatus.hh"
#include "net/URLCache.hh"
#include "readers/JSONReader.hh"
#include "writers/Gnuplot2DWriter.hh"
#include "writers/ProgressBar.hh"

using namespace dqmcpp;
using namespace std;

#define MAXSTATUS4BOX (10)

namespace {

std::string getYlabel(const ECAL::Channel& channel, const int status) {
  auto s = std::string(channel);
  if (status > 0) {
    s += " ^{" + to_string(status) + "}";
  }
  return s;
}
}  // namespace

namespace dqmcpp {
namespace plugins {

std::vector<std::string> ChannelPlugin::urls(const ECAL::Run&) {
  return std::vector<std::string>();
}

// channel and number of channel's repetitions
using BadChannel = std::pair<ECAL::Channel, int>;
std::vector<ECAL::RunChannelData> ChannelPlugin::analyze(

    const std::vector<ECAL::RunChannelData>& rundata,
    std::function<bool(double)> checkfn) {
  std::map<ECAL::Channel, int> badchannels_map;
  writers::ProgressBar progress(rundata.size() * 2);
  progress.setLabel("analyze ...");
  for (auto& e : rundata) {
    progress.increment();
    for (auto& channeldata : e.data) {
      if (checkfn(channeldata.value)) {
        badchannels_map[channeldata.base]++;
      }
    }
  }
  set<ECAL::Channel> badchannels;

  for (auto& pair : badchannels_map) {
    if (pair.second > 1)
      badchannels.insert(pair.first);
  }
  progress.setLabel("copy ...");
  std::vector<ECAL::RunChannelData> rd;
  rd.reserve(rundata.size());
  for (auto& e : rundata) {
    std::vector<ECAL::ChannelData> bd;
    bd.reserve(badchannels.size());
    std::copy_if(e.data.begin(), e.data.end(), std::back_inserter(bd),
                 [&badchannels](const ECAL::ChannelData& ecd) {
                   return badchannels.find(ecd.base) != badchannels.end();
                 });
    rd.push_back(ECAL::RunChannelData(e.run, bd));
    progress.increment();
  }
  return rd;
}

void ChannelPlugin::plot(const std::vector<ECAL::RunChannelData>& rundata,
                         const std::string& filename,
                         const std::string& title,
                         const writers::Axis& zaxis,
                         const writers::Palette& palette) {
  if (rundata.size() == 0)
    return;
  writers::Gnuplot2DWriter::Data2D data;
  std::vector<std::pair<std::string, std::string>> boxes;
  const auto& lastrun = rundata.back().run;
  writers::ProgressBar progress(rundata.size() * 2);
  progress.setLabel("find bad ...");
  std::set<ECAL::Channel> badchannels;
  std::map<ECAL::Channel, int> channelstatus;
  for (auto& rd : rundata) {
    for (auto& chd : rd.data) {
      badchannels.insert(chd.base);
    }
    progress.increment();
  }
  const filters::ChannelStatus lastRunStatus(lastrun);
  for (auto& b : badchannels) {
    channelstatus[b] = lastRunStatus[b];
  }
  for (auto& rd : rundata) {
    progress.setLabel(to_string(rd.run.runnumber));
    const auto xlabel = std::to_string(rd.run.runnumber);
    const auto status = filters::ChannelStatus(rd.run);
    for (auto& b : badchannels) {
      const auto ylabel = getYlabel(b, channelstatus.at(b));
      const auto channel_status = status[b];
      if (channel_status > MAXSTATUS4BOX) {
        boxes.emplace_back(xlabel, ylabel);
      }
      int idx = common::index(
          rd.data, [&b](const ECAL::ChannelData& cd) { return cd.base == b; });
      if (idx != -1) {
        data.insert({{xlabel, ylabel}, rd.data.at(idx).value});
      }
    }
    progress.increment();
  }
  progress.finish();
  cout << "Output to gnuplot ..." << endl;
  std::ofstream out(filename + ".plt");
  writers::Gnuplot2DWriter writer(data);
  writer.setTitle(title);
  writer.setZ(zaxis);
  writer.setPalette(palette);
  const int ztick = (zaxis.maxd() - zaxis.mind()) / 10.;
  writer.setZTick(ztick);
  writer.setOutput(filename + ".png");
  for (auto& b : boxes) {
    writer.addBox(b);
  }
  out << writer << std::endl;
  out.close();
  return;
}

vector<ECAL::RunChannelData> ChannelPlugin::getRunData(void) {
  using namespace std;
  using namespace dqmcpp;
  vector<ECAL::RunChannelData> rundata;
  writers::ProgressBar progress;
  for (auto& run : runListReader->runs()) {
    vector<ECAL::ChannelData> data;
    data.reserve(ECAL::NTotalChannels);
    progress.setMaxValue(urls(run).size() * runListReader->runs().size());
    progress.setLabel(to_string(run.runnumber));
    const auto contents = net::URLCache::get(urls(run));
    for (auto& c : contents) {
      auto data_tt = readers::JSONReader::parse(c);
      data.insert(data.end(), data_tt.begin(), data_tt.end());
      progress.increment();
    }
    ECAL::RunChannelData rd(run, data);
    rundata.push_back(rd);
  }
  return rundata;
}

void ChannelPlugin::Process() {
  // nothing here
}

}  // namespace plugins
}  // namespace dqmcpp