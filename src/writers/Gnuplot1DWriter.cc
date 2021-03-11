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
     << "set xrange [-0.5:" << gw._data->size() - 1.5 << "]" << std::endl
     << "set xtics scale 0" << std::endl
     << "set ytics scale 0" << std::endl
     << "set title \"" << gw.getTitle() << "\"" << std::endl;
  os << "$map1 << EOD" << std::endl;
  for (auto& e : *(gw._data))
    os << "\"" << e.first << "\" " << e.second << std::endl;
  os << "EOD" << std::endl;
  os << "set output \"" << gw.getOutput() << "\"" << std::endl;
  os << "plot '$map1' u 2:xticlabels(1) w boxes notitle" << std::endl;
  return os;
}

}  // namespace writers
}  // namespace dqmcpp