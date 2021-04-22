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
#include "../common/logging.hh"
#include "../common/math.hh"
#include "../net/URLCache.hh"

namespace {
using namespace dqmcpp::ECAL;
using namespace dqmcpp;
using namespace rapidjson;
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
bool isValid(const Document& j) {
  const auto& hist = j["hist"];
  // {"hist": "unsupported type"}
  return hist.IsObject();
}

std::vector<ECAL::Data2D> parse2D(Document& j, bool skipZeroes = true) {
  // TODO:: make it common with parse ..
  using namespace std;
  // check validity of json
  if (!isValid(j))
    return std::vector<ECAL::Data2D>();
  const auto& xaxis = j["hist"]["xaxis"];
  const auto& yaxis = j["hist"]["yaxis"];
  const auto xnbins =
      xaxis["last"]["id"].GetInt() - xaxis["first"]["id"].GetInt() + 1;
  const auto xfirst = xaxis["first"]["value"].GetDouble();
  const auto xlast = xaxis["last"]["value"].GetDouble();
  const auto xstep = (xlast - xfirst) / xnbins;

  const auto ynbins =
      yaxis["last"]["id"].GetInt() - yaxis["first"]["id"].GetInt() + 1;
  const auto yfirst = yaxis["first"]["value"].GetDouble();
  const auto ylast = yaxis["last"]["value"].GetDouble();
  const auto ystep = (ylast - yfirst) / ynbins;

  // bin increment always == 1 as it is channel
  const auto& content = j["hist"]["bins"]["content"].GetArray();
  std::vector<ECAL::Data2D> channel_data;
  for (int ybin = 0; ybin < ynbins; ++ybin) {
    double y = yfirst + ystep * 0.5 + ystep * ybin;
    auto xarray = content[ybin].GetArray();
    for (int xbin = 0; xbin < xnbins; ++xbin) {
      double x = xfirst + xstep * 0.5 + xstep * xbin;
      const auto value = xarray[xbin].GetDouble();
      ECAL::Data2D c(Point2D(x, y), value);
      const bool skip = common::isZero(value) && skipZeroes;
      if (!skip) {
        // to avoid overlapping DQM output for different SM
        channel_data.push_back(c);
      }
    }
  };
  return channel_data;
}

std::vector<dqmcpp::ECAL::ChannelData> parse(Document& j) {
  using namespace std;
  // check validity of json
  const auto data2d = parse2D(j);
  if (data2d.size() == 0)
    return std::vector<ECAL::ChannelData>();
  const auto iz = getECALDetector(j["hist"]["title"].GetString());
  vector<ECAL::ChannelData> channel_data;
  const auto& xaxis = j["hist"]["xaxis"];
  const std::string xtitle = xaxis["title"].GetString();
  const char xsign = (xtitle.find('-') != xtitle.npos) ? -1 : 1;
  for (auto& datapoint : data2d) {
    const int x = std::trunc(datapoint.base.x);
    const int y = std::trunc(datapoint.base.y);
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

std::vector<ECAL::Data1D> parse1D(Document& j, bool skipZeroes) {
  // TODO:: make it common with parse ..
  using namespace std;
  // check validity of json
  if (!isValid(j))
    return std::vector<ECAL::Data1D>();
  const auto& xaxis = j["hist"]["xaxis"];
  const auto xnbins =
      xaxis["last"]["id"].GetInt() - xaxis["first"]["id"].GetInt() + 1;
  const auto xfirst = xaxis["first"]["value"].GetDouble();
  const auto xlast = xaxis["last"]["value"].GetDouble();
  const auto xstep = (xlast - xfirst) / xnbins;

  // bin increment always == 1 as it is channel
  const auto content = j["hist"]["bins"]["content"].GetArray();
  std::vector<ECAL::Data1D> channel_data;
  for (int xbin = 0; xbin < xnbins; ++xbin) {
    double x = xfirst + xstep * 0.5 + xstep * xbin;
    const auto value = content[xbin].GetDouble();
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
