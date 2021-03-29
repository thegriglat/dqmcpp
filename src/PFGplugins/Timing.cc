#include "Timing.hh"

#include <fstream>
#include <iostream>
#include <map>
#include <string>
#include <vector>
#include "ECAL/ECAL.hh"
#include "common/common.hh"
#include "common/gnuplot.hh"
#include "net/DQMURLProvider.hh"
#include "readers/JSONReader.hh"
#include "writers/Gnuplot1DWriter.hh"

using namespace std;
using namespace dqmcpp;

REGISTER_PLUGIN(Timing)

namespace {

std::string det(const int iz) {
  switch (iz) {
    case -1:
      return "EE-";
    case 0:
      return "EB";
    case 1:
      return "EE+";
  }
  return "UNDEFINED";
}

struct URLInfo {
  int iz;
  std::string url;
  URLInfo(int iz, std::string url) : iz(iz), url(url){};
};

URLInfo get_url(const int run, const std::string& dataset, const int iz) {
  vector<URLInfo> a = {
      {-1, net::DQMURL::dqmurl(run, dataset,
                               "EcalEndcap/EESummaryClient/EETMT EE - "
                               "timing mean 1D summary")},
      {0, net::DQMURL::dqmurl(
              run, dataset,
              "EcalBarrel/EBSummaryClient/EBTMT timing mean 1D summary")},
      {1, net::DQMURL::dqmurl(run, dataset,
                              "EcalEndcap/EESummaryClient/EETMT EE + "
                              "timing mean 1D summary")}};
  return a.at(iz + 1);
}

}  // namespace

namespace dqmcpp {
namespace plugins {

void Timing::Process() {
  for (int iz = -1; iz <= 1; ++iz) {
    vector<ECAL::RunData<double>> rundata;
    for (auto& run : runListReader->runs()) {
      const auto url = get_url(run.runnumber, run.dataset, iz);
      cout << url.url << endl;
      const auto content =
          readers::JSONReader::parse1D(readers::JSONReader::get(url.url));
      const auto default_a = (iz == 0) ? 1e4 : 1e3;
      const auto gauss_fit = common::gnuplot::gauss(default_a, 0.1, 1.0);
      const auto fit_result = common::gnuplot::fit(
          content,
          [](const ECAL::Data1D& d1) {
            return std::pair<double, double>(d1.x, d1.value);
          },
          gauss_fit);
      const auto mu = fit_result.getParameter("mu").value;
      rundata.push_back(ECAL::RunData<double>(run, mu));
    }
    // plot
    string filename = "Timing_mean_" + det(iz) + ".plt";
    ofstream out(filename);

    writers::Gnuplot1DWriter writer(rundata,
                                    [](const ECAL::RunData<double>& rd) {
                                      return std::pair<std::string, double>(
                                          to_string(rd.run.runnumber), rd.data);
                                    });
    writer.setTitle("Timing mean " + det(iz));
    writer.setOutput("Timing_mean_" + det(iz) + ".png");
    writer.with("linespoints");
    writer.setY(-3, 3);
    out << writer << endl;
    out.close();
  }
}  // namespace plugins
}  // namespace plugins
}  // namespace dqmcpp