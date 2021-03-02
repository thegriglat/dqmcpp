#ifndef PLUGININTERFACE_HH

#include <vector>
#include <string>
#include "../dataclasses/ECALHardware.hh"

/**
 * @brief Virtual class for plugins (tests)
 *
 */
class Plugin
{
public:
    /**
     * @brief Return list of URLs with data
     *
     * @param runnumber
     * @param dataset
     * @return std::vector<std::string>
     */
    virtual std::vector<std::string> urls(const unsigned int runnumber, const std::string &dataset) = 0;
    virtual std::vector<ECALHardware::RunData> analyze(const std::vector<ECALHardware::RunData> &rundata) = 0;
    virtual void plot(const std::vector<ECALHardware::RunData> &rundata) = 0;
};

#define PLUGININTERFACE_HH
#endif