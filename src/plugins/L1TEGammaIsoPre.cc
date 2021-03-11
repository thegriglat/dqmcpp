/**
 * @file L1TEGammaIsoPre.cc
 * @author Grigory Latyshev (thegriglat@gmail.com)
 * @brief L1TEGammaIsoPrePlugin implementation
 */
#include "L1TEGammaIsoPre.hh"
#include <algorithm>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include "../net/DQMURLProvider.hh"
#include "../readers/JSONReader.hh"
#include "../writers/Gnuplot1DWriter.hh"

using namespace std;

namespace {

using namespace dqmcpp;

struct RunProb {
  int run;
  double prob;
  RunProb(int run, double val) : run(run), prob(val){};
};

string urls(const int run, const std::string& dataset) {
  const std::string path =
      "L1T/L1TObjects/L1TEGamma/timing/First_bunch/ptmin_10p0_gev/"
      "egamma_noniso_bx_ieta_firstbunch_ptmin10p0";
  return net::DQMURL::dqmurl(run, dataset, path);
}
vector<ECAL::Data2D> filter(const vector<ECAL::Data2D>& data, const double x) {
  vector<ECAL::Data2D> r;
  for (auto& e : data)
    if (std::abs(e.x - x) < 0.1)
      r.push_back(e);
  return r;
}

double sum(const vector<ECAL::Data2D>& d) {
  double s = 0;
  for (auto& e : d)
    s += e.value;
  return s;
}

vector<RunProb> calcProb(const vector<ECAL::RunData2D>& rundata) {
  vector<RunProb> result;
  result.reserve(rundata.size());
  for (auto& rd : rundata) {
    auto minus1values = filter(rd.data, -1);
    auto zerovalues = filter(rd.data, 0);
    auto minus1_i = sum(minus1values);
    auto zero_i = sum(zerovalues);
    auto prob = minus1_i / (minus1_i + zero_i);
    result.push_back(RunProb(rd.run, prob));
  }
  return result;
}

vector<ECAL::Data2D> filter0m2(const vector<ECAL::Data2D>& data) {
  vector<ECAL::Data2D> r;
  for (auto& e : data)
    if (e.x > -1.5 && e.x < 0.5)
      r.push_back(e);
  return r;
}

}  // namespace
namespace dqmcpp {
namespace plugins {

void L1TEGammaIsoPrePlugin::Process() {
  vector<ECAL::RunData2D> rundata;
  rundata.reserve(runListReader->runs().size());
  for (auto& run : runListReader->runs()) {
    auto url = urls(run.runnumber, run.dataset);
    cout << run.dataset << " " << url << endl;
    auto content = reader->parse2D(reader->get(url));
    if (content.size() == 0)
      continue;
    content = filter0m2(content);
    rundata.push_back(ECAL::RunData2D(run.runnumber, content));
  }
  vector<RunProb> runprob = calcProb(rundata);
  ofstream out("l1.plt");
  writers::Gnuplot1DWriter::Data1D data;
  for (auto& rp : runprob) {
    data.push_back({std::to_string(rp.run), rp.prob});
  }
  writers::Gnuplot1DWriter writer(data);
  writer.setTitle("L1TEGammaIsoPre");
  writer.setOutput("L1TEGammaIsoPre.png");
  out << writer << std::endl;
  out.close();
}

}  // namespace plugins
}  // namespace dqmcpp