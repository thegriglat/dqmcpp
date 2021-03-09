#include "ecalchannels.hh"
#include <algorithm>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include "../common/common.hh"
#include "../common/logging.hh"
using namespace std;

// symbols from dumped ecalchannels.o
extern "C" char _binary_ecalchannels_dat_start;
extern "C" char _binary_ecalchannels_dat_end;

static std::vector<ChannelInfo> _channels;
static bool isInit = false;
static void Init();

static int* getPtr(const ChannelInfo& info, const int n) {
  return (int*)(&info) + n;
};

void Init() {
  if (isInit)
    return;
  _channels.reserve(75848);
  // read text from ecalchannels.o to stringstream
  std::stringstream in;
  char* p = &_binary_ecalchannels_dat_start;
  while (p != &_binary_ecalchannels_dat_end) {
    // just insert byte-per-byte
    in << *p++;
  }
  string line;
  while (!in.eof()) {
    std::getline(in, line);
    ChannelInfo info;
    const auto stringlist = split(line, ",");
    for (int n = 0; n < stringlist.size(); n++) {
      auto token = stringlist.at(n);
      if (n < 26) {
        int* intptr = getPtr(info, n);
        *(intptr) = atoi(token.c_str());
      } else {
        if (n == 26)
          info.det = token;
        else
          info.crate = token;
      };
    }
    _channels.push_back(info);
  }
  isInit = true;
};
namespace ECALChannels {
const ChannelInfo* find(const ECAL::Channel& channel) {
  Init();
  auto it = std::find_if(
      _channels.begin(), _channels.end(), [channel](const ChannelInfo& info) {
        if (channel.iz == ECAL::DETECTORS::EB) {
          // barrel
          return channel.ix_iphi == info.iphi && channel.iy_ieta == info.ieta;
        }
        int iz = 1;
        if (channel.iz == ECAL::DETECTORS::EEMINUS) {
          iz = -1;
        }
        return channel.ix_iphi == info.ix && channel.iy_ieta == info.iy &&
               iz == info.iz;
      });
  if (it != _channels.end()) {
    return &(*it);
  }
  std::cerr << "channel not found!" << std::endl;
  std::cerr << "====" << std::endl
            << "ix_iphi = " << channel.ix_iphi
            << " iy_ieta = " << channel.iy_ieta << " iz = " << channel.iz
            << std::endl
            << "====" << std::endl;
  return nullptr;
}

const std::vector<ChannelInfo> list() {
  Init();
  return _channels;
}

const std::string detByTTTTC(const int tt, const int tcc) {
  Init();
  auto it = std::find_if(_channels.begin(), _channels.end(),
                         [tt, tcc](const ChannelInfo& c) {
                           return c.tower == tt && c.tcc == tcc;
                         });
  if (it == _channels.end())
    return "";
  return it->det;
}
};  // namespace ECALChannels