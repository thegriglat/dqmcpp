/**
 * @file Gnuplot1DWriter.cc
 * @author Grigory Latyshev (thegriglat@gmail.com)
 *
 */
#include "Gnuplot1DWriter.hh"

#include <iostream>

namespace dqmcpp {
namespace writers {

using namespace std;

std::ostream& operator<<(std::ostream& os, const Gnuplot1DWriter& gw) {
  auto scale = 1.0;
  os << "scale = " << scale << std::endl
     << "set term pngcairo size 1024*scale,768*scale*scale fontscale scale "
        "linewidth "
        "scale pointscale scale"
     << std::endl;
  os << "set xtics rotate 90" << std::endl
     << "set xrange [-0.5:" << gw._data.size() - 0.5 << "]" << std::endl;
  if (gw.with_option != "boxes")
    os << "set grid lw 2" << std::endl;
  os << "set yrange [" << gw.getY().min << ":" << gw.getY().max << "]"
     << std::endl
     //   << "set ytics scale 0" << std::endl
     << "set title \"" << gw.getTitle() << "\"" << std::endl;
  os << "$map1 << EOD" << std::endl;
  for (auto& e : gw._data)
    os << "\"" << e.first << "\" " << e.second << std::endl;
  os << "EOD" << std::endl;
  os << "set output \"" << gw.getOutput() << "\"" << std::endl;
  os << "plot '$map1' u 2:xticlabels(1) ";
  if (gw.with_option.length() != 0)
    // TODO: fix hardcoded style
    os << "w " << gw.with_option << " ";
  if (gw.with_option.find("lines") != std::string::npos)
    os << "lt 1 lw 2 ";
  if (gw.with_option.find("points") != std::string::npos)
    os << "pt 7 pi -1 ps 1.5 ";
  os << "notitle" << std::endl;
  return os;
}

}  // namespace writers
}  // namespace dqmcpp