#ifndef ECALCHANNELS_HH

#include <string>
#include <vector>

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

class ECALChannels
{
public:
    static ECALChannels &Instance()
    {
        static ECALChannels s;
        return s;
    }

private:
    std::vector<ChannelInfo> _channels;
    ECALChannels()
    {
        Init();
    };
    ~ECALChannels() {};
    void Init();
    ECALChannels(ECALChannels const &) = delete;
    ECALChannels &operator=(ECALChannels const &) = delete;
};

#define ECALCHANNELS_HH
#endif