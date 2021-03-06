#include "Laser3Amplitude.hh"

#include <algorithm>
#include <iostream>
#include <map>
#include <set>
#include <string>
#include <vector>
#include "ECAL/ECAL.hh"
#include "ECAL/EELightMonitoringRegions.hh"
#include "colors/Colors.hh"
#include "common/common.hh"
#include "common/gnuplot.hh"
#include "net/DQMURLProvider.hh"
#include "net/URLCache.hh"
#include "readers/JSONReader.hh"
#include "writers/Gnuplot2DWriter.hh"
#include "writers/ProgressBar.hh"

#define MEDIANUP (1.15)
#define MEDIANLOW (0.85)
#define MEDIANUPEE (1.3)
#define MEDIANLOWEE (0.7)

using namespace std;
using namespace dqmcpp;

REGISTER_PLUGIN(Laser3Amplitude)

namespace {

const string geturl(const ECAL::Run& run, const int sm) {
  return net::DQMURL::dqmurl(
      run, common::string_format(
               "EcalBarrel/EBLaserTask/Laser3/EBLT amplitude EB%+03d L3", sm));
}

const string geturl_ee(const ECAL::Run& run, const int sm) {
  return net::DQMURL::dqmurl(
      run, common::string_format(
               "EcalEndcap/EELaserTask/Laser3/EELT amplitude EE%+03d L3", sm));
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
          [sm](const ChannelData& d2d) { return isIBlock(d2d.base, sm); }),
      [](const ChannelData& d2d) { return d2d.value; });
  const double Lmedian = common::median(
      common::filter(
          data,
          [sm](const ChannelData& d2d) { return isLBlock(d2d.base, sm); }),
      [](const ChannelData& d2d) { return d2d.value; });
  for (auto it = data.begin(); it != data.end(); ++it) {
    const auto median = (isIBlock(it->base, sm)) ? Imedian : Lmedian;
    if (common::isNotZero(median))
      it->value /= median;
  }
};

void scaleSM_EE(std::vector<ECAL::ChannelData>& data) {
  using ECAL::ChannelData;
  set<uint8_t> regions;
  for (auto& d : data)
    regions.insert(ECAL::EELightMR(d.base.ix_iphi, d.base.iy_ieta));
  std::map<uint8_t, double> region_median;
  for (const auto region : regions) {
    const auto median = common::median(
        common::filter(data,
                       [region](const ChannelData& cd) {
                         return region == ECAL::EELightMR(cd.base.ix_iphi,
                                                          cd.base.iy_ieta);
                       }),
        [](const ChannelData& cd) { return cd.value; });
    region_median.insert({region, median});
  }
  for (auto& d : data) {
    const auto region = ECAL::EELightMR(d.base.ix_iphi, d.base.iy_ieta);
    const auto median = region_median.at(region);
    if (common::isNotZero(median))
      d.value /= median;
  }
}

std::vector<pair<int, int>> getBadXY_EE(
    const vector<plugins::Laser3Amplitude::RunL1Data>& rundata) {
  set<pair<int, int>> allxy;
  vector<pair<int, int>> badxy;
  for (auto& rd : rundata)
    for (auto& d : rd.data)
      allxy.insert({d.base.ix_iphi, d.base.iy_ieta});

  for (auto& xy : allxy) {
    const auto x = xy.first;
    const auto y = xy.second;
    vector<double> values;
    values.reserve(rundata.size());

    for (auto& rd : rundata)
      for (auto& d : rd.data)
        if (d.base.ix_iphi == x && d.base.iy_ieta == y)
          values.push_back(d.value);

    const auto median_over_runs = common::median(values);
    const auto median_upper = median_over_runs * MEDIANUPEE;
    const auto median_lower = median_over_runs * MEDIANLOWEE;
    if (std::any_of(values.begin(), values.end(),
                    [median_upper, median_lower](const double v) {
                      return v > median_upper || v < median_lower;
                    })) {
      badxy.push_back({x, y});
    }
  }
  return badxy;
}

