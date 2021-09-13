/**
 * @file ChannelStatus.hh
 * @author Grigory Latyshev (thegriglat@gmail.com)
 * @brief Channel status filter
 *
 */

#ifndef CHANNELSTATUSFILTER_HH

#include <string>
#include "../ECAL/ECAL.hh"

namespace dqmcpp {
namespace filters {
namespace ChannelStatus {

class ChannelStatus {
 private:
  std::vector<ECAL::ChannelData> _data;

 public:
  /**
   * @brief Construct a new Channel Status object
   *
   * @param run
   */
  ChannelStatus(const ECAL::Run& run);
  /**
   * @brief Construct a new Channel Status object
   *
   * @param run
   * @param dataset
   */
  ChannelStatus(const unsigned int run, const std::string& dataset)
      : ChannelStatus(ECAL::Run(run, dataset)){};
  /**
   * @brief Return true if channel is marked with status >= minStatus
   *
   * @param channel Channel to check
   * @param minStatus Minimal allowed status (-1 is default which means any)
   * @return true
   * @return false
   */
  bool operator()(const ECAL::Channel& channel,
                  const unsigned int minStatus = -1) const;
};

}  // namespace ChannelStatus
}  // namespace filters
}  // namespace dqmcpp

#endif  // !CHANNELSTATUSFILTER_HH