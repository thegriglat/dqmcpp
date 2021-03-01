#include <iostream>
#include <fstream>
#include <vector>
#include "../plugins/TestPlugin.hh"
#include "../readers/JSONReader.hh"

using namespace std;

int main()
{
    std::cout << "At the moment it is just dummy file " << std::endl;
    auto plugin = new TestPlugin();
    auto reader = new JSONReader();
    auto runnumber = 315257;

    int i = 0;
    vector<ECALHardware::ChannelData> data;
    data.reserve(ECALHardware::NTotalChannels);
    for (auto &url : plugin->urls(runnumber, "")) {
        cout << url << endl;
        auto data_tt = reader->parse(reader->get(url));
        for (auto &e : data_tt)
            data.push_back(e);
    }
    ECALHardware::RunData rd;
    rd.channeldata = data;
    rd.run.runnumber = runnumber;
    rd.run.dataset = "";
    vector<ECALHardware::RunData> rundata = {rd};
    rundata = plugin->analyze(rundata);
}