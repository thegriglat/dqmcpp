/**
 * @file ChannelStatus.hh
 * @author Grigory Latyshev (thegriglat@gmail.com)
 * @brief Channel status filter
 *
 */

#ifndef CHANNELSTATUSFILTER_HH
#define CHANNELSTATUSFILTER_HH

#include <string>
#include "../ECAL/ECAL.hh"

namespace dqmcpp {
namespace filters {

/**
 * @brief Channel status filter
 *
 */
class ChannelStatus {
 private:
  std::vector<ECAL::ChannelData> _data;

 public:
  /**
   * @brief Construct a new Channel Status filter object
   *
   * @param run Reference run as ECAL::Run
   */
  ChannelStatus(const ECAL::Run& run);
  /**
   * @brief Construct a new Channel Status filter object
   *
   * @param run Reference run number
   * @param dataset Dataset for reference runnumber
   */
  ChannelStatus(const int run, const std::string& dataset)
      : ChannelStatus(ECAL::Run(run, dataset)){};
  /**
   * @brief Return true if channel is marked with status >= minStatus
   *
   * @param channel Channel to check
   * @param minStatus Minimal allowed status (-1 is default which means any)
   * @return true
   * @return false
   */
  bool operator()(const ECAL::Channel& channel, const int minStatus) const;
  /**
   * @brief Return true if channel is marked with **any** status
   *
   * @param channel Channel to check
   * @return true
   * @return false
   */
  bool operator()(const ECAL::Channel& channel) const;
};

}  // namespace filters
}  // namespace dqmcpp

#endif  // !CHANNELSTATUSFILTER_HH