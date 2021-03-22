#include "TPOccupancyL1.hh"

#include <algorithm>
#include <iostream>
#include <map>
#include <string>
#include <vector>
#include "../ECAL/ECAL.hh"
#include "../common/common.hh"
#include "../common/gnuplot.hh"
#include "../net/DQMURLProvider.hh"
#include "../writers/Gnuplot1DWriter.hh"
#include "../writers/ProgressBar.hh"

using namespace std;
using namespace dqmcpp;

namespace {

const string geturl(const ECAL::Run& run) {
  return net::DQMURL::dqmurl(run, "L1T/L1TStage2CaloLayer1/ecalOccupancy");
}

vector<std::pair<std::string, double>> getData(
    const vector<plugins::TPOccupancyL1::RunL1Data>& rundata,
    int x,
    int y) {
  vector<pair<string, double>> result;
  result.reserve(rundata.size());
  for (auto& rd : rundata) {
    const auto tt1 = find_if(
        rd.data.begin(), rd.data.end(),
        [x, y](const ECAL::Data2D& d2d) { return d2d.x == x && d2d.y == y; });
    if (tt1 != rd.data.end()) {
      const auto ally = common::filter(
          rd.data, [x](const ECAL::Data2D& d2d) { return d2d.x == x; });
      const auto avgy = common::median(
          ally, [](const ECAL::Data2D& d2d) { return d2d.value; });
      result.push_back({to_string(rd.run.runnumber), (tt1->value) / (avgy)});
    }
  }
  return result;
}

}  // namespace

namespace dqmcpp {
namespace plugins {

vector<TPOccupancyL1::RunL1Data> TPOccupancyL1::getRunData(void) {
  vector<RunL1Data> rundata;
  rundata.reserve(runListReader->runs().size());
  writers::ProgressBar progress(runListReader->runs().size());
  for (auto& run : runListReader->runs()) {
    progress.setLabel(to_string(run.runnumber));
    const auto url = geturl(run);
    const auto content = reader->parse2D(reader->get(url));
    rundata.emplace_back(run, content);
    progress.increment();
  }
  return rundata;
}

void TPOccupancyL1::Process() {
  // plot
  const auto rundata = getRunData();
  writers::ProgressBar progress((28 * 2));
  for (int x = -28; x <= 28; ++x) {
    if (x == 0)
      continue;
    progress.increment();
    for (int y = 1; y <= 72; ++y) {
      const auto data = getData(rundata, x, y);
      if (data.size() == 0)
        continue;
      writers::Gnuplot1DWriter writer(data);
      const string name = "TPOccupancyL1_" + to_string(x) + "_" + to_string(y);
      writer.setOutput(name + ".png");
      writer.setTitle(name);
      writer.setY(0, 6);
      writer.with("linespoints");
      ofstream out(name + ".plt");
      out << writer;
      out.close();
      //   common::gnuplot::exec(writer);
    }
  }
  return;
}

}  // namespace plugins
}  // namespace dqmcpp