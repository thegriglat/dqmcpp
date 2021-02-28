#include <iostream>

#include "../readers/JSONReader.hh"
#include <fstream>

int main()
{
    std::cout << "At the moment it is just dummy file " << std::endl;
    JSONReader reader;
    for (int i = -18; i < 18; i++) {
        if (i == 0)
            continue;
        char *s = new char[512];
        sprintf(s, "https://cmsweb.cern.ch/dqm/offline/jsonfairy/archive/315257/EGamma/Run2018A-12Nov2019_UL2018-v2/DQMIO/EcalBarrel/EBPedestalOnlineClient/EBPOT%%20pedestal%%20rms%%20map%%20G12%%20EB%+03d", i);
        std::string url(s);
        std::cout << url << std::endl;
        const auto j = JSONReader::parseJSON(reader.get(url));
        std::cout << j["hist"]["title"].get<std::string>() << std::endl;
        delete[] s;
    }
}