#ifndef ECALCHANNELS_DB_HH
/**
 * @file ecalchannels_db.hh
 * @author Grigory Latyshev (thegriglat@gmail.com)
 * @brief Channel database functions
 *
 *
 */
#include "ECALChannels.hh"

namespace dqmcpp {
namespace ECALChannels {

/**
 * @brief Namespace to operate with in-memory ECAL channels database
 *
 */
namespace ChannelsDB {
/**
 * @brief Raw pointer to channel database
 *
 * @return const ECALChannelsList*
 */
const ECALChannelsList* channels();
/**
 * @brief Find ECAL channel by coordinates
 *
 * @param ix_iphi ix (EE +/-) or iphi (EB) coordinate
 * @param iy_ieta iy (EE +/-) or ieta (EB) coordinate
 * @param iz 0 for EB, +1 for EE+, -1 for EE-
 * @return const ChannelInfo*
 */
const ChannelInfo* find(const int ix_iphi, const int iy_ieta, const int iz);

/**
 * @brief Find ECAL channel by ECAL::Channel object
 *
 * @param c ECAL channel
 * @return const ChannelInfo*
 */
inline const ChannelInfo* find(const ECAL::Channel& c) {
  return find(c.ix_iphi, c.iy_ieta, c.iz);
};
};  // namespace ChannelsDB

}  // namespace ECALChannels
}  // namespace dqmcpp

#define ECALCHANNELS_DB_HH
#endif