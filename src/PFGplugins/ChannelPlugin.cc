#include "ChannelPlugin.hh"
/**
 * @file RMSChannels.cc
 * @author Grigory Latyshev (thegriglat@gmail.com)
 * @brief Heatmap with noisy RMS channels
 */
// #include <cmath>
#include <fstream>
#include <functional>
#include <map>
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
  for (auto& e : rundata) {
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
  ECAL::Run lastrun(rundata.back().run);
  vector<pair<ECAL::Channel, int>> analyzed_channels;
  for (auto& rd : rundata) {
    const auto runstr = std::to_string(rd.run.runnumber);
    for (auto& chd : rd.data) {
      auto channel_info = ECALChannels::find(chd.channel);
      auto it = std::find_if(analyzed_channels.begin(), analyzed_channels.end(),
                             [&chd](const pair<ECAL::Channel, int>& pair) {
                               return pair.first == chd.channel;
                             });
      int channel_status;
      if (it == analyzed_channels.end()) {
        channel_status =
            plugins::ChannelStatus::getChannelStatus(lastrun, chd.channel);
        analyzed_channels.push_back({chd.channel, channel_status});
      } else {
        channel_status = it->second;
      }
      string channel_status_str = "";
      if (channel_status != 0)
        channel_status_str = "^{" + to_string(channel_status) + "}";
      const std::string channelstr =
          channel_info->det() + " TT" + std::to_string(channel_info->tower) +
          " [" + std::to_string(chd.channel.ix_iphi) + "," +
          std::to_string(chd.channel.iy_ieta) + "]" + channel_status_str;
      data.insert({{runstr, channelstr}, chd.value});
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