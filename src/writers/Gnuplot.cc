/**
 * @file Gnuplot.cc
 * @author Grigory Latyshev (thegriglat@gmail.com)
 * @brief Gnuplot implementation
 *
 *
 */
#include "Gnuplot.hh"

#include <sstream>
#include <string>

namespace {
std::string dbl2str(const double d) {
  std::ostringstream ss;
  ss << d;
  return ss.str();
}

}  // namespace

namespace dqmcpp {
namespace writers {

Axis Gnuplot::axis(const double min, const double max) {
  double _min = std::min(min, max);
  double _max = std::max(min, max);
  return {dbl2str(_min), dbl2str(_max)};
}

}  // namespace writers
}  // namespace dqmcpp