std::vector<pair<int, int>> getBadXY(
    const vector<plugins::Laser3Amplitude::RunL1Data>& rundata,
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
              return cd.base.ix_iphi == x && cd.base.iy_ieta == y;
            });
        if (it != rd.data.end()) {
          values.push_back(it->value);
        }
      }
      const auto median_over_runs = common::median(values);
      const auto median_upper = median_over_runs * MEDIANUP;
      const auto median_lower = median_over_runs * MEDIANLOW;
      if (std::any_of(values.begin(), values.end(),
                      [median_upper, median_lower](const double v) {
                        return v > median_upper || v < median_lower;
                      })) {
        badxy.push_back({x, y});
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

vector<Laser3Amplitude::RunL1Data> Laser3Amplitude::getRunData(const int sm,
                                                               bool eb) {
  vector<RunL1Data> rundata;
  rundata.reserve(runListReader->runs().size());
  const auto getter = (eb) ? geturl : geturl_ee;
  const auto contents = net::URLCache::get(common::map<ECAL::Run, string>(
      runListReader->runs(),
      [getter, sm](const ECAL::Run& run) { return getter(run, sm); }));

  for (size_t i = 0; i < contents.size(); ++i) {
    const auto cont = readers::JSONReader::parse(contents.at(i));
    rundata.emplace_back(runListReader->runs().at(i), cont);
  }
  return rundata;
}

void Laser3Amplitude::Process() {
  writers::ProgressBar progress(18 * 2 + 9 * 2);
  writers::Gnuplot2DWriter::Data2D gdata;
  double _maxvalue = -1;
  // ECAL Barrel
  for (auto sm : getSM()) {
    if (sm == 0)
      continue;
    progress.setLabel(common::string_format("EB%+03d", sm));
    auto rundata = getRunData(sm, true);
    for (auto& rd : rundata) {
      scaleSM(rd.data, sm);
    }
    // get bad xy over all runs
    auto badxy = getBadXY(rundata, sm);
    for (auto& rd : rundata) {
      const auto badxyfiltered =
          common::filter(rd.data, [&badxy](const ECAL::ChannelData& cd) {
            const pair<int, int> xy = {cd.base.ix_iphi, cd.base.iy_ieta};
            return std::find(badxy.begin(), badxy.end(), xy) != badxy.end();
          });
      for (auto& c : badxyfiltered) {
        const string xlabel = to_string(rd.run.runnumber);
        const string ylabel = std::string(c.base);
        const auto value = c.value;
        _maxvalue = std::max(_maxvalue, value);
        gdata.insert({{xlabel, ylabel}, value});
      }
    }
    progress.increment();
  }
  writers::Gnuplot2DWriter writer(gdata);
  writer.setOutput("Laser3Amplitude_EB.png");
  writer.setTitle("Laser3Amplitude EB");
  writer.setZ(0, _maxvalue);
  writer.setZTick(0.1);
  writer.setPalette({{0.0, colors::ColorSets::black},
                     {0.0, colors::ColorSets::blue},
                     {MEDIANLOW / _maxvalue, colors::ColorSets::white},
                     {MEDIANUP / _maxvalue, colors::ColorSets::white},
                     {MEDIANUP / _maxvalue, colors::ColorSets::yellow},
                     {1.0, colors::ColorSets::red}});
  ofstream out("Laser3Amplitude_EB.plt");
  out << writer;
  out.close();
  // ECAL Endcap
  double _maxvalueee = -1;
  writers::Gnuplot2DWriter::Data2D gdataee;
  for (int sm = -9; sm <= 9; ++sm) {
    if (sm == 0)
      continue;
    progress.setLabel(common::string_format("EE%+03d", sm));
    auto rundata = getRunData(sm, false);
    for (auto& rd : rundata) {
      scaleSM_EE(rd.data);
    }
    // get bad xy over all runs
    auto badxy = getBadXY_EE(rundata);
    for (auto& rd : rundata) {
      const auto badxyfiltered =
          common::filter(rd.data, [&badxy](const ECAL::ChannelData& cd) {
            const pair<int, int> xy = {cd.base.ix_iphi, cd.base.iy_ieta};
            return std::find(badxy.begin(), badxy.end(), xy) != badxy.end();
          });
      for (auto& c : badxyfiltered) {
        const string xlabel = to_string(rd.run.runnumber);
        const string ylabel = std::string(c.base);
        const auto value = c.value;
        _maxvalueee = std::max(_maxvalueee, value);
        gdataee.insert({{xlabel, ylabel}, value});
      }
    }
    progress.increment();
  }

  writers::Gnuplot2DWriter writeree(gdataee);
  writeree.setOutput("Laser3Amplitude_EE.png");
  writeree.setTitle("Laser3Amplitude EE");
  writeree.setZ(0, _maxvalueee);
  writeree.setZTick(0.1);
  writeree.setPalette({{0.0, colors::ColorSets::black},
                       {0.0, colors::ColorSets::blue},
                       {MEDIANLOWEE / _maxvalueee, colors::ColorSets::white},
                       {MEDIANUPEE / _maxvalueee, colors::ColorSets::white},
                       {MEDIANUPEE / _maxvalueee, colors::ColorSets::yellow},
                       {1.0, colors::ColorSets::red}});
  // gnuplot

  ofstream outee("Laser3Amplitude_EE.plt");
  outee << writeree;
  outee.close();
  return;
}

}  // namespace plugins
}  // namespace dqmcpp