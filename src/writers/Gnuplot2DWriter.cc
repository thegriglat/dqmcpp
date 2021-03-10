/**
 * @file Gnuplot2DWriter.cc
 * @author Grigory Latyshev (thegriglat@gmail.com)
 * @brief Gnuplot2DWriter implementation
 */
#include "Gnuplot2DWriter.hh"

#include <algorithm>
#include <iostream>
#include "../common/common.hh"

#define DEFAULT_VALUE (0)

Gnuplot2DWriter::Gnuplot2DWriter(Data2D& data) : _data(data) {
  for (auto& elem : data) {
    const auto xlabel = elem.first.first;
    const auto ylabel = elem.first.second;
    if (!has(_xlabels, xlabel)) {
      _xlabels.push_back(xlabel);
    }
    if (!has(_ylabels, ylabel)) {
      _ylabels.push_back(ylabel);
    }
  }
  // TODO: sort?
  std::sort(_xlabels.begin(), _xlabels.end());
  std::sort(_ylabels.begin(), _ylabels.end());
}

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
      if (gw._data.find({xlabel, ylabel}) != gw._data.end())
        all[index(ix, iy)] = gw.get(xlabel, ylabel);
    }
  }
  // print x labels
  auto scale = (double)gw.ncolumns() / gw.nrows();
  os << "scale = " << scale << std::endl
     << "set term pngcairo size 1024*scale,768*scale*scale fontscale scale "
        "linewidth "
        "scale pointscale scale"
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
     << "set xtics scale 0" << std::endl
     << "set ytics scale 0" << std::endl
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

Gnuplot2DWriter& Gnuplot2DWriter::setPalette(Palette palette) {
  _palette = palette;
  std::sort(_palette.begin(), _palette.end(),
            [](PaletteColor& a, PaletteColor& b) {
              return a.zposition < b.zposition;
            });
  return *this;
}

std::string Gnuplot2DWriter::palette_str() const {
  std::string s = "(";
  for (unsigned int i = 0; i < _palette.size(); ++i) {
    auto& e = _palette.at(i);
    const auto palette_pos =
        _zaxis.min + e.zposition * (_zaxis.max - _zaxis.min);
    s += std::to_string(palette_pos) + " \"" + e.color + "\"";
    if (i != _palette.size() - 1)
      s += ", ";
  }
  s += ")";
  return s;
}