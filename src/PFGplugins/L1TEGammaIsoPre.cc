/**
 * @file L1TEGammaIsoPre.cc
 * @author Grigory Latyshev (thegriglat@gmail.com)
 * @brief L1TEGammaIsoPrePlugin implementation
 */
#include "L1TEGammaIsoPre.hh"
#include <algorithm>
#include <fstream>
#include <functional>
#include <iostream>
#include <string>
#include <vector>
#include "ECAL/ECAL.hh"
#include "common/common.hh"
#include "net/DQMURLProvider.hh"
#include "readers/JSONReader.hh"
#include "writers/Gnuplot1DWriter.hh"

using namespace std;

REGISTER_PLUGIN(L1TEGammaIsoPrePlugin)

namespace {

using namespace dqmcpp;

using RunProb = dqmcpp::ECAL::RunData<double>;

string urls(const int run, const std::string& dataset) {
  const std::string path =
      "L1T/L1TObjects/L1TEGamma/timing/First_bunch/ptmin_10p0_gev/"
      "egamma_noniso_bx_ieta_firstbunch_ptmin10p0";
  return net::DQMURL::dqmurl(run, dataset, path);
}

vector<ECAL::Data2D> filter(
    const vector<ECAL::Data2D>& data,
    std::function<bool(const ECAL::Data2D&)> filter_fn) {
  vector<ECAL::Data2D> r;
  r.reserve(data.size());
  for (auto& e : data)
    if (filter_fn(e))
      r.push_back(e);
  return r;
}

double sum(const vector<ECAL::Data2D>& d) {
  return dqmcpp::common::sum(d, [](const ECAL::Data2D& e) { return e.value; });
}

vector<RunProb> calcProb(const vector<ECAL::RunData2D>& rundata) {
  vector<RunProb> result;
  result.reserve(rundata.size());
  for (auto& rd : rundata) {
    // -1 bin
    auto minus1values = filter(
        rd.data, [](const ECAL::Data2D& d) { return std::abs(d.x + 1) < 0.1; });
    // zero bin
    auto zerovalues = filter(
        rd.data, [](const ECAL::Data2D& d) { return std::abs(d.x) < 0.1; });
    auto minus1_i = sum(minus1values);
    auto zero_i = sum(zerovalues);
    auto prob = minus1_i / (minus1_i + zero_i);
    result.push_back(RunProb(rd.run, prob));
  }
  return result;
}

vector<ECAL::Data2D> filter0m1(const vector<ECAL::Data2D>& data) {
  return filter(data,
                [](const ECAL::Data2D& d) { return d.x > -1.5 && d.x < 0.5; });
}

vector<ECAL::Data2D> filter_det(const vector<ECAL::Data2D>& data, bool isEB) {
  auto filter_fn =
      (isEB) ? [](const ECAL::Data2D& d) { return std::abs(d.y) < 40; }
             : [](const ECAL::Data2D& d) { return std::abs(d.y) >= 40; };
  return filter(data, filter_fn);
}

}  // namespace
namespace dqmcpp {
namespace plugins {

void L1TEGammaIsoPrePlugin::Process() {
  vector<ECAL::RunData2D> rundata;
  vector<ECAL::RunData2D> eb_rundata;
  vector<ECAL::RunData2D> ee_rundata;

  rundata.reserve(runListReader->runs().size());
  for (auto& run : runListReader->runs()) {
    auto url = urls(run.runnumber, run.dataset);
    cout << run.dataset << " " << url << endl;
    auto content = readers::JSONReader::parse2D(readers::JSONReader::get(url));
    if (content.size() == 0)
      continue;
    content = filter0m1(content);
    rundata.push_back(ECAL::RunData2D(run, content));
    auto eb_content = filter_det(content, true);
    auto ee_content = filter_det(content, false);
    eb_rundata.push_back(ECAL::RunData2D(run, eb_content));
    ee_rundata.push_back(ECAL::RunData2D(run, ee_content));
  }
  vector<RunProb> runprob = calcProb(rundata);
  vector<RunProb> eb_runprob = calcProb(eb_rundata);
  vector<RunProb> ee_runprob = calcProb(ee_rundata);

  auto sorter = [](const RunProb& a, const RunProb& b) {
    return a.run.runnumber < b.run.runnumber;
  };
  std::sort(runprob.begin(), runprob.end(), sorter);
  std::sort(eb_runprob.begin(), eb_runprob.end(), sorter);
  std::sort(ee_runprob.begin(), ee_runprob.end(), sorter);
  writers::Gnuplot1DWriter::Data1D data;
  writers::Gnuplot1DWriter::Data1D eb_data;
  writers::Gnuplot1DWriter::Data1D ee_data;

  for (auto& rp : runprob)
    data.push_back({std::to_string(rp.run.runnumber), rp.data});
  for (auto& rp : eb_runprob)
    eb_data.push_back({std::to_string(rp.run.runnumber), rp.data});
  for (auto& rp : ee_runprob)
    ee_data.push_back({std::to_string(rp.run.runnumber), rp.data});

  ofstream out("L1TEGammaIsoPre_ECAL.plt");
  writers::Gnuplot1DWriter writer(data);
  writer.setTitle("L1TEGammaIsoPre ECAL");
  writer.setOutput("L1TEGammaIsoPre_ECAL.png");
  out << writer << std::endl;
  out.close();

  ofstream outeb("L1TEGammaIsoPre_EB.plt");
  writers::Gnuplot1DWriter writereb(eb_data);
  writereb.setTitle("L1TEGammaIsoPre EB");
  writereb.setOutput("L1TEGammaIsoPre_EB.png");
  outeb << writereb << std::endl;
  outeb.close();

  ofstream outee("L1TEGammaIsoPre_EE.plt");
  writers::Gnuplot1DWriter writeree(ee_data);
  writeree.setTitle("L1TEGammaIsoPre EE");
  writeree.setOutput("L1TEGammaIsoPre_EE.png");
  outee << writeree << std::endl;
  outee.close();
}

}  // namespace plugins
}  // namespace dqmcpp