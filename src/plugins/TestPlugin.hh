#ifndef TESTPLUGIN_HH

#include "PluginInterface.hh"

class TestPlugin : public Plugin
{
public:
    std::vector<std::string> urls(const unsigned int runnumber, const std::string &dataset);
    std::vector<ECALHardware::RunData> analyze(const std::vector<ECALHardware::RunData> &rundata);
    void plot(const std::vector<ECALHardware::RunData> &rundata);
};

#define TESTPLUGIN_HH
#endif