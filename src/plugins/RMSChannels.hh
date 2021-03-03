#ifndef RMSCHANNELS_HH

#include "Plugin.hh"

class RMSPlugin : public Plugin
{
public:
    std::vector<std::string> urls(const unsigned int runnumber, const std::string &dataset);
    std::vector<ECALHardware::RunData> analyze(const std::vector<ECALHardware::RunData> &rundata);
    void plot(const std::vector<ECALHardware::RunData> &rundata);
};

#define RMSCHANNELS_HH
#endif