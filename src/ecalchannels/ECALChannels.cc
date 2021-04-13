/**
 * @file ECALChannels.cc
 * @author Grigory Latyshev (thegriglat@gmail.com)
 * @brief ECAL channels database functions
 */
#include "ECALChannels.hh"
#include <algorithm>
#include <array>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include "../common/common.hh"
#include "../common/logging.hh"
#include "../common/string.hh"
#include "ecalchannels_db.hh"

using namespace std;

namespace {

using namespace dqmcpp;
const dqmcpp::ECALChannels::ChannelInfo* infoeb(const int tt, const int tcc) {
  const auto channels = ECALChannels::ChannelsDB::channels();
  auto it = std::find_if(channels->begin(), channels->end(),
                         [tt, tcc](const ECALChannels::ChannelInfo& c) {
                           return c.tower == tt && c.tcc == tcc;
                         });
  if (it != channels->end())
    return &(*it);
  return nullptr;
}

const dqmcpp::ECALChannels::ChannelInfo* infoee(const int ccu, const int tcc) {
  const auto channels = ECALChannels::ChannelsDB::channels();
  auto it = std::find_if(channels->begin(), channels->end(),
                         [ccu, tcc](const ECALChannels::ChannelInfo& c) {
                           return c.ccu == ccu && c.tcc == tcc;
                         });
  if (it != channels->end())
    return &(*it);
  return nullptr;
}
}  // namespace

namespace dqmcpp {
namespace ECALChannels {
const ChannelInfo* find(const ECAL::Channel& c) {
  return ChannelsDB::find(c.ix_iphi, c.iy_ieta, c.iz);
};

const ECALChannelsList* list() {
  return ChannelsDB::channels();
}

const std::string detByTTTTC(const int tt, const int tcc) {
  auto i = infoeb(tt, tcc);
  if (i)
    return i->det();
  return "";
}

const std::string detByCCUTCC(const int ccu, const int tcc) {
  auto i = infoee(ccu, tcc);
  if (i)
    return i->det();
  return "";
}

int ccu(const ECAL::TT& tt) {
  auto i = infoeb(tt.tt, tt.tcc);
  if (i)
    return i->ccu;
  return 0;
}

int ChannelInfo::sm() const {
  if (fed >= 610 && fed <= 627)
    return 610 - 1 - fed;
  if (fed >= 628 && fed <= 645)
    return fed - 628 + 1;

  if (fed >= 601 && fed <= 609)
    return (fed <= 603) ? (594 - fed) : (603 - fed);

  if (fed >= 646 && fed <= 654)
    return (fed <= 648) ? (fed - 639) : (fed - 648);
  return 0;
}

std::string ChannelInfo::part() const {
  if (fed >= 610 && fed <= 645)
    return "EB";
  return "EE";
}

std::string ChannelInfo::det() const {
  return common::string_format("%s%+02d", part().c_str(), sm());
}

}  // namespace ECALChannels
}  // namespace dqmcpp