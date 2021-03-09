#include "RMSChannels.hh"
/**
 * @file RMSChannels.cc
 * @author Grigory Latyshev (thegriglat@gmail.com)
 * @brief Heatmap with noisy RMS channels
 */
#include <cmath>
#include <fstream>
#include <map>
#include <string>
#include "../dataclasses/ecalchannels.hh"
#include "../readers/DQMURLProvider.hh"
#include "../writers/Gnuplot2DWriter.hh"

#define RMSMAX (5)

static const auto rmslimit = [](const double rms) { return rms > RMSMAX; };

std::vector<std::string> RMSPlugin::urls(const unsigned int runnumber,
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
std::vector<ECAL::RunData> RMSPlugin::analyze(
    const std::vector<ECAL::RunData>& rundata) {
  std::vector<ECAL::Channel> badchannels;
  badchannels.reserve(100);
  for (auto& e : rundata) {
    for (auto& channeldata : e.channeldata) {
      if (rmslimit(channeldata.value)) {
        if (std::find(badchannels.begin(), badchannels.end(),
                      channeldata.channel) == badchannels.end())
          badchannels.push_back(channeldata.channel);
      }
    }
  }
  std::vector<ECAL::RunData> rd;
  rd.reserve(rundata.size());
  for (auto& e : rundata) {
    std::vector<ECAL::ChannelData> bd;
    std::copy_if(e.channeldata.begin(), e.channeldata.end(),
                 std::back_inserter(bd),
                 [badchannels](const ECAL::ChannelData& ecd) {
                   return std::find(badchannels.begin(), badchannels.end(),
                                    ecd.channel) != badchannels.end();
                 });
    rd.push_back(ECAL::RunData(e.run, bd));
  }
  return rd;
}
void RMSPlugin::plot(const std::vector<ECAL::RunData>& rundata) {
  if (rundata.size() == 0)
    return;

  Gnuplot2DWriter::Data2D data;
  for (auto& rd : rundata) {
    const auto runstr = std::to_string(rd.run.runnumber);
    for (auto& chd : rd.channeldata) {
      auto channel_info = ECALChannels::find(chd.channel);
      const std::string channelstr =
          channel_info->det + " TT" + std::to_string(channel_info->tower) +
          " [" + std::to_string(chd.channel.ix_iphi) + "," +
          std::to_string(chd.channel.iy_ieta) + "]";
      data.insert({{runstr, channelstr}, chd.value});
    }
  }
  std::ofstream out("rms_channels.plt");
  out << Gnuplot2DWriter(data)
             .title("G12 RMS channels")
             .setPalette({{0, "white"},
                          {3, "#006400"},
                          {3, "#fff497"},
                          {100, "#ff0201"}})
             .setZ(0, 100)
             .output("rms_channels.png")
      << std::endl;
  out.close();
  return;
}

void RMSPlugin::Process() {
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