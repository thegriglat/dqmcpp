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

using namespace std;
using namespace dqmcpp;

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
  std::copy(__xlabels.begin(), __xlabels.end(), std::back_inserter(_xlabels));
  std::copy(__ylabels.begin(), __ylabels.end(), std::back_inserter(_ylabels));
  std::sort(_xlabels.begin(), _xlabels.end());
  std::sort(_ylabels.begin(), _ylabels.end());
}

void Gnuplot2DWriter::setSortYFn(
    std::function<bool(const std::string&, const std::string&)> sort_function) {
  sortfn = sort_function;
  std::sort(_ylabels.begin(), _ylabels.end(), sortfn);
};

void Gnuplot2DWriter::addBox(const std::string& xlabel,
                             const std::string& ylabel,
                             const unsigned int pattern) {
  const auto xbin = common::index(_xlabels, xlabel);
  const auto ybin = common::index(_ylabels, ylabel);
  if (xbin < 0 || ybin < 0)
    return;
  boxes.emplace_back(xbin, ybin, pattern);
}

std::ostream& operator<<(std::ostream& os,
                         const Gnuplot2DWriter::Rectangle& r) {
  const float x1 = r.xbin - 0.5;
  const float x2 = r.xbin + 0.5;
  const float y1 = r.ybin - 0.5;
  const float y2 = r.ybin + 0.5;
  os << "set object rect from " << x1 << "," << y1 << " to " << x2 << "," << y2
     << "front fc lt -1 fs transparent pattern " << r.pattern << " lw 0"
     << std::endl;
  return os;
}

std::ostream& operator<<(std::ostream& os, const Gnuplot2DWriter& gw) {
  // prepare default map
  using MapPair = std::pair<std::pair<std::string, std::string>, double>;
  // copy data to vector to partition it later
  vector<MapPair> alldata;
  alldata.reserve(gw._data->size());
  std::transform(gw._data->begin(), gw._data->end(),
                 std::back_inserter(alldata),
                 [](const auto pair) { return pair; });
  auto index = [&gw](const int x, const int y) {
    return gw.ncolumns() * x + y;
  };
  double scale = 1;
  if (gw.nrows() != 0)
    scale = static_cast<double>(gw.ncolumns()) / gw.nrows();
  // scale in 0.5 .. 3
  scale = std::min(std::max(scale, 0.5), 3.);
  // pagescale is common page scale for large run numbers
  // optimum is about 1024px/24runs ~ 40 px/run
  double pagescale = 1.0;
  if (1024. / gw.ncolumns() < 40 || 768. * scale / gw.nrows() < 40) {
    pagescale =
        std::max(40.0 / 1024. * gw.ncolumns(), 40 / 368. / scale * gw.nrows());
  }
  pagescale = std::min(std::max(pagescale, 0.8), 5.0);

  os << "scale = " << scale << std::endl
     << "pagescale = " << pagescale << std::endl
     << "set term pngcairo size pagescale * 1024,768*scale * pagescale "
        "fontscale pagescale"
     << std::endl
     << "set palette defined " << gw.palette_str() << std::endl;
  os << "set cbrange [" << gw.getZ().min << ":" << gw.getZ().max << "]"
     << std::endl;
  os << "set zrange [" << gw.getZ().min << ":" << gw.getZ().max << "]"
     << std::endl;
  os << "set cbtics " << gw.getZTick() << std::endl;
  os << "set xtics rotate 90" << std::endl;
  const auto xlabel_chunks = common::chunks(gw._xlabels, gw.getChunkSize());
  const auto ylabels = gw._ylabels;
  // cache y indices
  std::map<std::string, int> yindexes;
  for (auto y : ylabels) {
    yindexes[y] = common::index(ylabels, y);
  }
  for (size_t nch = 0; nch < xlabel_chunks.size(); ++nch) {
    const auto xl_chunk = xlabel_chunks.at(nch);
    std::vector<double> all(xl_chunk.size() * gw.ncolumns());
    std::fill(all.begin(), all.end(), DEFAULT_VALUE);
    const auto endit = std::partition(
        alldata.begin(), alldata.end(), [&xl_chunk](const auto pair) {
          return common::has(xl_chunk, pair.first.first);
        });
    // cache x indices
    std::map<std::string, int> xindexes;
    for (auto x : xl_chunk) {
      xindexes[x] = common::index(xl_chunk, x);
    }
    for (auto it = alldata.begin(); it != endit; ++it) {
      const auto& xl = it->first.first;
      const auto& yl = it->first.second;
      const auto ix = xindexes[xl];
      const auto iy = yindexes[yl];
      all.at(index(ix, iy)) = it->second;
    }
    if (xl_chunk.size() == 0) {
      std::cout << "No x labels to plot. Chunk #" << nch << std::endl;
      continue;
    }
    // grid. use additional grid to draw lines
    os << "set x2tics 1 format '' scale 0,0.001" << std::endl
       << "set y2tics 1 format '' scale 0,0.001" << std::endl
       << "set mx2tics 2" << std::endl
       << "set my2tics 2" << std::endl
       << "set x2range[-0.5:" << xl_chunk.size() - 0.5 << "]" << std::endl
       << "set y2range[-0.5:" << gw.ncolumns() - 0.5 << "]" << std::endl
       << "set yrange[-0.5:" << gw.ncolumns() - 0.5 << "]" << std::endl
       << "set grid front mx2tics my2tics lw 1"
       << std::endl
       // remove x/y tics, keep labels
       << "set xtics scale 0" << std::endl
       << "set ytics scale 0" << std::endl
       << "set title \"" << gw.getTitle() << "\"" << std::endl;
    if (gw.getLogscale().size() != 0)
      os << "set logscale " << gw.getLogscale() << std::endl;
    for (auto& box : gw.boxes)
      os << box << std::endl;
    const auto low_label = xl_chunk.front();
    const auto high_label = xl_chunk.back();
    // get extension
    std::string filename = gw.getOutput();
    if (xlabel_chunks.size() != 1) {
      const auto parts = common::split(gw.getOutput(), ".");
      filename = parts.at(0) + "." + std::to_string(nch) + "." + parts.at(1);
    }
    os << "set output \"" << filename << "\"" << std::endl;
    if (gw.ncolumns() != 0) {
      os << "$map" << nch << " << EOD" << std::endl;
      os << "N ";
      for (auto& e : xl_chunk)
        os << e << " ";
      os << std::endl;
      for (size_t iy = 0; iy < gw.ncolumns(); ++iy) {
        const auto ylabel = gw._ylabels.at(iy);
        os << "\"" << ylabel << "\" ";
        for (size_t ix = 0; ix < xl_chunk.size(); ++ix) {
          os << all[index(ix, iy)] << " ";
        }
        os << std::endl;
      }
      /*
       * in case of 1 row data I have to hack gnuplot and insert dummy data
       * which won't be displayed
       */
      if (gw.ncolumns() == 1) {
        os << "\"dummy\"";
        for (unsigned int ix = 0; ix < xl_chunk.size(); ++ix)
          os << " 0";
        os << std::endl;
      }
      //  end of hack
      os << std::endl;
      os << "EOD" << std::endl;
      os << "plot '$map" << nch
         << "' matrix rowheaders columnheaders with image notitle" << std::endl;
    } else {
      os << "print 'no data to plot'" << std::endl;
    }
  }
  return os;
}

void Gnuplot2DWriter::setXlabels(const std::vector<std::string>& xlabels) {
  _xlabels = xlabels;
  std::sort(_xlabels.begin(), _xlabels.end());
}

}  // namespace writers
}  // namespace dqmcpp