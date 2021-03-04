#include "TTF4Plugin.hh"

#include <vector>
#include <string>
#include <iostream>
#include "../readers/DQMURLProvider.hh"

using namespace std;
namespace
{
struct URLType {
    std::string url;
    bool isEB = false;
    URLType(const std::string _s, bool eb): url(_s), isEB(eb) {};
};

std::vector<URLType> urls(const unsigned int runnumber, const std::string &dataset)
{
    std::vector<URLType> urls;
    const std::string plot = "EcalBarrel/EBTriggerTowerTask/EBTTT TT Masking Status";
    urls.push_back(URLType(
                       DQMURL::dqmurl(runnumber, dataset, plot), true));
    for (int i = 0; i < 2; ++i) {
        string eeplot = plot;
        eeplot += (i == 0) ? " EE +" : " EE -";
        urls.push_back(URLType(
                           DQMURL::dqmurl(runnumber, dataset, eeplot), false));
    }
    return urls;
}

}
void TTF4Plugin::Process()
{
    for (auto r: runListReader->runs()) {
        for (auto url: urls(r.runnumber, r.dataset)) {
            cout << "[" << ((url.isEB) ? "EB" : "EE") << "] " << url.url << endl;
        }
    }
}