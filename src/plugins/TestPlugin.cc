#include "TestPlugin.hh"

std::vector<std::string> TestPlugin::urls(const unsigned int runnumber, const std::string &dataset)
{
    std::vector<std::string> urls;
    for (int i = -18; i < 19; ++i) {
        if (i == 0)
            continue;
        char *s = new char[1024];
        sprintf(s, "https://cmsweb.cern.ch/dqm/offline/jsonfairy/archive/%d/EGamma/Run2018A-12Nov2019_UL2018-v2/DQMIO/EcalBarrel/EBPedestalOnlineClient/EBPOT%%20pedestal%%20rms%%20map%%20G12%%20EB%+03d", runnumber, i);
        urls.push_back(s);
        delete[] s;
    }
    for (int i = -9; i < 10; ++i) {
        if (i == 0)
            continue;
        char *s = new char[1024];
        sprintf(s, "https://cmsweb.cern.ch/dqm/offline/jsonfairy/archive/%d/EGamma/Run2018A-12Nov2019_UL2018-v2/DQMIO/EcalEndcap/EEPedestalOnlineClient/EEPOT%%20pedestal%%20rms%%20map%%20G12%%20EE%+03d", runnumber, i);
        urls.push_back(s);
        delete[] s;
    }
    return urls;
}
std::vector<ECALHardware::RunData> TestPlugin::analyze(const std::vector<ECALHardware::RunData> &rundata)
{
    return rundata;
}