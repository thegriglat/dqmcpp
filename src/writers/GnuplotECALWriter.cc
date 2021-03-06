/**
 * @file GnuplotECALWriter.cc
 * @author Grigory Latyshev (thegriglat@gmail.com)
 * @brief GnuplotECALWriter implementation
 * @todo Rewrite using Gnuplot2DWriter
 */
#include "GnuplotECALWriter.hh"
#include <algorithm>
#include <iomanip>
#include "../common/functional.hh"
#include "../common/logging.hh"
#include "GnuplotECALWriterLines.hh"

namespace {
using namespace dqmcpp;
using namespace dqmcpp::writers;

inline void drawLine(std::ostream& os,
                     int x1,
                     int y1,
                     int x2,
                     int y2,
                     const std::string& color = "black") {
  os << "set arrow front nohead from " << x1 << "," << y1 << " to " << x2 << ","
     << y2 << "lc rgb \"" << color << "\"" << std::endl;
}

static void drawEELines(std::ostream& os) {
  for (auto& e : EndcapLines()) {
    drawLine(os, e[0], e[1], e[2], e[3]);
  }
}

struct Point {
  int x;
  int y;
  double value;
};

void writeBarrel(std::ostream& os,
                 const GnuplotECALWriter& gw,
                 ECAL::RunChannelData& rd,
                 const int numdata) {
  auto barrel = common::filter(
      rd.data, [](const ECAL::ChannelData& cd) { return cd.base.isEB(); });
  os << "set xrange [0:360]" << std::endl
     << "set yrange [-85.5: 85.5]" << std::endl
     << "set size ratio 0.472" << std::endl
     << "set xlabel \"iphi\"" << std::endl
     << "set ylabel \"ieta\"" << std::endl
     << "set title \"ECAL Barrel " << gw.getTitle() << "\"" << std::endl
     << "set xtics 20" << std::endl
     << "set ytics 100" << std::endl;
  // labels
  for (int i = -18; i < 19; ++i) {
    if (i == 0)
      continue;
    const std::string sign = (i > 0) ? "+" : "-";
    const int xpos = 360 * (std::abs(i) - 1) / 18 + 3;
    const int ypos = (i > 0) ? 44 : -44;
    os << "set label front \"" << sign << std::setw(2) << std::setfill('0')
       << std::abs(i) << "\" at " << xpos << "," << ypos << std::endl;
  }
  os << "$map" << numdata << " << EOD" << std::endl;
  /**
   * The main idea that we allocate large array and pre-setup it with -1
   * Then we just update needed values
   * The same for endcap
   */
  std::array<Point, 361 * 86 * 2> points;
  auto index = [](int x, int y) { return 85 * 2 * x + y + 85; };

  for (int x = 1; x < 361; ++x) {
    for (int y = -85; y < 86; ++y) {
      points.at(index(x, y)).x = x;
      points.at(index(x, y)).y = y;
      points.at(index(x, y)).value = -1;
    }
  }
  for (auto& e : barrel) {
    auto x = e.base.ix_iphi;
    auto y = e.base.iy_ieta;
    auto val = e.value;
    points[index(x, y)].value = val;
  }
  for (int x = 1; x < 361; ++x) {
    for (int y = -85; y < 86; ++y) {
      os << x - 0.5 << " " << y << " " << points.at(index(x, y)).value
         << std::endl;
    }
  }
  os << "EOD" << std::endl;
  os << "set output \"eb_" << gw.getOutput() << "_" << rd.run.runnumber
     << ".png\"" << std::endl;
  os << "plot '$map" << numdata << "' using 1:2:3 w image notitle" << std::endl;
}

void writeEndcap(std::ostream& os,
                 const GnuplotECALWriter& gw,
                 ECAL::RunChannelData& rd,
                 const int numdata,
                 const int iz) {
  const auto endcap = common::filter(
      rd.data, [iz](const ECAL::ChannelData& cd) { return cd.base.iz == iz; });
  std::string title = (iz == 1) ? "ECAL EE+" : "ECAL EE-";
  os << "set xrange [0:100]" << std::endl
     << "set yrange [0:100]" << std::endl
     << "set xtics 5" << std::endl
     << "set ytics 5" << std::endl
     << "set xlabel \"ix\"" << std::endl
     << "set ylabel \"iy\"" << std::endl
     << "set title \"" << title << " " << gw.getTitle() << "\"" << std::endl
     << "set size square" << std::endl;
  drawEELines(os);
  drawEESM(os, iz);
  os << "$map" << numdata << " << EOD" << std::endl;
  std::array<Point, 101 * 101> points;
  auto index = [](int x, int y) { return 100 * x + y; };
  for (int x = 0; x < 101; ++x) {
    for (int y = 0; y < 101; ++y) {
      points.at(index(x, y)).x = x;
      points.at(index(x, y)).y = y;
      points.at(index(x, y)).value = -1;
    }
  }
  for (auto& e : endcap) {
    auto x = e.base.ix_iphi;
    auto y = e.base.iy_ieta;
    auto val = e.value;
    points[index(x, y)].value = val;
  }
  for (int x = 1; x < 101; ++x)
    for (int y = 1; y < 101; ++y)
      os << x - 0.5 << " " << y - 0.5 << " " << points.at(index(x, y)).value
         << std::endl;
  os << "EOD" << std::endl;
  const std::string filename = (iz == 1) ? "eeplus" : "eeminus";
  os << "set output \"" << filename << "_" << gw.getOutput() << "_"
     << rd.run.runnumber << ".png\"" << std::endl;
  os << "plot '$map" << numdata << "' using 1:2:3 w image notitle" << std::endl;
}

static void writeGnuplot(std::ostream& os,
                         const GnuplotECALWriter& gw,
                         std::vector<ECAL::RunChannelData>& rd) {
  // filter barrel
  os << "scale = 2" << std::endl
     << "set term pngcairo size 1024*scale,768*scale fontscale scale linewidth "
        "scale pointscale scale"
     << std::endl
     << "set xtics rotate 90" << std::endl
     << "set view map" << std::endl
     << "set grid front" << std::endl
     << "set cbrange [" << gw.getZ().min << ":" << gw.getZ().max << "]"
     << std::endl
     << "set palette defined " << gw.palette_str() << std::endl;
  for (unsigned int i = 0; i < rd.size(); ++i) {
    auto r = rd.at(i);
    writeBarrel(os, gw, r, 3 * i);
    os << "unset label" << std::endl;

    writeEndcap(os, gw, r, 3 * i + 1, 1);
    os << "unset label" << std::endl;

    writeEndcap(os, gw, r, 3 * i + 2, -1);
    os << "unset label" << std::endl;
  };
}

}  // namespace

namespace dqmcpp {
namespace writers {

std::ostream& operator<<(std::ostream& os, const GnuplotECALWriter& gw) {
  writeGnuplot(os, gw, *(gw.rd));
  return os;
}

}  // namespace writers
}  // namespace dqmcpp