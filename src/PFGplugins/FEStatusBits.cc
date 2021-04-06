#include "FEStatusBits.hh"

#include <cmath>
#include <fstream>
#include <iostream>
#include <set>
#include <string>
#include <vector>
#include "ECAL/ECAL.hh"
#include "colors/Colors.hh"
#include "common/common.hh"
#include "net/DQMURLProvider.hh"
#include "readers/JSONReader.hh"
#include "writers/Gnuplot2DWriter.hh"
#include "writers/ProgressBar.hh"

using namespace std;
using namespace dqmcpp;

REGISTER_PLUGIN(FEStatusBits)

namespace {

struct URLType {
  bool isEB;
  int sm;
  std::string url;
  URLType(bool isEB, int sm, std::string url) : isEB(isEB), sm(sm), url(url){};
};

struct TTInfo {
  int tt;
  int sm;
  int status;
  bool isEB;

  double value;
  TTInfo(int tt, int sm, int status, bool isEB, double value)
      : tt(tt), sm(sm), status(status), isEB(isEB), value(value){};
};

using RunTTInfo = dqmcpp::ECAL::RunData<std::vector<TTInfo>>;

vector<URLType> urls(const int runnumber, const std::string& dataset) {
  std::vector<URLType> urls;
  char* plot = new char[512];
  for (int i = -18; i < 19; ++i) {
    if (i == 0)
      continue;
    sprintf(plot,
            "EcalBarrel/EBStatusFlagsTask/FEStatus/EBSFT front-end status bits "
            "EB%+03d",
            i);
    urls.push_back(
        URLType(true, i, net::DQMURL::dqmurl(runnumber, dataset, plot)));
  }
  for (int i = -9; i < 10; ++i) {
    if (i == 0)
      continue;
    sprintf(plot,
            "EcalEndcap/EEStatusFlagsTask/FEStatus/EESFT front-end status bits "
            "EE%+03d",
            i);
    urls.push_back(
        URLType(false, i, net::DQMURL::dqmurl(runnumber, dataset, plot)));
  }
  delete[] plot;
  return urls;
}

void rescaleToOne(vector<TTInfo>& data) {
  // get uniq list of filled bins;
  set<int> xbins;
  for (auto& e : data) {
    xbins.insert(e.tt);
  }
  for (auto& xbin : xbins) {
    // get filtered data
    auto filtered = common::filter(
        data, [xbin](const TTInfo& elem) { return elem.tt == xbin; });
    auto sum = common::sum(filtered, [](const TTInfo& e) { return e.value; });
    for (auto& elem : data) {
      if (elem.tt != xbin)
        continue;
      elem.value /= sum;
    }
  }
}

void plot(const std::vector<RunTTInfo>& rundata,
          const std::vector<int>& statuses) {
  writers::Gnuplot2DWriter::Data2D data;
  // get runlist
  vector<int> runs;
  for (auto& rd : rundata)
    runs.push_back(rd.run.runnumber);
  std::sort(runs.begin(), runs.end());
  vector<int> filled_runs;
  for (auto& run : runs) {
    auto rd = std::find_if(
        rundata.begin(), rundata.end(),
        [run](const RunTTInfo& rd0) { return rd0.run.runnumber == run; });
    const string runlabel = to_string(run);
    // filter tt with given statuses
    auto filtered_tt =
        common::filter(rd->data, [&statuses](const TTInfo& ttinfo) {
          return (common::has(statuses, ttinfo.status));
        });
    for (auto& ttdata : filtered_tt) {
      std::string ttlabel = (ttdata.isEB) ? "EB" : "EE";
      ttlabel += to_string(ttdata.sm) + ":TT" + to_string(ttdata.tt);
      // if data has this tt -- +status
      // else insert
      std::pair<string, string> data_key = {runlabel, ttlabel};
      auto it = data.find(data_key);
      if (it == data.end()) {
        // not found
        data.insert({data_key, ttdata.value});
      } else {
        it->second += ttdata.value;
      }
    }
    if (filtered_tt.size() != 0)
      filled_runs.push_back(run);
  }
  // we have to fill some dummy data for runs without given statuses
  for (auto& run : runs) {
    if (common::has(filled_runs, run))
      continue;
    auto some = data.begin();
    if (some == data.end()) {
      cout << "no data ..." << endl;
      continue;
    }
    const auto ttlabel = some->first.second;
    data.insert({{to_string(run), ttlabel}, 0.0});
  }
  writers::Gnuplot2DWriter writer(data);
  vector<string> status_list;
  for (auto& i : statuses)
    status_list.push_back(dqmcpp::plugins::FEStatusBits::STATUSES.at(i));
  const string title = "ECAL FEStatusBits " + common::join(status_list, "+");
  const string filename =
      "FEStatusBits_" + common::join(status_list, "_") + ".plt";
  ofstream out(filename);
  writer.setZ(0, 1);
  writer.setPalette(colors::PaletteSets::Rainbow);
  writer.setTitle(title);
  writer.setOutput(filename + ".png");
  writer.setZTick(0.1);
  out << writer << endl;
  out.close();
}

}  // namespace

namespace dqmcpp {
namespace plugins {

const std::vector<std::string> FEStatusBits::STATUSES = {
    "ENABLED",           "DISABLED",  "TIMEOUT",   "HEADERERROR",
    "CHANNELID",         "LINKERROR", "BLOCKSIZE", "SUPPRESSED",
    "FIFOFULL",          "L1ADESYNC", "BXDESYNC",  "L1ABXDESYNC",
    "FIFOFULLL1ADESYNC", "HPARITY",   "VPARITY",   "FORCEDZS"};

void FEStatusBits::Process() {
  writers::ProgressBar progress(runListReader->runs().size());
  vector<RunTTInfo> rundata;
  for (auto& run : runListReader->runs()) {
    progress.setLabel(to_string(run.runnumber));
    vector<TTInfo> data;
    for (auto& url : urls(run.runnumber, run.dataset)) {
      //   cout << url.url << endl;
      auto data_json =
          readers::JSONReader::parse2D(readers::JSONReader::get(url.url));
      vector<TTInfo> data_tt;
      data_tt.reserve(data_json.size());
      for (auto& e : data_json)
        if (common::isNotZero(e.value)) {
          TTInfo tti = TTInfo(std::floor(e.base.x), url.sm,
                              std::floor(e.base.y), url.isEB, e.value);
          data_tt.push_back(tti);
        }
      rescaleToOne(data_tt);
      for (auto& e : data_tt)
        data.push_back(e);
    }
    const RunTTInfo rd(run, data);
    rundata.push_back(rd);
    progress.increment();
  }

  // get uniq statuses

  std::set<std::vector<int>> uniq_statuses;
  // summary plot
  uniq_statuses.insert({common::index(STATUSES, std::string("LINKERROR")),
                        common::index(STATUSES, std::string("HEADERERROR")),
                        common::index(STATUSES, std::string("TIMEOUT"))});
  for (auto& rd : rundata) {
    for (auto& tti : rd.data)
      uniq_statuses.insert({tti.status});
  }
  for (auto& statuslist : uniq_statuses) {
    // skip ENABLED
    if (common::has(statuslist, common::index(STATUSES, string("ENABLED"))))
      continue;
    if (common::has(statuslist, common::index(STATUSES, string("SUPPRESSED"))))
      continue;
    plot(rundata, statuslist);
  }
}

}  // namespace plugins
}  // namespace dqmcpp