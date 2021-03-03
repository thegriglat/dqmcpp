#ifndef ECALHARDWARE_HH

#include <string>
#include <vector>
#include <iostream>

namespace ECALHardware
{
constexpr unsigned int NEBChannels = 61200;
constexpr unsigned int NEEChannels = 14648;
constexpr unsigned int NTotalChannels = NEBChannels + NEEChannels;
/**
 * @brief Explicit enum of ECAL parts
 * Usually shouldn't be casted to int
 */
enum DETECTORS {
    EB = 0,
    EEPLUS = 1,
    EEMINUS = -1
};
/**
* @brief Basic channel struct
* Trivial constructor is disabled
*/
struct Channel {
    // unsigned long channel_id;
    int ix_iphi;
    int iy_ieta;
    DETECTORS iz;
    Channel(int x, int y, DETECTORS z) : ix_iphi(x), iy_ieta(y), iz(z) {};
    friend inline std::ostream &operator<<(std::ostream &os, const Channel &c)
    {
        os << "{x: " << c.ix_iphi << ", y: " << c.iy_ieta << ", z:" << c.iz << "}";
        return os;
    }

    friend inline bool operator==(const Channel &a, const Channel &b)
    {
        return a.ix_iphi == b.ix_iphi && a.iy_ieta == b.iy_ieta && a.iz == b.iz;
    }
};
/**
* @brief Basic channel data struct
* Trivial constructor is disabled
*/
struct ChannelData {
    Channel channel;
    double value;
    ChannelData(Channel _channel, double _value) : channel(_channel), value(_value) {};
    friend inline std::ostream &operator<<(std::ostream &os, const ChannelData &cd)
    {
        os << cd.channel << " value: " << cd.value;
        return os;
    }
};
/**
* @brief Basic run information
* Trivial constructor is disabled
*/
struct Run {
    unsigned int runnumber;
    std::string dataset;
    Run(unsigned int _run, std::string _dataset) : runnumber(_run), dataset(_dataset) {};
};
/**
 * @brief Run data struct.
 * Trivial constructor is disabled
 */
struct RunData {
    Run run;
    std::vector<ChannelData> channeldata;
    RunData(Run _run, std::vector<ChannelData> _cd) : run(_run), channeldata(_cd) {};
};
};
#define ECALHARDWARE_HH
#endif