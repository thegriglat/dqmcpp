#include "RMSChannels.hh"

#include <string>
#include <fstream>
#include <cmath>
#include "../writers/GnuplotECALWriter.hh"
#include "../readers/DQMURLProvider.hh"
#include "../dataclasses/ecalchannels.hh"

#define RMSMAX (5)

static const auto rmslimit = [](const double rms)
{
    return rms > RMSMAX;
};

std::vector<std::string>
RMSPlugin::urls(const unsigned int runnumber, const std::string &dataset)
{
    std::vector<std::string> urls;
    char *plot = new char[72];
    for (int i = -18; i < 19; ++i) {
        if (i == 0)
            continue;
        sprintf(plot, "EcalBarrel/EBPedestalOnlineClient/EBPOT pedestal rms map G12 EB%+03d", i);
        urls.push_back(
            DQMURL::dqmurl(runnumber, dataset, plot));
    }
    for (int i = -9; i < 10; ++i) {
        if (i == 0)
            continue;
        sprintf(plot, "EcalEndcap/EEPedestalOnlineClient/EEPOT pedestal rms map G12 EE%+03d", i);
        urls.push_back(
            DQMURL::dqmurl(runnumber, dataset, plot));
    }
    delete[] plot;
    return urls;
}
std::vector<ECAL::RunData> RMSPlugin::analyze(const std::vector<ECAL::RunData> &rundata)
{
    std::vector<ECAL::Channel> badchannels;
    badchannels.reserve(100);
    for (auto &e : rundata) {
        for (auto &channeldata : e.channeldata) {
            if (rmslimit(channeldata.value)) {
                if (std::find(badchannels.begin(), badchannels.end(), channeldata.channel) == badchannels.end())
                    badchannels.push_back(channeldata.channel);
            }
        }
    }
    std::vector<ECAL::RunData> rd;
    rd.reserve(rundata.size());
    for (auto &e : rundata) {
        std::vector<ECAL::ChannelData> bd;
        std::copy_if(e.channeldata.begin(), e.channeldata.end(), std::back_inserter(bd),
        [badchannels](const ECAL::ChannelData &ecd) {
            return std::find(badchannels.begin(), badchannels.end(), ecd.channel) != badchannels.end();
        });
        rd.push_back(ECAL::RunData(e.run, bd));
    }
    return rd;
}
void RMSPlugin::plot(const std::vector<ECAL::RunData> &rundata)
{
    if (rundata.size() == 0)
        return;
    std::ofstream out("rms_channels.plt");
    out << "set xtics rotate 90" << std::endl
        << "set cbrange [0:100]" << std::endl
        << "set cbtics 5" << std::endl
        << "set palette defined (0 \"white\", 3 \"green\", 3 \"yellow\", 5 \"yellow\", 100 \"red\")" << std::endl;

    const auto scale = (double)rundata.at(0).channeldata.size() / rundata.size();
    out << "scale = " << scale << std::endl
        << "set size ratio scale" << std::endl
        << "set term pngcairo size 1024,768*scale" << std::endl
        << "set xlabel \"Run number\"" << std::endl
        << "set ylabel \"Channel\"" << std::endl
        << "set title \"Channels with RMS >" << RMSMAX << "\"" << std::endl;
    out << "$map1 << EOD" << std::endl;
    out << rundata.size() << " ";
    for (auto &e : rundata) {
        // print runs
        out << e.run.runnumber << " ";
    }
    out << std::endl;
    // print TT and data for all runs
    // all channels are equal
    const auto &channels = rundata.at(1).channeldata;
    std::cout << "Found " << channels.size() << " bad channels" << std::endl;
    for (auto channel : channels) {
        std::cout << ".";
        auto info = ECALChannels::find(channel.channel);
        if (info) {
            // channel found
            out << "\"" << info->det << " TT" << info->ccu << " [" << channel.channel.ix_iphi << "," << channel.channel.iy_ieta << "]\"";
            std::vector<double> channel_values;
            // get data from all runs for given channel
            channel_values.reserve(rundata.size());
            for (auto &rund : rundata) {
                auto it = std::find_if(rund.channeldata.begin(),
                                       rund.channeldata.end(),
                [channel](const ECAL::ChannelData &cd) {
                    return cd.channel == channel.channel;
                });
                channel_values.push_back(it->value);
            }
            for (auto value : channel_values) {
                const auto printed_value = (std::isnan(value)) ? -2 : value;
                out << " " << printed_value;
            }
            out << std::endl;
        }
    }
    std::cout << std::endl;
    out << "EOD" << std::endl
        << "set output \"rms_channels.png\"" << std::endl;
    out << "plot '$map1' matrix rowheaders columnheaders with image" << std::endl;
    out.close();
    return;
}