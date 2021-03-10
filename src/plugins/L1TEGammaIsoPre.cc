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
#include "../readers/DQMURLProvider.hh"
#include "../readers/JSONReader.hh"

using namespace std;
struct RunProb {
  int run;
  double prob;
  RunProb(int run, double val) : run(run), prob(val){};
};
namespace {
string urls(const int run, const std::string& dataset) {
  const std::string path =
      "L1T/L1TObjects/L1TEGamma/timing/First_bunch/ptmin_10p0_gev/"
      "egamma_noniso_bx_ieta_firstbunch_ptmin10p0";
  return DQMURL::dqmurl(run, dataset, path);
}
vector<ECAL::Data2D> filter(vector<ECAL::Data2D>& data, double x) {
  vector<ECAL::Data2D> r;
  std::copy_if(data.begin(), data.end(), std::back_inserter(r),
               [x](const ECAL::Data2D& d) { return d.x == x; });
  return r;
}

double sum(vector<ECAL::Data2D>& d) {
  double s = 0;
  for (auto& e : d)
    s += e.value;
  return s;
}

vector<RunProb> calcProb(vector<ECAL::RunData2D>& rundata) {
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

vector<ECAL::Data2D> filter0m2(vector<ECAL::Data2D>& data) {
  data.erase(std::remove_if(data.begin(), data.end(),
                            [](const ECAL::Data2D& d) {
                              return d.x < -1.5 || d.x > 0.5 ||
                                     std::abs(d.y) > 40;
                            }),
             data.end());
  return data;
}

}  // namespace

void L1TEGammaIsoPrePlugin::Process() {
  vector<ECAL::RunData2D> rundata;
  rundata.reserve(runListReader->runs().size());
  for (auto& run : runListReader->runs()) {
    auto url = urls(run.runnumber, run.dataset);
    cout << run.dataset << " " << url << endl;
    auto content = reader->parse2D(reader->get(url));
    content = filter0m2(content);
    rundata.push_back(ECAL::RunData2D(run.runnumber, content));
  }
  vector<RunProb> runprob = calcProb(rundata);
  ofstream out("l1.dat");
  for (auto& e : runprob)
    out << e.run << " " << e.prob << endl;
  out.close();
}