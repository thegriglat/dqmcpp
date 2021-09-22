#ifndef ECALCHANNELS_HH
/**
 * @file ECALChannels.hh
 * @author Grigory Latyshev (thegriglat@gmail.com)
 * @brief Access to ECAL channels database
 */
#include <array>
#include <string>
#include <utility>
#include "../ECAL/ECAL.hh"
#include "ChannelInfo.hh"

namespace dqmcpp {
/**
 * @brief Access to ECAL channels database
 *
 */
namespace ECALChannels {

using ECALChannelsList = std::array<dqmcpp::ECALChannels::ChannelInfo, 75848>;

/**
 * @brief Iterator structure to use in std:: algorithm function
 *
 */
struct ChannelIteratorPair {
  ECALChannelsList::const_iterator begin;
  ECALChannelsList::const_iterator end;
};

/**
 * @brief Find first matching channel in database. Returns nullptr if not found.
 *
 * @param channel Channel to be matched
 * @return const ChannelInfo* | nullptr
 */
ECALChannelsList::const_iterator find(const ECAL::Channel& channel);

/**
 * @brief Returns begin() and end() iterator pair
 *
 * @return ChannelIteratorPair
 */
ChannelIteratorPair list(void);

}  // namespace ECALChannels
}  // namespace dqmcpp
#define ECALCHANNELS_HH
#endif