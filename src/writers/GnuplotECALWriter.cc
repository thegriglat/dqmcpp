#include "GnuplotECALWriter.hh"
#include "GnuplotECALWriterLines.hh"
#include "../filters/ECALFilters.hh"
#include <algorithm>
#include <iostream>
#include <iomanip>
#include "../common/logging.hh"

static bool BarrelSort(const ECALHardware::ChannelData &a, const ECALHardware::ChannelData &b)
{
    auto ax = a.channel.ix_iphi;
    auto ay = a.channel.iy_ieta;
    auto bx = b.channel.ix_iphi;
    auto by = b.channel.iy_ieta;
    if (ay == by) {
        return ax < bx;
    };
    return ay < by;
}

static inline void drawLine(std::ostream &os, int x1, int y1, int x2, int y2, const std::string color = "black")
{
    os << "set arrow front nohead from " << x1 << "," << y1 << " to " << x2 << "," << y2 << "lc rgb \"" << color << "\"" << std::endl;
}

static void drawEELines(std::ostream &os)
{
    for (auto &e : EndcapLines()) {
        drawLine(os, e[0], e[1], e[2], e[3]);
    }
}

struct Point {
    int x;
    int y;
    double value;
};

static void writeBarrel(std::ostream &os, ECALHardware::RunData &rd, const int numdata)
{
    auto barrel = ECALFilters::barrel(rd.channeldata);
    // sorting for gnuplot
    std::sort(barrel.begin(), barrel.end(), BarrelSort);
    os << "set xrange [0.5:360]" << std::endl
       << "set yrange [-84: 85]" << std::endl
       << "set size ratio 0.472" << std::endl
       << "set xlabel \"iphi\"" << std::endl
       << "set ylabel \"ieta\"" << std::endl
       << "set title \"ECAL Barrel\"" << std::endl
       << "set xtics 20" << std::endl
       << "set ytics 100" << std::endl;
    // labels
    for (int i = -18; i < 19; ++i) {
        if (i == 0)
            continue;
        const std::string sign = (i > 0) ? "+" : "-";
        const int xpos = 360 * (std::abs(i) - 1) / 18 + 3;
        const int ypos = (i > 0) ? 44 : -44;
        os << "set label front \"" << sign << std::setw(2) << std::setfill('0') << std::abs(i) << "\" at " << xpos << "," << ypos << std::endl;
    }
    os << "$map" << numdata << " << EOD" << std::endl;
    /**
     * The main idea that we allocate large array and pre-setup it with -1
     * Then we just update needed values
     * The same for endcap
     */
    Point *points = new Point[361 * 85 * 2];
    auto index = [](int x, int y) {
        return 85 * 2 * x + y + 84;
    };

    for (int x = 0; x < 360; ++x) {
        for (int y = -84; y < 85; ++y) {
            points[index(x, y)].x = x;
            points[index(x, y)].y = y;
            points[index(x, y)].value = -1;
        }
    }
    for (auto &e : barrel) {
        auto x = e.channel.ix_iphi;
        auto y = e.channel.iy_ieta;
        auto val = e.value;
        points[index(x, y)].value = val;
    }
    for (int x = 0; x < 360; ++x) {
        for (int y = -84; y < 85; ++y) {
            os << x + 0.5 << " " << y + 0.5 << " " << points[index(x, y)].value << std::endl;
        }
    }
    delete[] points;
    os << "EOD" << std::endl;
    os << "set output \"eb_" << rd.run.runnumber << ".png\"" << std::endl;
    os << "plot '$map" << numdata << "' using 1:2:3 w image notitle" << std::endl;
}

static void writeEndcap(std::ostream &os, ECALHardware::RunData &rd, const int numdata, const int iz)
{
    auto endcap = (iz == 1) ? ECALFilters::eeplus(rd.channeldata) : ECALFilters::eeminus(rd.channeldata);
    const std::string title = (iz == 1) ? "ECAL EE+" : "ECAL EE-";
    std::sort(endcap.begin(), endcap.end(), BarrelSort);
    os << "set xrange [0:100]" << std::endl
       << "set yrange [0:100]" << std::endl
       << "set xtics 5" << std::endl
       << "set ytics 5" << std::endl
       << "set xlabel \"ix\"" << std::endl
       << "set ylabel \"iy\"" << std::endl
       << "set title \"" << title << "\"" << std::endl
       << "set size square" << std::endl;
    drawEELines(os);
    drawEESM(os, iz);
    os << "$map" << numdata << " << EOD" << std::endl;
    Point *points = new Point[101 * 101];
    auto index = [](int x, int y) {
        return 100 * x + y;
    };
    for (int x = 0; x < 101; ++x) {
        for (int y = 0; y < 101; ++y) {
            points[index(x, y)].x = x;
            points[index(x, y)].y = y;
            points[index(x, y)].value = -1;
        }
    }
    for (auto &e : endcap) {
        auto x = e.channel.ix_iphi;
        auto y = e.channel.iy_ieta;
        auto val = e.value;
        points[index(x, y)].value = val;
    }
    for (int x = 0; x < 101; ++x)
        for (int y = 0; y < 101; ++y)
            os << x + 0.5 << " " << y + 0.5 << " " << points[index(x, y)].value << std::endl;
    delete[] points;
    os << "EOD" << std::endl;
    const std::string filename = (iz == 1) ? "eeplus" : "eeminus";
    os << "set output \"" << filename << "_" << rd.run.runnumber << ".png\"" << std::endl;
    os << "plot '$map" << numdata << "' using 1:2:3 w image notitle" << std::endl;
}

static void writeGnuplot(std::ostream &os, const GnuplotECALWriter &gw, std::vector<ECALHardware::RunData> &rd)
{
    // filter barrel
    os << "set term png size 1024,768" << std::endl;
    os << "set xtics rotate 90" << std::endl
       << "set view map" << std::endl
       << "set grid front" << std::endl
       << "set cbrange [" << gw.zrange().min << ":" << gw.zrange().max << "]" << std::endl
       << "set palette defined " << gw.palette_str() << std::endl;
    for (int i = 0; i < rd.size(); ++i) {
        auto r = rd.at(i);
        writeBarrel(os, r, 3 * i);
        os << "unset label" << std::endl;

        writeEndcap(os, r, 3 * i + 1, 1);
        os << "unset label" << std::endl;

        writeEndcap(os, r, 3 * i + 2, -1);
        os << "unset label" << std::endl;
    };
}

std::ostream &operator<<(std::ostream &os, const GnuplotECALWriter &gw)
{
    writeGnuplot(os, gw, *(gw.rd));
    return os;
}
