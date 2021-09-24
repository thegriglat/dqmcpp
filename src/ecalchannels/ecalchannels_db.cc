/**
 * @file ecalchannels_db.cc
 * @author Grigory Latyshev (thegriglat@gmail.com)
 * @brief
 *
 *
 */
#include "ecalchannels_db.hh"

#include <algorithm>

#include "../common/lists.hh"
#include "../common/math.hh"
#include "ecalchannels_db.cppt"

#define SORTBY(A, B, X, C) \
  if (A.X == B.X) {        \
    C;                     \
  } else {                 \
    return (A.X < B.X);    \
  }

namespace {

int sortCh(const dqmcpp::ECALChannels::ChannelInfo& a,
           const dqmcpp::ECALChannels::ChannelInfo& b) {
  /*
  sort by z
  iphi
  ieta
  ix
  iy
  */
  SORTBY(a, b, iz,
         SORTBY(a, b, iphi,
                SORTBY(a, b, ieta,
                       SORTBY(a, b, ix, SORTBY(a, b, iy, return false)))));
}

bool isInit = false;
void Init() {
  if (isInit)
    return;
  std::sort(_channels.begin(), _channels.end(), sortCh);
  isInit = true;
}
}  // namespace

namespace dqmcpp {
namespace ECALChannels {

namespace ChannelsDB {

const ECALChannelsList* channels() {
  Init();
  return &_channels;
}

const ChannelInfo* find(const int ix_iphi, const int iy_ieta, const int iz) {
  Init();
  ChannelInfo key;
  if (iz == 0) {
    key.iz = common::sign(iy_ieta);
    key.ix = -999;
    key.iy = -999;
    key.iphi = ix_iphi;
    key.ieta = iy_ieta;
  } else {
    key.ix = ix_iphi;
    key.iy = iy_ieta;
    key.iphi = -999;
    key.ieta = -999;
    key.iz = iz;
  }
  auto ptr =
      common::binary_search(_channels.begin(), _channels.end(), key, sortCh);
  if (ptr != _channels.end())
    return ptr;
  return nullptr;
}
};  // namespace ChannelsDB

}  // namespace ECALChannels
}  // namespace dqmcpp
