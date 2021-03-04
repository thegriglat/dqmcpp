#ifndef RMSCHANNELS_HH

#include "Plugin.hh"

class RMSPlugin : public Plugin
{
public:
    std::vector<std::string> urls(const unsigned int runnumber, const std::string &dataset);
    std::vector<ECAL::RunData> analyze(const std::vector<ECAL::RunData> &rundata);
    void plot(const std::vector<ECAL::RunData> &rundata);
};

#define RMSCHANNELS_HH
#endif