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

#include "ecalchannels_db.hh"

using namespace std;

namespace dqmcpp {
namespace ECALChannels {
const ChannelInfo* find(const ECAL::Channel& channel) {
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

const ECALChannelsList list() {
  return _channels;
}

const std::string detByTTTTC(const int tt, const int tcc) {
  auto it = std::find_if(_channels.begin(), _channels.end(),
                         [tt, tcc](const ChannelInfo& c) {
                           return c.tower == tt && c.tcc == tcc;
                         });
  if (it == _channels.end())
    return "";
  return it->det();
}

std::string ChannelInfo::det() const {
  switch (fed) {
    case 601:
      return "EE-7";
    case 602:
      return "EE-08";
    case 603:
      return "EE-09";
    case 604:
      return "EE-01";
    case 605:
      return "EE-02";
    case 606:
      return "EE-03";
    case 607:
      return "EE-04";
    case 608:
      return "EE-05";
    case 609:
      return "EE-06";
    case 610:
      return "EB-01";
    case 611:
      return "EB-02";
    case 612:
      return "EB-03";
    case 613:
      return "EB-04";
    case 614:
      return "EB-05";
    case 615:
      return "EB-06";
    case 616:
      return "EB-07";
    case 617:
      return "EB-08";
    case 618:
      return "EB-09";
    case 619:
      return "EB-10";
    case 620:
      return "EB-11";
    case 621:
      return "EB-12";
    case 622:
      return "EB-13";
    case 623:
      return "EB-14";
    case 624:
      return "EB-15";
    case 625:
      return "EB-16";
    case 626:
      return "EB-17";
    case 627:
      return "EB-18";
    case 628:
      return "EB+01";
    case 629:
      return "EB+02";
    case 630:
      return "EB+03";
    case 631:
      return "EB+04";
    case 632:
      return "EB+05";
    case 633:
      return "EB+06";
    case 634:
      return "EB+07";
    case 635:
      return "EB+08";
    case 636:
      return "EB+09";
    case 637:
      return "EB+10";
    case 638:
      return "EB+11";
    case 639:
      return "EB+12";
    case 640:
      return "EB+13";
    case 641:
      return "EB+14";
    case 642:
      return "EB+15";
    case 643:
      return "EB+16";
    case 644:
      return "EB+17";
    case 645:
      return "EB+18";
    case 646:
      return "EE+07";
    case 647:
      return "EE+08";
    case 648:
      return "EE+09";
    case 649:
      return "EE+01";
    case 650:
      return "EE+02";
    case 651:
      return "EE+03";
    case 652:
      return "EE+04";
    case 653:
      return "EE+05";
    case 654:
      return "EE+06";
  }
  return "unknown SM";
}

}  // namespace ECALChannels
}  // namespace dqmcpp