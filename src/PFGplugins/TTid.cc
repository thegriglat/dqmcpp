#include "TTid.hh"

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

REGISTER_PLUGIN(TTid)

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
                 "EcalBarrel/EBIntegrityTask/TTId/EBIT TTId EB%+03d", i)));
  }
  return s;
}

void plot(const vector<ECAL::RunTTData>& rundata) {
  writers::Gnuplot2DWriter::Data2D data;
  double _max = -1;
  for (auto& rd : rundata) {
    const string xlabel = to_string(rd.run.runnumber);
    for (auto& d : rd.data) {
      const string det = ECALChannels::detByTTTTC(d.tt, d.tcc);
      const string ylabel =
          common::string_format("%s TT%02d", det.c_str(), d.tt);
      data.insert({{xlabel, ylabel}, d.value});
      _max = std::max(_max, d.value);
    }
  }
  writers::Gnuplot2DWriter writer(data);
  writer.setOutput("TTid.png");
  writer.setPalette(colors::PaletteSets::Heatmap);
  if (_max > 0)
    writer.setZ(0, _max);
  writer.setZTick(10);
  writer.setTitle("TTId");
  writer.setLogscale("cb");
  ofstream out("TTid.plt");
  out << writer;
  out.close();
}

}  // namespace

void dqmcpp::plugins::TTid::Process() {
  writers::ProgressBar progress(runListReader->runs().size());
  std::vector<ECAL::RunTTData> rundata;
  for (auto& run : runListReader->runs()) {
    progress.setLabel(to_string(run.runnumber));
    progress.increment();
    const auto urls = get_urls(run);
    const auto contents = net::URLCache::get(urls);
    ECAL::RunTTData ttdata(run, {});
    for (auto& content : contents) {
      const auto ttd = ECAL::channel2TT(readers::JSONReader::parse(content));
      ttdata.data.insert(ttdata.data.end(), ttd.begin(), ttd.end());
    }
    rundata.push_back(ttdata);
  }
  plot(rundata);
}