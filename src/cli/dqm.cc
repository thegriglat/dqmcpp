#include <iostream>
#include <fstream>
#include <vector>
#include "../plugins/RMSChannels.hh"
#include "../plugins/TestPlugin.hh"
#include "../readers/JSONReader.hh"
#include "../readers/RunListReader.hh"

#include "../dataclasses/ecalchannels.hh"

using namespace std;

int main(int argc, char **argv)
{
    if (argc != 2) {
        std::cout << "Usage: " << argv[0] << " runlist.txt" << std::endl;
        return 0;
    }
    RunListReader rlr(argv[1]);
    auto plugin = new RMSPlugin();
    auto reader = new JSONReader();
    int i = 0;
    vector<ECALHardware::RunData> rundata;
    for (auto &run : rlr.runs()) {
        vector<ECALHardware::ChannelData> data;
        data.reserve(ECALHardware::NTotalChannels);
        for (auto &url : plugin->urls(run.runnumber, run.dataset)) {
            cout << url << endl;
            auto data_tt = reader->parse(reader->get(url));
            for (auto &e : data_tt)
                data.push_back(e);
        }
        ECALHardware::RunData rd (run, data);
        rundata.push_back(rd);
    }
    plugin->plot(plugin->analyze(rundata));
    return 0;
}