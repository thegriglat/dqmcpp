#ifndef ECALHARDWARE_HH

#include <string>

namespace ECALHardware
{
    constexpr unsigned int NEBChannels = 61200;
    constexpr unsigned int NEEChannels = 14648;
    constexpr unsigned int NTotalChannels = NEBChannels + NEEChannels;
    enum DETECTORS
    {
        EB = 0,
        EEPLUS = 1,
        EEMINUS = -1
    };
    /**
     * @brief Basic channel struct
     * 
     */
    struct Channel
    {
        unsigned long channel_id;
        DETECTORS iz;
        int ix_iphi;
        int iy_ieta;
    };
    /**
     * @brief Basic channel data struct
     * 
     */
    struct ChannelData
    {
        Channel channel;
        double value;
    };
    /**
     * @brief Basic run information
     * 
     */
    struct Run
    {
        unsigned int runnumber;
        std::string dataset;
    };
};
#define ECALHARDWARE_HH
#endif