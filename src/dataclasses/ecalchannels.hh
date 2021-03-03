#ifndef ECALCHANNELS_HH

#include "ECALHardware.hh"

/**
 * @brief Static channel list structure
 * Dumped from DQM a long long time ago ...
 */
struct ChannelInfo {
    int fed;
    int tcc;
    int tower;
    int stripintower;
    int xtalinstrip;
    int ccu;
    int vfe;
    int xtalinvfe;
    int xtalinccu;
    int ieta;
    int iphi;
    int ix;
    int iy;
    int iz;
    int hashedid;
    int ic;
    int cmsswid;
    int dbid;
    int ietatt;
    int iphitt;
    int tccch;
    int tccslot;
    int slbch;
    int slbslot;
    int ietagct;
    int iphigct;
    std::string det;
    std::string crate;
};

namespace ECALChannels
{
/**
 * @brief Find channel in static database. Returns nullptr if not found
 *
 * @param channel Channel to be matched
 * @return const ChannelInfo* | nullptr
 */
const ChannelInfo *find(const ECALHardware::Channel &channel);
};

#define ECALCHANNELS_HH
#endif