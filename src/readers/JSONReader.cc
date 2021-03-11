/**
 * @file JSONReader.cc
 * @author Grigory Latyshev (thegriglat@gmail.com)
 * @brief JSONReader code
 * @todo join parse and parse2D functions
 */

#include "JSONReader.hh"
#include <cmath>
#include <vector>
#include "../common/logging.hh"

namespace {
using namespace dqmcpp::ECAL;
/**
 * @brief Get iz of ECAL from histo title
 *
 * @param title
 */
DETECTORS getECALDetector(const std::string& title) {
  if (title.find("EB") != title.npos) {
    return DETECTORS::EB;
  }
  if (title.find("EE+") != title.npos || title.find("EE +") != title.npos) {
    return DETECTORS::EEPLUS;
  }
  if (title.find("EE-") != title.npos || title.find("EE -") != title.npos) {
    return DETECTORS::EEMINUS;
  }
  WARNING("Cannot determine ECAL iz from title");
  return DETECTORS::EB;
}

/**
 * @brief Check that DQM json is valid. Just check that some data returned.
 * No "smart" checks performed.
 * @param j Parsed HTTP response content
 * @return true
 * @return false
 */
bool isValid(nlohmann::json& j) {
  const auto hist = j["hist"];
  // {"hist": "unsupported type"}
  if (hist.empty() || hist.is_string())
    return false;
  return true;
}

}  // namespace

namespace dqmcpp {
namespace readers {

nlohmann::json JSONReader::parseJSON(const std::string& content) {
  auto j = nlohmann::json::parse(content);
  return j;
}

std::vector<ECAL::ChannelData> JSONReader::parse(nlohmann::json& j) {
  using namespace std;
  // check validity of json
  const auto data2d = parse2D(j);
  if (data2d.size() == 0)
    return std::vector<ECAL::ChannelData>();
  const auto iz = getECALDetector(j["hist"]["title"].get<string>());
  vector<ECAL::ChannelData> channel_data;
  const auto xaxis = j["hist"]["xaxis"];
  const auto xtitle = xaxis["title"].get<string>();
  const char xsign = (xtitle.find('-') != xtitle.npos) ? -1 : 1;
  for (auto& datapoint : data2d) {
    const int x = std::trunc(datapoint.x);
    const int y = std::trunc(datapoint.y);
    int ix_iphi;
    int iy_ieta;
    if (iz == ECAL::DETECTORS::EB) {
      // EB
      // in case of barrel we have to swap x and y values
      ix_iphi = std::abs(y) + 1;
      iy_ieta = (x + 1) * xsign;
    } else {
      // EE
      ix_iphi = x + 1;
      iy_ieta = y + 1;
    }
    ECAL::Channel c(ix_iphi, iy_ieta, iz);
    channel_data.push_back(ECAL::ChannelData(c, datapoint.value));
  }
  return channel_data;
}

std::vector<ECAL::Data2D> JSONReader::parse2D(nlohmann::json& j) {
  // TODO:: make it common with parse ..
  using namespace std;
  using BinContentList = vector<vector<double>>;
  // check validity of json
  if (!isValid(j))
    return std::vector<ECAL::Data2D>();
  const auto xaxis = j["hist"]["xaxis"];
  const auto yaxis = j["hist"]["yaxis"];
  const auto xnbins =
      xaxis["last"]["id"].get<int>() - xaxis["first"]["id"].get<int>() + 1;
  const auto xfirst = xaxis["first"]["value"].get<double>();
  const auto xlast = xaxis["last"]["value"].get<double>();
  const auto xstep = (xlast - xfirst) / xnbins;

  const auto ynbins =
      yaxis["last"]["id"].get<int>() - yaxis["first"]["id"].get<int>() + 1;
  const auto yfirst = yaxis["first"]["value"].get<double>();
  const auto ylast = yaxis["last"]["value"].get<double>();
  const auto ystep = (ylast - yfirst) / ynbins;

  // bin increment always == 1 as it is channel
  const auto content = j["hist"]["bins"]["content"].get<BinContentList>();
  std::vector<ECAL::Data2D> channel_data;
  for (int ybin = 0; ybin < ynbins; ++ybin) {
    double y = yfirst + ystep * 0.5 + ystep * ybin;
    for (int xbin = 0; xbin < xnbins; ++xbin) {
      double x = xfirst + xstep * 0.5 + xstep * xbin;
      const auto value = content.at(ybin).at(xbin);
      ECAL::Data2D c(x, y, value);
      if (value != 0) {
        // to avoid overlapping DQM output for different SM
        channel_data.push_back(c);
      }
    }
  };
  return channel_data;
}

}  // namespace readers
}  // namespace dqmcpp