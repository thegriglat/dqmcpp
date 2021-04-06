#include "ReadoutFlagDropped.hh"

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

REGISTER_PLUGIN(ReadoutFlagDropped)

using namespace std;
using namespace dqmcpp;

namespace {

using namespace std;
using namespace dqmcpp;

string geturl(const ECAL::Run& run, const int iz) {
  if (iz == 0)
    return net::DQMURL::dqmurl(
        run,
        "EcalBarrel/EBSelectiveReadoutTask/EBSRT FR Flagged Dropped Readout");
  if (iz == -1)
    return net::DQMURL::dqmurl(run,
                               "EcalEndcap/EESelectiveReadoutTask/EESRT FR "
                               "Flagged Dropped Readout EE -");
  if (iz == 1)
    return net::DQMURL::dqmurl(run,
                               "EcalEndcap/EESelectiveReadoutTask/EESRT FR "
                               "Flagged Dropped Readout EE +");
  return "";
}

void plot(vector<ECAL::RunTTData>& rundata) {
  writers::Gnuplot2DWriter::Data2D data;
  vector<string> allruns;
  for (auto& rd : rundata) {
    string xlabel = to_string(rd.run.runnumber);
    allruns.push_back(xlabel);
    for (auto& ttd : rd.data) {
      const auto det = ECALChannels::det(ttd.base);
      const string ttccu = (ttd.base.iz == 0) ? "TT" : "CCU";
      const string ylabel = common::string_format("%s %s%02d", det.c_str(),
                                                  ttccu.c_str(), ttd.base.tt);
      data.insert({{xlabel, ylabel}, ttd.value});
    }
  }
  writers::Gnuplot2DWriter writer(data);
  writer.setXlabels(allruns);
  writer.setOutput("ReadoutFlagDropped.png");
  writer.setTitle("ReadoutFlagDropped");
  writer.setZ(0, 1);
  writer.setZTick(0.1);
  writer.setPalette(colors::PaletteSets::Rainbow);
  ofstream out("ReadoutFlagDropped.plt");
  out << writer;
  out.close();
}

}  // namespace

void dqmcpp::plugins::ReadoutFlagDropped::Process() {
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
          const auto& list = ECALChannels::list();
          auto it = std::find_if(
              list.begin(), list.end(),
              [x, y](const dqmcpp::ECALChannels::ChannelInfo& e) {
                if (std::abs(e.ix - x) <= 2.5 && std::abs(e.iy - y) <= 2.5)
                  return true;
                return false;
              });
          if (it != list.end()) {
            content.emplace_back(ECAL::TT(it->ccu, it->tcc, iz), e.value);
          }
        }
      }
      ttdata.insert(ttdata.end(), content.begin(), content.end());
    }
    rundata.emplace_back(run, ttdata);
    pb.increment();
  }
  pb.finish();
  plot(rundata);
}