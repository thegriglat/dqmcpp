#include "ChannelStatus.hh"

#include <cmath>
#include <fstream>
#include <iostream>
#include <map>
#include <set>
#include <string>
#include "../ECAL/ECAL.hh"
#include "../colors/Colors.hh"
#include "../common/clusters.hh"
#include "../common/common.hh"
#include "../common/gnuplot.hh"
#include "../ecalchannels/ECALChannels.hh"
#include "../net/DQMURLProvider.hh"
#include "../readers/JSONReader.hh"
#include "../writers/Gnuplot2DWriter.hh"
#include "../writers/ProgressBar.hh"

using namespace std;
using namespace dqmcpp;

namespace {

string geturl(const ECAL::Run& run, const int iz) {
  string url;
  switch (iz) {
    case -1:
      url = "EcalEndcap/EEIntegrityClient/EEIT EE - channel status map";
      break;
    case 0:
      url = "EcalBarrel/EBIntegrityClient/EBIT channel status map";
      break;
    case 1:
      url = "EcalEndcap/EEIntegrityClient/EEIT EE + channel status map";
      break;
  }
  return net::DQMURL::dqmurl(run, url);
}  // namespace

int channelDistance(const dqmcpp::ECAL::ChannelData& a,
                    const dqmcpp::ECAL::ChannelData& b) {
  // for distance cut we don't need to have exact 2D distance
  // squared d. is enough
  const auto dx = a.channel.ix_iphi - b.channel.ix_iphi;
  const auto dy = a.channel.iy_ieta - b.channel.iy_ieta;
  return dx * dx + dy * dy;
}

int getChannelTT(const ECAL::ChannelData& cd) {
  auto cinfo = ECALChannels::find(cd.channel);
  if (!cinfo) {
    cerr << "Cannot find channel " << cd << endl;
    exit(1);
  }
  if (cd.channel.isEB())
    return cinfo->tower;
  else
    return cinfo->ccu;
}

void dump(dqmcpp::writers::Gnuplot2DWriter& writer,
          const std::string& filename) {
  ofstream out(filename);
  out << writer;
  out.close();
}

struct ChannelDataTT : public dqmcpp::ECAL::ChannelData {
  int tt;
  ChannelDataTT(const dqmcpp::ECAL::ChannelData& cd) : ChannelData(cd) {
    tt = getChannelTT(cd);
  }
};

}  // namespace

