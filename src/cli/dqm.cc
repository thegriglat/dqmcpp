#include <iostream>
#include <fstream>
#include <vector>
#include "../readers/JSONReader.hh"
#include "../readers/RunListReader.hh"
#include "../plugins/Plugins.hh"

using namespace std;

int main(int argc, char **argv)
{
    if (argc != 3) {
        std::cout << "Usage: " << argv[0] << " runlist.txt" << std::endl;
        std::cout << "Plugins: " << std::endl;
        for (auto &name : Plugins::list()) {
            std::cout << "  " << name << std::endl;
        }
        return 0;
    }
    auto plugin = Plugins::get(argv[1]);
    if (!plugin) {
        exit(1);
    }
    RunListReader rlr(argv[2]);
    auto reader = new JSONReader();
    int i = 0;
    vector<ECAL::RunData> rundata;
    for (auto &run : rlr.runs()) {
        vector<ECAL::ChannelData> data;
        data.reserve(ECAL::NTotalChannels);
        for (auto &url : plugin->urls(run.runnumber, run.dataset)) {
            cout << url << endl;
            auto data_tt = reader->parse(reader->get(url));
            for (auto &e : data_tt)
                data.push_back(e);
        }
        ECAL::RunData rd(run, data);
        rundata.push_back(rd);
    }
    plugin->plot(plugin->analyze(rundata));
    return 0;
}