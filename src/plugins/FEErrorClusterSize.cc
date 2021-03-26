#include "FEErrorClusterSize.hh"

#include <algorithm>
#include <array>
#include <cmath>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include "../ECAL/ECAL.hh"
#include "../colors/Colors.hh"
#include "../common/clusters.hh"
#include "../common/common.hh"
#include "../net/DQMURLProvider.hh"
#include "../writers/Gnuplot2DWriter.hh"
#include "../writers/ProgressBar.hh"

#define SQR(X) ((X) * (X))

using namespace std;
using namespace dqmcpp;

namespace {

struct URLInfo {
  int iz;
  std::string url;
};

vector<URLInfo> urls(const int run, const std::string& dataset) {
  // EB
  // EcalBarrel/EBSummaryClient/EBSFT front-end status summary
  // EE
  // EcalEndcap/EESummaryClient/EESFT EE - front-end status summary
  const vector<URLInfo> urls = {
      {0, net::DQMURL::dqmurl(
              run, dataset,
              "EcalBarrel/EBSummaryClient/EBSFT front-end status summary")},
      {-1,
       net::DQMURL::dqmurl(
           run, dataset,
           "EcalEndcap/EESummaryClient/EESFT EE - front-end status summary")},
      {1,
       net::DQMURL::dqmurl(
           run, dataset,
           "EcalEndcap/EESummaryClient/EESFT EE + front-end status summary")}};
  return urls;
}

struct Point {
  double x;
  double y;
  Point(double x, double y) : x(x), y(y){};
  friend bool inline operator==(const Point& a, const Point& b) {
    return common::equal(a.x, b.x) && common::equal(a.y, b.y);
  }
};

struct PluginData {
  int iz;
  int clusterSize;
  int count = 0;
  PluginData(int iz, int clusterSize, int count = 0)
      : iz(iz), clusterSize(clusterSize), count(count){};
};

using RunFEData = ECAL::RunData<std::vector<PluginData>>;

void plot(const std::vector<RunFEData>& rundata) {
  writers::Gnuplot2DWriter::Data2D data;
  int maxcount = -1;
  for (auto& e : rundata) {
    const string xlabel = to_string(e.run.runnumber);
    for (auto& d : e.data) {
      const string ylabel = to_string(d.clusterSize);
      const auto value = d.count;
      if (value > maxcount)
        maxcount = value;
      data.insert({{xlabel, ylabel}, value});
    }
  }
  // fill empty runs
  for (auto& rd : rundata) {
    if (rd.data.size() != 0)
      continue;
    const auto some = data.begin();
    const string rs = to_string(rd.run.runnumber);
    if (some != data.end()) {
      const auto ylabel = some->first.second;
      data.insert({{rs, ylabel}, 0});
    }
  }
  writers::Gnuplot2DWriter writer(data);
  writer.setPalette(colors::PaletteSets::Heatmap);
  writer.setOutput("FEErrorClusterSize.png");
  writer.setTitle("FEErrorClusterSize");
  writer.setZ(0, maxcount);
  writer.setZTick(1);
  ofstream out("FEErrorClusterSize.plt");
  out << writer;
  out.close();
}

double Pdistance(const Point& a, const Point& b) {
  const auto dx = a.x - b.x;
  const auto dy = a.y - b.y;
  return std::sqrt(SQR(dx) + SQR(dy));
}

}  // namespace

namespace dqmcpp {
namespace plugins {

void FEErrorClusterSize::Process() {
  writers::ProgressBar progress(runListReader->runs().size());
  vector<RunFEData> rundata;
  for (auto& run : runListReader->runs()) {
    vector<PluginData> rdata;
    const auto& runnumber = run.runnumber;
    const auto& dataset = run.dataset;
    progress.setLabel(to_string(runnumber));
    const auto urllist = urls(runnumber, dataset);
    for (auto& url : urllist) {
      const auto iz = url.iz;
      auto data2d = reader->parse2D(reader->get(url.url), false);
      auto it = std::remove_if(
          data2d.begin(), data2d.end(),
          [](const ECAL::Data2D& d2d) { return common::isNotZero(d2d.value); });
      data2d.erase(it, data2d.end());
      vector<Point> points;
      points.reserve(data2d.size());
      for (auto& d2d : data2d) {
        points.emplace_back(d2d.x, d2d.y);
      }
      const auto clusters = common::clusters(points, 6, Pdistance);
      for (auto& cluster : clusters) {
        const auto clustersize = static_cast<int>(cluster.size());
        if (clustersize == 0)
          continue;
        auto it = std::find_if(
            rdata.begin(), rdata.end(), [iz, clustersize](const PluginData& e) {
              return e.iz == iz && e.clusterSize == clustersize;
            });
        if (it == rdata.end()) {
          // new clustersize
          rdata.push_back(PluginData(iz, clustersize, 1));
        } else {
          it->count++;
        }
      }
    }
    rundata.push_back(RunFEData(run, rdata));
    progress.increment();
  }
  plot(rundata);
}

}  // namespace plugins
}  // namespace dqmcpp