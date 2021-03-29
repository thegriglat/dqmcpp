#include "TPOccupancyL1.hh"

#include <algorithm>
#include <iostream>
#include <map>
#include <set>
#include <string>
#include <vector>
#include "ECAL/ECAL.hh"
#include "colors/Colors.hh"
#include "common/common.hh"
#include "common/gnuplot.hh"
#include "net/DQMURLProvider.hh"
#include "readers/JSONReader.hh"
#include "writers/Gnuplot2DWriter.hh"

#include "writers/ProgressBar.hh"

using namespace std;
using namespace dqmcpp;

REGISTER_PLUGIN(TPOccupancyL1)

namespace {

const string geturl(const ECAL::Run& run) {
  return net::DQMURL::dqmurl(run, "L1T/L1TStage2CaloLayer1/ecalOccupancy");
}

double getAvgY(const plugins::TPOccupancyL1::RunL1Data& rd, const int x) {
  const auto ally = common::filter(rd.data, [x](const ECAL::Data2D& d2d) {
    return common::equal(d2d.x, static_cast<double>(x));
  });
  const auto avgy =
      common::median(ally, [](const ECAL::Data2D& d2d) { return d2d.value; });
  return avgy;
}

std::pair<int, int> getXY(const std::string& s) {
  const auto list = common::split(s, ",");
  const auto x = list.at(0).substr(1);
  const auto y = list.at(1).substr(0, list.at(1).size() - 2);
  return std::pair<int, int>(atoi(x.c_str()), atoi(y.c_str()));
}

bool sortYlabel(const std::string& a, const std::string& b) {
  const auto ap = getXY(a);
  const auto bp = getXY(b);
  // first == ieta
  // second == iphi
  if (ap.first == bp.first) {
    return ap.second < bp.second;
  }
  return ap.first < bp.first;
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
    const auto content =
        readers::JSONReader::parse2D(readers::JSONReader::get(url));
    rundata.emplace_back(run, content);
    progress.increment();
  }
  return rundata;
}

void TPOccupancyL1::Process() {
  // plot
  auto rundata = getRunData();
  for (auto& rd : rundata) {
    for (int x = -28; x <= 28; ++x) {
      if (x == 0)
        continue;
      // get avg
      const double avgy = getAvgY(rd, x);
      // scale to avgy
      for (auto& d : rd.data) {
        if (common::equal(d.x, static_cast<double>(x))) {
          d.value /= avgy;
        }
      }
    }
    // now we have scaled rundata
  }

  // determine median against all runs and get list of bad elements
  set<pair<int, int>> badxy;
  writers::ProgressBar* progress = new writers::ProgressBar(28 * 2);
  progress->setLabel("x iterations");
  for (int x = -28; x <= 28; ++x) {
    if (x == 0)
      continue;
    for (int y = 1; y <= 72; ++y) {
      vector<double> runsvalues;
      runsvalues.reserve(rundata.size());
      for (auto& rd : rundata) {
        auto it = std::find_if(
            rd.data.begin(), rd.data.end(), [x, y](const ECAL::Data2D& d2d) {
              return common::equal(d2d.x, static_cast<double>(x)) &&
                     common::equal(d2d.y, static_cast<double>(y));
            });
        if (it != rd.data.end()) {
          runsvalues.push_back(it->value);
        }
      }
      // get median
      const double median = common::median(runsvalues);
      const double upperlimit = median * 2;
      const double lowerlimit = median / 2;
      for (auto& rd : rundata) {
        for (auto& d : rd.data) {
          if (!common::equal(d.x, static_cast<double>(x)) ||
              !common::equal(d.y, static_cast<double>(y)))
            continue;
          if (d.value > upperlimit || d.value < lowerlimit)
            badxy.insert({x, y});
        }
      }
    }
    progress->increment();
  }
  delete progress;
  progress = new writers::ProgressBar(rundata.size());
  // now we have list of bad [x,y]
  writers::Gnuplot2DWriter::Data2D data;
  double _maxvalue = -1;
  for (auto& rd : rundata) {
    progress->setLabel("writing " + to_string(rd.run.runnumber));
    for (auto& p : badxy) {
      const auto x = p.first;
      const auto y = p.second;
      auto it = std::find_if(
          rd.data.begin(), rd.data.end(), [x, y](const ECAL::Data2D& d2d) {
            return common::equal(d2d.x, static_cast<double>(x)) &&
                   common::equal(d2d.y, static_cast<double>(y));
          });
      if (it != rd.data.end()) {
        const string xlabel = to_string(rd.run.runnumber);
        const string ylabel = common::string_format("[%d,%d]", x, y);
        const auto value = it->value;
        _maxvalue = std::max(value, _maxvalue);
        data.insert({{xlabel, ylabel}, value});
      }
    }
    progress->increment();
  }
  delete progress;
  writers::Gnuplot2DWriter writer(data);
  writer.setOutput("TPOccupancyL1.png");
  writer.setTitle("TPOccupancyL1");
  writer.setZ(0, _maxvalue);
  writer.setPalette({{0.0, colors::ColorSets::black},
                     {0.0, colors::ColorSets::blue},
                     {0.5 / _maxvalue, colors::ColorSets::white},
                     {2.0 / _maxvalue, colors::ColorSets::white},
                     {2.0 / _maxvalue, colors::ColorSets::yellow},
                     {4.0 / _maxvalue, colors::ColorSets::orange},
                     {1.0, colors::ColorSets::red}});
  writer.setSortYFn(sortYlabel);
  ofstream out("TPOccupancyL1.plt");
  out << writer;
  out.close();
  return;
}

}  // namespace plugins
}  // namespace dqmcpp