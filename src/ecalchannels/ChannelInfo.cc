/**
 * @file ChannelInfo.cc
 * @author Grigory Latyshev (thegriglat@gmail.com)
 * @brief ChannelInfo struct functions
 *
 */
#include "ChannelInfo.hh"
#include <string>
#include "../common/string.hh"

namespace dqmcpp {
namespace ECALChannels {

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

int ChannelInfo::det_iz() const {
  if (fed >= 610 && fed <= 645)
    return 0;
  if (iz > 0)
    return 1;
  return -1;
}

std::string ChannelInfo::part() const {
  if (det_iz() == 0)
    return "EB";
  return "EE";
}

std::string ChannelInfo::det() const {
  return dqmcpp::common::string_format("%s%+03d", part().c_str(), sm());
}
}  // namespace ECALChannels
}  // namespace dqmcpp