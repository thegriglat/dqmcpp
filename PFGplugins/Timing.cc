#include "Timing.hh"

#include <array>
#include <fstream>
#include <map>
#include <string>
#include <vector>
#include "ECAL/ECAL.hh"
#include "common/common.hh"
#include "common/gnuplot.hh"
#include "net/DQMURLProvider.hh"
#include "net/URLCache.hh"
#include "readers/JSONReader.hh"
#include "writers/Gnuplot1DWriter.hh"
#include "writers/ProgressBar.hh"

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

std::string get_url(const ECAL::Run& run, const int iz) {
  const array<std::string, 3> a = {
      net::DQMURL::dqmurl(run,
                          "EcalEndcap/EESummaryClient/EETMT EE - "
                          "timing mean 1D summary"),
      net::DQMURL::dqmurl(
          run, "EcalBarrel/EBSummaryClient/EBTMT timing mean 1D summary"),
      net::DQMURL::dqmurl(run,
                          "EcalEndcap/EESummaryClient/EETMT EE + "
                          "timing mean 1D summary")};
  return a.at(iz + 1);
}

}  // namespace

namespace dqmcpp {
namespace plugins {

void Timing::Process() {
  writers::ProgressBar progress(3 * runListReader->runs().size());
  for (int iz = -1; iz <= 1; ++iz) {
    progress.setLabel(det(iz));
    vector<ECAL::RunData<double>> rundata;
    const auto contents = net::URLCache::get(common::map<ECAL::Run, string>(
        runListReader->runs(),
        [iz](const ECAL::Run& run) { return get_url(run, iz); }));

    for (size_t i = 0; i < contents.size(); ++i) {
      const auto content = readers::JSONReader::parse1D(contents.at(i));
      const auto default_a = (iz == 0) ? 1e4 : 1e3;
      const auto gauss_fit = common::gnuplot::gauss(default_a, 0.1, 1.0);
      const auto fit_result = common::gnuplot::fit(
          content,
          [](const ECAL::Data1D& d1) {
            return std::pair<double, double>(d1.base.x, d1.value);
          },
          gauss_fit);
      const auto mu = fit_result.getParameter("mu").value;
      rundata.push_back(ECAL::RunData<double>(runListReader->runs().at(i), mu));
      progress.increment();
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