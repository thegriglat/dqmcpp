/**
 * @file Gnuplot1DWriter.cc
 * @author Grigory Latyshev (thegriglat@gmail.com)
 *
 */
#include "Gnuplot1DWriter.hh"

#include <iostream>
#include "../common/common.hh"

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
  os << "set xtics rotate 90" << std::endl;
  const auto xchunks = common::chunks(gw._data, gw.getChunkSize());
  for (size_t nch = 0; nch < xchunks.size(); ++nch) {
    const auto xchunk = xchunks.at(nch);
    os << "set xrange [-0.5:" << xchunk.size() - 0.5 << "]" << std::endl;
    if (gw.with_option != "boxes")
      os << "set grid lw 2" << std::endl;
    os << "set yrange [" << gw.getY().min << ":" << gw.getY().max << "]"
       << std::endl
       //   << "set ytics scale 0" << std::endl
       << "set title \"" << gw.getTitle() << "\"" << std::endl;
    os << "$map" << nch << " << EOD" << std::endl;
    for (auto& e : xchunk)
      os << "\"" << e.first << "\" " << e.second << std::endl;
    os << "EOD" << std::endl;
    std::string filename = gw.getOutput();
    if (xchunks.size() != 1) {
      const auto parts = common::split(gw.getOutput(), ".");
      filename = parts.at(0) + "." + std::to_string(nch) + "." + parts.at(1);
    }
    os << "set output \"" << filename << "\"" << std::endl;
    os << "plot '$map" << nch << "' u 2:xticlabels(1) ";
    if (gw.with_option.length() != 0)
      // TODO: fix hardcoded style
      os << "w " << gw.with_option << " ";
    if (gw.with_option.find("lines") != std::string::npos)
      os << "lt 1 lw 2 ";
    if (gw.with_option.find("points") != std::string::npos)
      os << "pt 7 pi -1 ps 1.5 ";
    os << "notitle" << std::endl;
  }
  return os;
}

}  // namespace writers
}  // namespace dqmcpp