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
#include "ChannelStatus.hh"
#include "common/common.hh"
#include "ecalchannels/ECALChannels.hh"
#include "net/URLCache.hh"
#include "readers/JSONReader.hh"
#include "writers/Gnuplot2DWriter.hh"
#include "writers/ProgressBar.hh"

using namespace dqmcpp;
using namespace std;

#define MAXSTATUS4BOX (10)

namespace {

std::string getYlabel(const ECAL::Channel& channel, const int status) {
  auto channel_info = ECALChannels::find(channel);
  string s = channel_info->det() + " TT" + std::to_string(channel_info->tower) +
             " [" + std::to_string(channel.ix_iphi) + "," +
             std::to_string(channel.iy_ieta) + "]";
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
  vector<BadChannel> badchannels_list;
  badchannels_list.reserve(100);
  writers::ProgressBar progress(rundata.size());
  progress.setLabel("analyzing...");
  for (auto& e : rundata) {
    progress.increment();
    for (auto& channeldata : e.data) {
      if (checkfn(channeldata.value)) {
        auto it = std::find_if(badchannels_list.begin(), badchannels_list.end(),
                               [&channeldata](const BadChannel& bc) {
                                 return bc.first == channeldata.channel;
                               });
        if (it == badchannels_list.end()) {
          // channel not recorded
          badchannels_list.push_back({channeldata.channel, 1});
        } else {
          // increment number;
          it->second++;
        }
      }
    }
  }
  auto it = std::remove_if(badchannels_list.begin(), badchannels_list.end(),
                           [](const BadChannel& c) { return c.second == 1; });
  badchannels_list.erase(it, badchannels_list.end());

  vector<ECAL::Channel> badchannels = common::map<BadChannel, ECAL::Channel>(
      badchannels_list, [](const BadChannel& bc) { return bc.first; });

  std::vector<ECAL::RunChannelData> rd;
  rd.reserve(rundata.size());
  for (auto& e : rundata) {
    std::vector<ECAL::ChannelData> bd;
    std::copy_if(e.data.begin(), e.data.end(), std::back_inserter(bd),
                 [&badchannels](const ECAL::ChannelData& ecd) {
                   return std::find(badchannels.begin(), badchannels.end(),
                                    ecd.channel) != badchannels.end();
                 });
    rd.push_back(ECAL::RunChannelData(e.run, bd));
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
  auto& lastrun = rundata.back().run;
  writers::ProgressBar progress(rundata.size());
  progress.setLabel("plotting...");
  std::set<std::array<int, 4>> badchannels;
  for (auto& rd : rundata) {
    for (auto& chd : rd.data) {
      auto cc = chd.channel.asArray();
      std::array<int, 4> c = {
          cc[0], cc[1], cc[2],
          plugins::ChannelStatus::getChannelStatus(lastrun, chd.channel)};
      badchannels.insert(c);
    }
  }
  for (auto& rd : rundata) {
    progress.increment();
    const auto xlabel = std::to_string(rd.run.runnumber);
    for (auto& b : badchannels) {
      const ECAL::Channel c(b[0], b[1], b[2]);
      int channel_status = b[3];
      const auto ylabel = getYlabel(c, channel_status);
      if (channel_status > MAXSTATUS4BOX) {
        boxes.emplace_back(xlabel, ylabel);
      }
      int idx = common::index(rd.data, [&c](const ECAL::ChannelData& cd) {
        return cd.channel == c;
      });
      if (idx != -1) {
        data.insert({{xlabel, ylabel}, rd.data.at(idx).value});
      }
    }
  }
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
      for (auto& e : data_tt)
        data.push_back(e);
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