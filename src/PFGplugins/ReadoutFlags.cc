#include "ReadoutFlags.hh"

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

#define LOWLIMIT (0.05)

using namespace std;
using namespace dqmcpp;

namespace {

using namespace std;
using namespace dqmcpp;

}  // namespace

namespace dqmcpp {
namespace plugins {

bool ReadoutFlags::removeFn(const ECAL::TTData& ttdata) const {
  return ttdata.value < 0.05;
}

string ReadoutFlags::geturl(const ECAL::Run& run, const int iz) const {
  return net::DQMURL::dqmurl(run, common::string_format("%d", iz));
}

std::vector<ECAL::RunTTData> ReadoutFlags::getRundata() const {
  const auto runs = runListReader->runs();
  writers::ProgressBar pb(runs.size());
  vector<ECAL::RunTTData> rundata;
  rundata.reserve(runs.size());
  for (auto& run : runs) {
    pb.setLabel(to_string(run.runnumber));
    vector<ECAL::TTData> ttdata;
    for (int iz = -1; iz <= 1; ++iz) {
      vector<ECAL::TTData> content;
      if (iz == 0) {
        content = ECAL::channel2TT(ECAL::Data2D2ChannelData(
            readers::JSONReader::parse2D(
                readers::JSONReader::get(geturl(run, 0))),
            iz));
      } else {
        const auto d2dv = readers::JSONReader::parse2D(
            readers::JSONReader::get(geturl(run, iz)));
        for (auto& e : d2dv) {
          const int x = std::trunc(e.base.x);
          const int y = std::trunc(e.base.y);
          const auto list = ECALChannels::list();
          auto it = std::find_if(
              list->begin(), list->end(),
              [x, y](const dqmcpp::ECALChannels::ChannelInfo& e) {
                if (std::abs(e.ix - x) <= 2.5 && std::abs(e.iy - y) <= 2.5)
                  return true;
                return false;
              });
          if (it != list->end()) {
            content.emplace_back(ECAL::TT(it->tower, it->tcc, iz), e.value);
          }
        }
      }
      vector<ECAL::TTData> newcontent;
      for (auto& e : content) {
        if (!removeFn(e))
          newcontent.push_back(e);
      }
      ttdata.insert(ttdata.end(), newcontent.begin(), newcontent.end());
    }
    rundata.emplace_back(run, ttdata);
    pb.increment();
  }
  return rundata;
}

void ReadoutFlags::plot(const vector<ECAL::RunTTData>& rundata,
                        const string name) {
  writers::Gnuplot2DWriter::Data2D data;
  vector<string> allruns;
  for (auto& rd : rundata) {
    string xlabel = to_string(rd.run.runnumber);
    allruns.push_back(xlabel);
    for (auto& ttd : rd.data) {
      const auto det = ECALChannels::det(ttd.base);
      int ttccu = ttd.base.tt;
      string tts = "TT";
      if (ttd.base.iz != 0) {
        tts = "CCU";
      }
      const string ylabel =
          common::string_format("%s %s%02d", det.c_str(), tts.c_str(), ttccu);
      data.insert({{xlabel, ylabel}, ttd.value});
    }
  }
  writers::Gnuplot2DWriter writer(data);
  writer.setXlabels(allruns);
  writer.setOutput(name + ".png");
  writer.setTitle(name);
  writer.setZ(0, 1);
  writer.setZTick(0.1);
  auto palette = colors::PaletteSets::Heatmap;
  // palette.push_back({0.0, "white"});
  // palette.push_back({LOWLIMIT, "white"});
  writer.setPalette(palette);
  ofstream out(name + ".plt");
  out << writer;
  out.close();
}

void ReadoutFlags::Process() {
  const auto rundata = getRundata();
  plot(rundata, pluginName());
}

}  // namespace plugins
}  // namespace dqmcpp