#include "RecHitOccupancyDiff.hh"
#include "net/DQMURLProvider.hh"
#include "readers/JSONReader.hh"
#include "writers/Gnuplot1DWriter.hh"
#include "writers/ProgressBar.hh"

#include <algorithm>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include "common/common.hh"

REGISTER_PLUGIN(RecHitOccupancyDiff)

#define LIMIT (400.0)

using namespace std;
using namespace dqmcpp;

namespace {

using namespace std;
using namespace dqmcpp;

string eburl(const ECAL::Run& run) {
  return net::DQMURL::dqmurl(run,
                             "EcalBarrel/EBOccupancyTask/EBOT rec hit thr "
                             "occupancy z+(far) - z-(near)");
}

string eeurl(const ECAL::Run& run) {
  return net::DQMURL::dqmurl(run,
                             "EcalEndcap/EEOccupancyTask/EEOT rec hit thr "
                             "occupancy z+(far) - z-(near)");
}

string url(const ECAL::Run& run, const int eb) {
  if (eb == 0)
    return eburl(run);
  return eeurl(run);
}

bool isRemoved(const ECAL::Data1D& d1d) {
  return std::abs(d1d.base.x) < LIMIT;
}

void eraseMain(vector<ECAL::Data1D>& list) {
  list.erase(std::remove_if(list.begin(), list.end(), isRemoved), list.end());
}

void plot(const vector<ECAL::RunData1D>& rundata, const std::string& name) {
  writers::Gnuplot1DWriter::Data1D data;
  for (auto& rd : rundata) {
    const auto xlabel = to_string(rd.run.runnumber);
    const double value =
        common::sum(rd.data, [](const ECAL::Data1D& d1d) { return d1d.value; });
    data.emplace_back(xlabel, value);
  }
  writers::Gnuplot1DWriter writer(data);
  writer.setOutput(name + ".png");
  writer.setTitle(name);
  writer.with("linespoints");
  ofstream out(name + ".plt");
  out << writer;
  out.close();
}

}  // namespace

void dqmcpp::plugins::RecHitOccupancyDiff::Process() {
  const auto runs = runListReader->runs();
  writers::ProgressBar pb(runs.size() * 2);
  // eb is 0, 1 is ee
  for (int i = 0; i < 2; ++i) {
    vector<ECAL::RunData1D> rundata;
    const string det = (i == 0) ? "EB" : "EE";
    for (auto& run : runs) {
      pb.setLabel(det + " " + to_string(run.runnumber));
      auto content =
          readers::JSONReader::parse1D(readers::JSONReader::get(url(run, i)));
      eraseMain(content);
      rundata.emplace_back(run, content);
      pb.increment();
    }
    plot(rundata, "RecHitOccupancyDiff-" + det);
  }
}
