#include "plugin.hh"
#include <iostream>

#include "common/common.hh"
#include "net/DQMURLProvider.hh"
#include "readers/JSONReader.hh"

/**
 * @brief Reading channels example
 * In this examples we will read dqmcpp::ECAL::Channel list from DQM
 * plot: EcalBarrel/EBOccupancyTask/EBOT rec hit occupancy, DQM Online, run
 * 321396
 */

REGISTER_PLUGIN(ReadingChannels, reading_channels)

using namespace std;
using namespace dqmcpp;

namespace dqmcpp {

namespace plugins {

void ReadingChannels::Process() {
  const int run = 321396;
  const auto dataset = "online";
  const auto path_sm = "EcalBarrel/EBOccupancyTask/EBOT digi occupancy EB-17";
  const auto url_sm = net::DQMURL::dqmurl(run, dataset, path_sm);
  cout << "Run: " << run << " dataset: " << dataset << " DQMURL: " << url_sm
       << endl;
  // getting DQM json
  auto json_sm = readers::JSONReader::get(url_sm);
  // parse it to std::vector<ECAL::Channel> for EB-17
  auto data_sm = readers::JSONReader::parse(json_sm);

  // iterate over data to see what is inside

  cout << "===== START EB-17 OUTPUT =====" << endl;
  for (auto& element : data_sm) {
    // using overloaded operator<<() for ECAL::Channel
    cout << element << endl;
  }

  cout << "===== END EB-17 OUTPUT =====" << endl;
  /**
   * For entire EB/EE-/EE+ it's needed to use JSONReader::parse2D()
   * as DQM rotates histogram... (swap X and Y axes)
   *
   */

  const auto url_eb = net::DQMURL::dqmurl(
      run, dataset, "EcalBarrel/EBOccupancyTask/EBOT rec hit occupancy");
  /**
   * Here data_eb is ECAL::Data2D vector.
   * Note that Point2D has coordinate of center of bin.
   * Use ECAL::Data2D2Channel() to convert Data2D to ECAL::ChannelData
   */
  auto data_eb = readers::JSONReader::parse2D(readers::JSONReader::get(url_eb));

  cout << "===== START EB OUTPUT =====" << endl;
  /**
   * Let's do something "useful"...
   * Drop all points with value less mean
   */
  const auto mean = common::mean(data_eb, [](const ECAL::Data2D& d2d) {
    // function to get double from custom type (Data2D in that case)
    return d2d.value;
  });
  cout << "data_eb size (before filter) = " << data_eb.size() << endl;
  data_eb = common::filter(data_eb, [mean](const ECAL::Data2D& d2d) {
    // keep only points with value > mean
    return d2d.value > mean;
  });
  cout << "data_eb size (after filter) = " << data_eb.size() << endl;

  /**
   * At my run it was:
   *   data_eb size (before filter) = 60838
   *   data_eb size (after filter) = 26941
   */
  for (auto& element : data_eb) {
    // 0 is ECAL Barrel (iz == 0)
    auto channel = ECAL::Data2D2Channel(element, 0);
    cout << "mean: " << mean << " Data2D: " << element
         << "  |  Channel:" << channel << endl;
  }
  cout << "===== END EB OUTPUT =====" << endl;
}
}  // namespace plugins

}  // namespace dqmcpp
