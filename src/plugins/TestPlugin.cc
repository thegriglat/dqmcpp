#include "TestPlugin.hh"

#include <string>
#include <fstream>
#include "../writers/GnuplotECALWriter.hh"
#include "../readers/DQMURLProvider.hh"

std::vector<std::string> TestPlugin::urls(const unsigned int runnumber, const std::string &dataset)
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
std::vector<ECALHardware::RunData> TestPlugin::analyze(const std::vector<ECALHardware::RunData> &rundata)
{
    return rundata;
}
void TestPlugin::plot(const std::vector<ECALHardware::RunData> &rundata)
{
    for (auto &r : rundata) {
        auto run = r.run.runnumber;
        std::string outfile = std::to_string(run) + ".plt";
        std::ofstream out(outfile);
        std::vector<ECALHardware::RunData> rd = {r};
        out << GnuplotECALWriter(rd) << std::endl;
        out.close();
    }
}