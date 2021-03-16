#include "gnuplot.hh"
#include <fstream>
#include <iostream>
#include <sstream>
#include "common.hh"
#include "pstream.h"

namespace dqmcpp {
namespace common {
namespace gnuplot {

std::string exec(const std::string& content) {
  using namespace std;
  redi::pstream gnuplot("gnuplot", redi::pstreams::pstdout |
                                       redi::pstreams::pstdin |
                                       redi::pstreams::pstderr);
  gnuplot << content << std::endl << redi::peof;
  std::stringstream ss;
  ss << gnuplot.err().rdbuf();
  gnuplot.close();
  return ss.str();
};

Fit GaussianFit("a/(sigma*sqrt(2.*pi))*exp(-(x-mu)**2./(2.*sigma**2))",
                {FitParameter("a"), FitParameter("mu"), FitParameter("sigma")});

Fit fit(const std::vector<double>& x,
        const std::vector<double>& y,
        const Fit& fit) {
  using namespace std;
  if (x.size() != y.size()) {
    cerr << "fit: x and y values have different size !" << endl;
    return fit;
  }
  stringstream ss;
  ss << "$map1 << EOD" << endl;
  for (unsigned int i = 0; i < x.size(); ++i) {
    const auto& xi = x.at(i);
    const auto& yi = y.at(i);
    ss << xi << " " << yi << endl;
  }
  ss << "EOD" << endl;
  ss << "set fit quiet" << endl;
  ss << "f(x) = " << fit.equation << endl;
  vector<string> paramnames;
  for (auto& param : fit.parameters) {
    ss << param.name << " = " << param.value << endl;
    paramnames.push_back(param.name);
  }
  ss << "fit f(x) '$map1' using 1:2 via " << common::join(paramnames, ",")
     << endl;
  ss << "print " << common::join(paramnames, ",") << endl;
  // fit [-6:6] gauss(x) '$map1' using (column(0)/2 - 25):(($1 != 0)?$1 : 1/0)
  // via a, sigma, mu
  const auto script = ss.str();
  const auto gnuplotOutput = exec(script);
  // new fit to be returned
  Fit newFit(fit);
  auto values = common::split(gnuplotOutput);
  for (unsigned int i = 0; i < newFit.parameters.size(); ++i) {
    newFit.parameters.at(i).value = std::atof(values.at(i).c_str());
  }
  return newFit;
}

}  // namespace gnuplot
}  // namespace common
}  // namespace dqmcpp