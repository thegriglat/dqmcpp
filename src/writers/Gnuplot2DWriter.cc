/**
 * @file Gnuplot2DWriter.cc
 * @author Grigory Latyshev (thegriglat@gmail.com)
 * @brief Gnuplot2DWriter implementation
 */
#include "Gnuplot2DWriter.hh"

#include <algorithm>
#include <cmath>
#include <iostream>
#include <set>
#include <string>
#include "../common/common.hh"

#define DEFAULT_VALUE (0)

namespace dqmcpp {
namespace writers {

Gnuplot2DWriter::Gnuplot2DWriter(const Data2D& data) : _data(&data) {
  std::set<std::string> __xlabels;
  std::set<std::string> __ylabels;
  for (auto& elem : data) {
    const auto xlabel = elem.first.first;
    const auto ylabel = elem.first.second;
    __xlabels.insert(xlabel);
    __ylabels.insert(ylabel);
  }
  _xlabels.reserve(__xlabels.size());
  _ylabels.reserve(__ylabels.size());
  for (auto& e : __xlabels)
    _xlabels.push_back(e);
  for (auto& e : __ylabels)
    _ylabels.push_back(e);
  std::sort(_xlabels.begin(), _xlabels.end());
  std::sort(_ylabels.begin(), _ylabels.end());
}

void Gnuplot2DWriter::setSortYFn(
    std::function<bool(const std::string&, const std::string&)> sort_function) {
  sortfn = sort_function;
  std::sort(_ylabels.begin(), _ylabels.end(), sortfn);
};

std::ostream& operator<<(std::ostream& os, const Gnuplot2DWriter& gw) {
  // prepare default map

  double* all = new double[gw.nrows() * gw.ncolumns()];
  auto index = [&gw](const int x, const int y) {
    return gw.ncolumns() * x + y;
  };
  for (unsigned int i = 0; i < gw.ncolumns() * gw.nrows(); ++i) {
    all[i] = DEFAULT_VALUE;
  }
  for (unsigned int iy = 0; iy < gw._ylabels.size(); ++iy) {
    const auto ylabel = gw._ylabels.at(iy);
    for (unsigned int ix = 0; ix < gw._xlabels.size(); ++ix) {
      const auto xlabel = gw._xlabels.at(ix);
      if (gw._data->find({xlabel, ylabel}) != gw._data->end())
        all[index(ix, iy)] = gw.get(xlabel, ylabel);
    }
  }
  // print x labels
  auto scale = (double)gw.ncolumns() / gw.nrows();
  while (scale < 1)
    scale *= 10;
  scale = std::min(9.0, scale);
  int pagescale = std::max(9.0 / scale, 3.0);
  int ticksfontsize = std::trunc(12.0 / scale) + 1;
  os << "scale = " << scale << std::endl
     << "pagescale = " << pagescale << std::endl
     << "set term pngcairo size 1024*pagescale*scale,1024*pagescale linewidth "
        "pagescale pointscale pagescale fontscale pagescale"
     << std::endl
     << "set palette defined " << gw.palette_str() << std::endl;
  os << "set cbrange [" << gw.getZ().min << ":" << gw.getZ().max << "]"
     << std::endl;
  os << "set zrange [" << gw.getZ().min << ":" << gw.getZ().max << "]"
     << std::endl;
  os << "set cbtics " << gw.getZTick() << std::endl;
  os << "set xtics rotate 90" << std::endl;
  // grid. use additional grid to draw lines
  os << "set x2tics 1 format '' scale 0,0.001" << std::endl
     << "set y2tics 1 format '' scale 0,0.001" << std::endl
     << "set mx2tics 2" << std::endl
     << "set my2tics 2" << std::endl
     << "set x2range[-0.5:" << gw.nrows() - 0.5 << "]" << std::endl
     << "set y2range[-0.5:" << gw.ncolumns() - 0.5 << "]" << std::endl
     << "set grid front mx2tics my2tics lw 1"
     << std::endl
     // remove x/y tics, keep labels
     << "set xtics scale 0 font '," << ticksfontsize << "'" << std::endl
     << "set ytics scale 0 font '," << ticksfontsize << "'" << std::endl
     << "set title \"" << gw.getTitle() << "\"" << std::endl;
  os << "$map1 << EOD" << std::endl;
  os << "N ";
  for (auto& e : gw._xlabels)
    os << e << " ";
  os << std::endl;
  for (unsigned int iy = 0; iy < gw.ncolumns(); ++iy) {
    const auto ylabel = gw._ylabels.at(iy);
    os << "\"" << ylabel << "\" ";
    for (unsigned int ix = 0; ix < gw.nrows(); ++ix) {
      os << all[index(ix, iy)] << " ";
    }
    os << std::endl;
  }
  os << std::endl;
  os << "EOD" << std::endl;
  os << "set output \"" << gw.getOutput() << "\"" << std::endl;
  os << "plot '$map1' matrix rowheaders columnheaders with image notitle"
     << std::endl;
  delete[] all;
  return os;
}

}  // namespace writers
}  // namespace dqmcpp