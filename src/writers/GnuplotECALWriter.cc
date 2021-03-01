#include "GnuplotECALWriter.hh"
#include "../filters/ECALFilters.hh"
#include <algorithm>
#include <iostream>
#include <iomanip>
#include "../logging/logging.hh"

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

static void writeBarrel(std::ostream &os, ECALHardware::RunData &rd, const int numdata)
{
    auto barrel = ECALFilters::barrel(rd.channeldata);
    // sorting for gnuplot
    std::sort(barrel.begin(), barrel.end(), BarrelSort);
    os << "set xrange [-0.5:360]" << std::endl
       << "set yrange [-84.5: 85]" << std::endl
       << "set view map" << std::endl
       << "set xlabel \"iphi\"" << std::endl
       << "set ylabel \"ieta\"" << std::endl
       << "set title \"ECAL Barrel\"" << std::endl
       << "set grid front" << std::endl
       << "set xtics 20" << std::endl
       << "set ytics 100" << std::endl
       << "set cbrange [0:40]" << std::endl
       << "set palette defined (-1 \"white\", 0 \"yellow\", 40.0 \"red\")" << std::endl;
    // labels
    for (int i = -18; i < 19; ++i) {
        if (i == 0)
            continue;
        const std::string sign = (i > 0) ? "+" : "-";
        const int xpos = 360 * (std::abs(i) - 1) / 18 + 1;
        const int ypos = (i > 0) ? 44 : -44;
        os << "set label front \"" << sign << std::setw(2) << std::setfill('0') << std::abs(i) << "\" at " << xpos << "," << ypos << std::endl;
    }
    os << "$map" << numdata << " << EOD" << std::endl;
    for (auto &ch : barrel) {
        os << ch.channel.ix_iphi << " " << ch.channel.iy_ieta << " " << ch.value << std::endl;
    }
    os << "EOD" << std::endl;
    os << "plot '$map" << numdata << "' using 1:2:3 w image" << std::endl;
}

static void writeGnuplot(std::ostream &os, std::vector<ECALHardware::RunData> &rd)
{
    // filter barrel
    for (int i = 0; i < rd.size(); ++i) {
        auto r = rd.at(i);
        writeBarrel(os, r, i);
    };
}

std::ostream &operator<<(std::ostream &os, const GnuplotECALWriter &gw)
{
    writeGnuplot(os, *(gw.rd));
    return os;
}
