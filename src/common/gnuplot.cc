/**
 * @file gnuplot.cc
 * @author Grigory Latyshev (thegriglat@gmail.com)
 * @brief Gnuplot.hh implementation
 *
 *
 */
#include "gnuplot.hh"
#include <algorithm>
#include <fstream>
#include <iostream>
#include <sstream>
#include "../common/common.hh"
#include "common.hh"
#include "pstream.h"

namespace dqmcpp {
namespace common {
namespace gnuplot {

/**
 * @brief Execute gnuplot instructions
 *
 * @param content Gnuplot input
 * @return std::string
 */
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

/**
 * @brief Returns gauss fit function for gnuplot
 *
 * @param a
 * @param mu
 * @param sigma
 * @return Fit
 */
Fit gauss(double a, double mu, double sigma) {
  return Fit("a/(sigma*sqrt(2.*pi))*exp(-(x-mu)**2./(2.*sigma**2))",
             {FitParameter("a", a), FitParameter("mu", mu),
              FitParameter("sigma", sigma)});
}

/**
 * @brief Fit [x,y] points with provided fit function
 *
 * @param x x values
 * @param y y values
 * @param fit fitting function object
 * @return Fit
 */
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
  ss << "set fit logfile '/dev/null'" << endl;
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

/**
 * @brief Retrieve fitted parameters after fit()
 *
 * @param name Parameter name
 * @return const FitParameter&
 */
const FitParameter& Fit::getParameter(const std::string& name) const {
  auto idx = dqmcpp::common::index(
      parameters, [&name](const FitParameter& e) { return e.name == name; });
  if (idx >= 0)
    return parameters.at(idx);
  throw "Cannot find fit parameter with name " + name;
}

}  // namespace gnuplot
}  // namespace common
}  // namespace dqmcpp