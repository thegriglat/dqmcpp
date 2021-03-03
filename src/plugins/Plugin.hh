#ifndef PLUGIN_HH

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
     * @param runnumber Runnumber
     * @param dataset Dataset to be used or "online" for online DQM
     * @return std::vector<std::string>
     */
    virtual std::vector<std::string> urls(const unsigned int runnumber, const std::string &dataset) = 0;
    /**
     * @brief Does really plugin analyze and transform channel data for plot() function
     *
     * @param rundata RunData struct with channels data. To be passed in plot() method.
     * @return std::vector<ECALHardware::RunData>
     */
    virtual std::vector<ECALHardware::RunData> analyze(const std::vector<ECALHardware::RunData> &rundata) = 0;
    /**
     * @brief Plot channels data as plugin wants
     *
     * @param rundata Channel data returned in analyze() method
     */
    virtual void plot(const std::vector<ECALHardware::RunData> &rundata) = 0;
};

#define PLUGIN_HH
#endif