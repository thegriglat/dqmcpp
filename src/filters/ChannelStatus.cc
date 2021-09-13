/**
 * @file ChannelStatus.cc
 * @author Grigory Latyshev (thegriglat@gmail.com)
 * @brief Channel status filter
 */

#include "ChannelStatus.hh"
#include <algorithm>
#include <string>
#include "../ECAL/ECAL.hh"
#include "../net/DQMURLProvider.hh"
#include "../readers/JSONReader.hh"

using namespace std;
using namespace dqmcpp;

namespace {

string geturl(const ECAL::Run& run, const int iz) {
  string url;
  switch (iz) {
    case -1:
      url = "EcalEndcap/EEIntegrityClient/EEIT EE - channel status map";
      break;
    case 0:
      url = "EcalBarrel/EBIntegrityClient/EBIT channel status map";
      break;
    case 1:
      url = "EcalEndcap/EEIntegrityClient/EEIT EE + channel status map";
      break;
  }
  return net::DQMURL::dqmurl(run, url);
}

std::vector<ECAL::ChannelData> getChannelStatus(const ECAL::Run& run,
                                                const int iz) {
  const string url = geturl(run, iz);
  if (iz != 0) {
    const auto content =
        readers::JSONReader::parse(readers::JSONReader::get(url));
    return content;
  }
  // for EB we have to transform Data2D to ChannelData as DQM rotates
  // histogram
  std::vector<ECAL::ChannelData> cd;
  const auto content =
      readers::JSONReader::parse2D(readers::JSONReader::get(url));
  cd.reserve(content.size());

  std::transform(
      content.begin(), content.end(), std::back_inserter(cd),
      [](const ECAL::Data2D& elem) { return ECAL::Data2D2Channel(elem); });
  return cd;
}

}  // namespace

namespace dqmcpp {
namespace filters {

ChannelStatus::ChannelStatus(const ECAL::Run& run) {
  _data.clear();
  for (int i = -1; i <= 1; ++i) {
    const auto status = getChannelStatus(run, i);
    _data.insert(_data.end(), status.begin(), status.end());
  };
}

bool ChannelStatus::operator()(const ECAL::Channel& channel,
                               const int minStatus) const {
  return std::any_of(_data.begin(), _data.end(),
                     [&channel, minStatus](const ECAL::ChannelData& cd) {
                       return cd.base == channel && cd.value >= minStatus;
                     });
};

bool ChannelStatus::operator()(const ECAL::Channel& channel) const {
  return std::any_of(
      _data.begin(), _data.end(),
      [&channel](const ECAL::ChannelData& cd) { return cd.base == channel; });
};

}  // namespace filters
}  // namespace dqmcpp
