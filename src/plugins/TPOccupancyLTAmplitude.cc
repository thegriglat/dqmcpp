#include "TPOccupancyLTAmplitude.hh"

#include <algorithm>
#include <iostream>
#include <map>
#include <set>
#include <string>
#include <vector>
#include "../ECAL/ECAL.hh"
#include "../colors/Colors.hh"
#include "../common/common.hh"
#include "../common/gnuplot.hh"
#include "../net/DQMURLProvider.hh"
#include "../writers/Gnuplot2DWriter.hh"
#include "../writers/ProgressBar.hh"

#define MEDIANUP (1.15)
#define MEDIANLOW (0.85)

using namespace std;
using namespace dqmcpp;

namespace {

const string geturl(const ECAL::Run& run, const int sm) {
  return net::DQMURL::dqmurl(
      run, common::string_format(
               "EcalBarrel/EBLaserTask/Laser3/EBLT amplitude EB%+03d L3", sm));
}

bool isIBlock(const ECAL::Channel& c, const int sm) {
  const bool a = std::abs(c.iy_ieta) > 5;
  const int l1 = std::abs(sm) * 20;
  const int l2 = l1 - 20;
  const int low = (sm > 0) ? l1 : l2;
  const bool b = std::abs(c.ix_iphi - low) < 10;
  const bool block = a && b;
  return block;
}

bool isLBlock(const ECAL::Channel& c, const int sm) {
  return !isIBlock(c, sm);
}

void scaleSM(std::vector<ECAL::ChannelData>& data, const int sm) {
  using ECAL::ChannelData;
  const double Imedian = common::median(
      common::filter(
          data,
          [sm](const ChannelData& d2d) { return isIBlock(d2d.channel, sm); }),
      [](const ChannelData& d2d) { return d2d.value; });
  const double Lmedian = common::median(
      common::filter(
          data,
          [sm](const ChannelData& d2d) { return isLBlock(d2d.channel, sm); }),
      [](const ChannelData& d2d) { return d2d.value; });
  for (auto it = data.begin(); it != data.end(); ++it) {
    const auto median = (isIBlock(it->channel, sm)) ? Imedian : Lmedian;
    if (median != 0)
      it->value /= median;
  }
};

std::vector<pair<int, int>> getBadXY(
    const vector<plugins::TPOccupancyLTAmplitude::RunL1Data>& rundata,
    const int sm) {
  using ECAL::ChannelData;
  vector<pair<int, int>> badxy;
  for (int x = std::abs(sm) * 20 - 20; x <= std::abs(sm) * 20; ++x) {
    for (int y = 1 * common::sign(sm); std::abs(y) <= 85;
         y += common::sign(sm)) {
      vector<double> values;
      values.reserve(rundata.size());
      for (auto& rd : rundata) {
        auto it = std::find_if(
            rd.data.begin(), rd.data.end(), [x, y](const ChannelData& cd) {
              return cd.channel.ix_iphi == x && cd.channel.iy_ieta == y;
            });
        if (it != rd.data.end()) {
          values.push_back(it->value);
        }
      }
      const auto median_over_runs = common::median(values);
      const auto median_upper = median_over_runs * MEDIANUP;
      const auto median_lower = median_over_runs * MEDIANLOW;
      for (auto& v : values) {
        if (v > median_upper || v < median_lower) {
          badxy.push_back({x, y});
          break;
        }
      }
    }
  }
  return badxy;
}

vector<int> getSM() {
  vector<int> sms;
  sms.reserve(18 * 2);
  for (int i = -18; i <= 18; ++i)
    if (i != 0)
      sms.push_back(i);
  return sms;
}

}  // namespace

namespace dqmcpp {
namespace plugins {

vector<TPOccupancyLTAmplitude::RunL1Data> TPOccupancyLTAmplitude::getRunData(
    const int sm) {
  vector<RunL1Data> rundata;
  rundata.reserve(runListReader->runs().size());
  for (auto& run : runListReader->runs()) {
    const auto url = geturl(run, sm);
    const auto content = reader->parse(reader->get(url));
    rundata.emplace_back(run, content);
  }
  return rundata;
}

void TPOccupancyLTAmplitude::Process() {
  // plot
  writers::ProgressBar progress(18 * 2);
  vector<TPOccupancyLTAmplitude::RunL1Data> allrundata;
  writers::Gnuplot2DWriter::Data2D gdata;
  double _maxvalue = -1;
  for (auto sm : getSM()) {
    if (sm == 0)
      continue;
    progress.setLabel(common::string_format("EB%+03d", sm));
    auto rundata = getRunData(sm);
    for (auto& rd : rundata) {
      scaleSM(rd.data, sm);
    }
    // get bad xy over all runs
    auto badxy = getBadXY(rundata, sm);
    for (auto& rd : rundata) {
      const auto badxyfiltered =
          common::filter(rd.data, [&badxy](const ECAL::ChannelData& cd) {
            const pair<int, int> xy = {cd.channel.ix_iphi, cd.channel.iy_ieta};
            return std::find(badxy.begin(), badxy.end(), xy) != badxy.end();
          });
      for (auto& c : badxyfiltered) {
        const string xlabel = to_string(rd.run.runnumber);
        const string ylabel = common::string_format(
            "EB%+03d [%d:%d]", sm, c.channel.ix_iphi, c.channel.iy_ieta);
        const auto value = c.value;
        _maxvalue = std::max(_maxvalue, value);
        gdata.insert({{xlabel, ylabel}, value});
      }
    }
    progress.increment();
  }

  // gnuplot

  writers::Gnuplot2DWriter writer(gdata);
  writer.setOutput("TPOccupancyLTAmplitude.png");
  writer.setTitle("TPOccupancyLTAmplitude");
  writer.setZ(0, _maxvalue);
  writer.setZTick(0.1);
  writer.setPalette({{0.0, colors::ColorSets::black},
                     {0.0, colors::ColorSets::blue},
                     {MEDIANLOW / _maxvalue, colors::ColorSets::white},
                     {MEDIANUP / _maxvalue, colors::ColorSets::white},
                     {MEDIANUP / _maxvalue, colors::ColorSets::yellow},
                     {1.0, colors::ColorSets::red}});
  ofstream out("TPOccupancyLTAmplitude.plt");
  out << writer;
  out.close();
  return;
}

}  // namespace plugins
}  // namespace dqmcpp