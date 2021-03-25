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
#include "../common/common.hh"
#include "../net/DQMURLProvider.hh"
#include "../writers/Gnuplot2DWriter.hh"

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

vector<array<Point, 2>> get_pairs(const vector<ECAL::Data2D>& data2d_in) {
  vector<array<Point, 2>> points;
  vector<ECAL::Data2D> data2d;
  // remove all non-zero elements;
  // in DQM 0 in these histograms mean bad TT
  for (auto& e : data2d_in) {
    if (common::isZero(e.value))
      data2d.push_back(e);
  }
  const double maxdistance = 6;  // 5 for TT
  for (auto it = data2d.begin(); it != data2d.end(); ++it) {
    for (auto jit = it + 1; jit != data2d.end(); ++jit) {
      const auto dx = it->x - jit->x;
      const auto dy = it->y - jit->y;
      const auto distance = std::sqrt(SQR(dx) + SQR(dy));
      if (distance > maxdistance)
        continue;
      Point p1(it->x, it->y);
      Point p2(jit->x, jit->y);
      points.push_back({p1, p2});
    }
  }
  return points;
}

vector<vector<Point>> clusterize(const vector<array<Point, 2>>& pairs_in) {
  vector<vector<Point>> clusters;
  vector<array<Point, 2>> pairs(pairs_in);
  if (pairs.size() == 0)
    return clusters;
  do {
    vector<Point> current_cluster = {pairs.at(0).at(0), pairs.at(0).at(1)};
    for (auto it = pairs.begin() + 1; it != pairs.end(); ++it) {
      const auto& p1 = it->at(0);
      const auto& p2 = it->at(1);
      auto pos1 = std::find(current_cluster.begin(), current_cluster.end(), p1);
      auto pos2 = std::find(current_cluster.begin(), current_cluster.end(), p2);
      const bool match1 = pos1 != current_cluster.end();
      const bool match2 = pos2 != current_cluster.end();
      if (match1 || match2) {
        // some of point connected with current cluster
        if (!match1) {
          current_cluster.push_back(p1);
        }
        if (!match2)
          current_cluster.push_back(p2);
      }
    }
    clusters.push_back(current_cluster);
    // remove added points
    auto removeit =
        std::remove_if(pairs.begin(), pairs.end(),
                       [&current_cluster](const array<Point, 2>& pts) {
                         return common::has(current_cluster, pts.at(0)) ||
                                common::has(current_cluster, pts.at(1));
                       });
    pairs.erase(removeit, pairs.end());
  } while (pairs.size() > 0);
  return clusters;
}

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

}  // namespace

namespace dqmcpp {
namespace plugins {

void FEErrorClusterSize::Process() {
  vector<RunFEData> rundata;
  for (auto& run : runListReader->runs()) {
    vector<PluginData> rdata;
    const auto& runnumber = run.runnumber;
    const auto& dataset = run.dataset;
    const auto urllist = urls(runnumber, dataset);
    for (auto& url : urllist) {
      cout << url.url << endl;
      const auto iz = url.iz;
      const auto data2d = reader->parse2D(reader->get(url.url), false);
      const auto pairs = get_pairs(data2d);
      const auto clusters = clusterize(pairs);
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
  }
  plot(rundata);
}

}  // namespace plugins
}  // namespace dqmcpp