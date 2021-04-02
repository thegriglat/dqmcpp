/**
 * @file JSONReader.cc
 * @author Grigory Latyshev (thegriglat@gmail.com)
 * @brief JSONReader code
 * @todo join parse and parse2D functions
 */

#include "JSONReader.hh"
#include <cmath>
#include <vector>
#include "../common/JSONParser.hh"
#include "../common/json.hh"
#include "../common/logging.hh"
#include "../common/math.hh"
#include "../net/URLCache.hh"

namespace {
using namespace dqmcpp::ECAL;
using namespace dqmcpp;
/**
 * @brief Get iz of ECAL from histo title
 *
 * @param title
 */
int getECALDetector(const std::string& title) {
  if (title.find("EB") != title.npos) {
    return 0;
  }
  if (title.find("EE+") != title.npos || title.find("EE +") != title.npos) {
    return +1;
  }
  if (title.find("EE-") != title.npos || title.find("EE -") != title.npos) {
    return -1;
  }
  WARNING("Cannot determine ECAL iz from title");
  return 0;
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

std::vector<ECAL::Data2D> parse2D(nlohmann::json& j, bool skipZeroes = true) {
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
      const bool skip = common::isZero(value) && skipZeroes;
      if (!skip) {
        // to avoid overlapping DQM output for different SM
        channel_data.push_back(c);
      }
    }
  };
  return channel_data;
}

std::vector<dqmcpp::ECAL::ChannelData> parse(nlohmann::json& j) {
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
    if (iz == 0) {
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

std::vector<ECAL::Data1D> parse1D(nlohmann::json& j, bool skipZeroes) {
  // TODO:: make it common with parse ..
  using namespace std;
  using BinContentList = vector<double>;
  // check validity of json
  if (!isValid(j))
    return std::vector<ECAL::Data1D>();
  const auto xaxis = j["hist"]["xaxis"];
  const auto yaxis = j["hist"]["yaxis"];
  const auto xnbins =
      xaxis["last"]["id"].get<int>() - xaxis["first"]["id"].get<int>() + 1;
  const auto xfirst = xaxis["first"]["value"].get<double>();
  const auto xlast = xaxis["last"]["value"].get<double>();
  const auto xstep = (xlast - xfirst) / xnbins;

  // bin increment always == 1 as it is channel
  const auto content = j["hist"]["bins"]["content"].get<BinContentList>();
  std::vector<ECAL::Data1D> channel_data;
  for (int xbin = 0; xbin < xnbins; ++xbin) {
    double x = xfirst + xstep * 0.5 + xstep * xbin;
    const auto value = content.at(xbin);
    ECAL::Data1D c(x, value);
    const bool skip = common::isZero(value) && skipZeroes;
    if (!skip) {
      // to avoid overlapping DQM output for different SM
      channel_data.push_back(c);
    }
  }
  return channel_data;
}

}  // namespace

namespace dqmcpp {
namespace readers {

std::vector<ECAL::Data2D> JSONReader::parse2D(const std::string& content,
                                              bool skipZeroes) {
  auto q = common::parseJSON(content);
  return ::parse2D(q, skipZeroes);
}

std::vector<ECAL::ChannelData> JSONReader::parse(const std::string& content) {
  auto q = common::parseJSON(content);
  return ::parse(q);
}

std::vector<ECAL::Data1D> JSONReader::parse1D(const std::string& content,
                                              bool skipZeroes) {
  auto q = common::parseJSON(content);
  return ::parse1D(q, skipZeroes);
}

std::string JSONReader::get(const std::string& url) {
  return dqmcpp::net::URLCache::get(url);
}

}  // namespace readers
}  // namespace dqmcpp