namespace dqmcpp {
namespace plugins {

std::vector<ECAL::ChannelData> ChannelStatus::getChannelStatus(
    const ECAL::Run& run,
    const int iz) const {
  const string url = geturl(run, iz);
  if (iz != 0) {
    const auto content =
        readers::JSONReader::parse(readers::JSONReader::get(url));
    return content;
  }
  // for EB we have to transform Data2D to ChannelData as DQM rotates
  // histogram
  std::vector<ECAL::ChannelData> cd;
  const auto content =
      readers::JSONReader::parse2D(readers::JSONReader::get(url));
  cd.reserve(content.size());
  for (auto& e : content) {
    cd.push_back(ECAL::Data2D2Channel(e));
  }
  return cd;
}

std::vector<ECAL::ChannelData> ChannelStatus::getChannelStatus(
    const ECAL::Run& run) const {
  std::vector<ECAL::ChannelData> cd;
  for (int iz = -1; iz <= 1; ++iz) {
    const auto _tmp = getChannelStatus(run, iz);
    cd.insert(cd.end(), _tmp.begin(), _tmp.end());
  }
  return cd;
}

int ChannelStatus::getChannelStatus(const ECAL::Run& run,
                                    const ECAL::Channel& channel) const {
  int iz = 0;
  if (channel.isEEM())
    iz = -1;
  if (channel.isEEP())
    iz = 1;
  const auto _d = getChannelStatus(run, iz);
  auto it = std::find_if(_d.begin(), _d.end(),
                         [&channel](const ECAL::ChannelData& cd) {
                           return cd.channel == channel;
                         });
  if (it == _d.end())
    return 0;
  return it->value;
}

void ChannelStatus::Process() {
  writers::ProgressBar progress(runListReader->runs().size());
  vector<pair<int, ChannelDataTT>> channeldata;
  vector<pair<int, ChannelDataTT>> ttdata;
  for (auto& run : runListReader->runs()) {
    progress.setLabel(to_string(run.runnumber));
    const auto rd = getChannelStatus(run);
    for (int iz = -1; iz <= 1; ++iz) {
      vector<ChannelDataTT> channelttdata;
      {
        const auto channelD =
            common::filter(rd, [iz](const ECAL::ChannelData& cd) {
              if (iz == 0 && cd.channel.isEB())
                return true;
              if (iz == 1 && cd.channel.isEEP())
                return true;
              if (iz == -1 && cd.channel.isEEM())
                return true;
              return false;
            });
        for (auto& c : channelD) {
          channelttdata.push_back(c);
        }
      }
      const auto clusters = common::clusters(channelttdata, 1, channelDistance);
      /*
       1. get separately TT and channels
       2. make history plot for TT and channels
      */
      for (auto& cluster : clusters) {
        set<int> ttset;
        for (auto c : cluster) {
          ttset.insert(c.tt);
        }
        for (auto tt : ttset) {
          // filter channels from cluster to determine full TT
          const auto channels_per_tt = common::filter(
              cluster, [tt](const ChannelDataTT& cd) { return tt == cd.tt; });
          if (channels_per_tt.size() == 25) {
            // full TT
            auto c = channels_per_tt.at(0);
            ttdata.push_back({run.runnumber, c});
          } else {
            // channels, not TT
            for (auto c : channels_per_tt) {
              channeldata.push_back({run.runnumber, c});
            }
          }
        }
      }
    }
    progress.increment();
  };

  // write channeldata
  {
    writers::Gnuplot2DWriter::Data2D data;
    for (auto& pair : channeldata) {
      const auto ch = pair.second;
      auto info = ECALChannels::find(ch.channel);
      string xlabel = to_string(pair.first);
      string det = (info) ? info->det() : "NONE?";
      string ylabel =
          common::string_format("%s [%+03d, %+03d]", det.c_str(),
                                ch.channel.ix_iphi, ch.channel.iy_ieta);
      data.insert({{xlabel, ylabel}, ch.value});
    }
    writers::Gnuplot2DWriter writer(data);
    writer.setZ(0, 14);
    writer.setPalette(colors::PaletteSets::Heatmap);
    writer.setZTick(1.0);
    writer.setOutput("ChannelStatusMap_channels.png");
    writer.setTitle("ChannelStatusMap: channels");
    writer.setPalette(colors::PaletteSets::Rainbow);
    dump(writer, "ChannelStatusMap_channels.plt");
  }

  // write ttdata
  {
    writers::Gnuplot2DWriter::Data2D data;
    for (auto& pair : ttdata) {
      const auto ch = pair.second;
      auto info = ECALChannels::find(ch.channel);
      string xlabel = to_string(pair.first);
      string det = (info) ? info->det() : "NONE?";
      auto tt = getChannelTT(ch);
      const string ttccu = (ch.channel.isEB()) ? "TT" : "CCU";
      string ylabel =
          common::string_format("%s %s %02d", det.c_str(), ttccu.c_str(), tt);
      data.insert({{xlabel, ylabel}, ch.value});
    }
    writers::Gnuplot2DWriter writer(data);
    writer.setZ(0, 14);
    writer.setPalette(colors::PaletteSets::Heatmap);
    writer.setZTick(1.0);
    writer.setOutput("ChannelStatusMap_tt.png");
    writer.setTitle("ChannelStatusMap: TT");
    writer.setPalette(colors::PaletteSets::Rainbow);
    dump(writer, "ChannelStatusMap_tt.plt");
  }
}
}  // namespace plugins
}  // namespace dqmcpp