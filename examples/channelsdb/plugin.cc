#include "plugin.hh"
#include <algorithm>
#include <iostream>
#include <vector>

// Use this header to access ECAL channel database
#include "ecalchannels/ECALChannels.hh"

REGISTER_PLUGIN(ChannelsDBEx, channelsdb)

using namespace std;
using namespace dqmcpp;

namespace dqmcpp {

namespace plugins {

void ChannelsDBEx::Process() {

  /**
   * DQMCpp channels database interface provides just two methods to access
channel db
   *     ECALChannelsList::const_iterator find(const ECAL::Channel& channel);
   * and
   *     std::pair<ECALChannelsList::const_iterator,
ECALChannelsList::const_iterator> list(void);
*/

  // most general way to access channel db is use list() method which returns
  // two iterators like STL begin() and end()

  cout << "=== list() method ===" << endl;

  auto channels = ECALChannels::list();
  /** here channels.begin points to the start of channels list and channels.end
   * then you can use standard C++ STL function to find channels matching
   * condition
   * NOTE: Use det_iz() function to retrieve iz value.
   */

  /**
   * For example we have to find all channels with ix = 43 in EE+
   */

  const int iz = 1; // EE+
  const int ix = 43;

  std::vector<ECALChannels::ChannelInfo> matched_channels;
  std::for_each(channels.begin, channels.end,
                [&matched_channels](const ECALChannels::ChannelInfo &c_info) {
                  // called for every channel
                  if (c_info.det_iz() == iz && c_info.ix == ix)
                    matched_channels.push_back(c_info);
                });

  // print matched channels
  for (auto &channel : matched_channels)
    cout << "(ix == 43 &&  EE+) is [" << channel.ix << ", " << channel.iy
         << ", " << channel.det_iz() << "] with dbid = " << channel.dbid
         << " det = " << channel.det() << endl;

  /**
   * Other way to use channels database is call find() function
   * It tries to find full channel information by channel coordinates
   * This is useful as in most cases plugins operates just channel coordinates
   * and it is needed to get channel info (det, tt, ccu,...) for given
   * coordinates
   */

  cout << "=== find() method ===" << endl;
  const auto channel = ECAL::Channel(43, 12, 1);
  auto full_channel_info = ECALChannels::find(channel);
  cout << "Channel [43,12,1] is in " << full_channel_info->det() << endl;

  // Channel() construction can be inlined in find() call
  // See ECAL::Channel type definition
  cout << "Channel [43,80,1] is in " << ECALChannels::find({43, 80, 1})->det()
       << endl;
}
} // namespace plugins

} // namespace dqmcpp
