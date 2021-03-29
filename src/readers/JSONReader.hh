#ifndef JSONREADER_HH
/**
 * @file JSONReader.hh
 * @brief Reading JSON data from DQM
 *
 */
#include "../ECAL/ECAL.hh"
#include "../common/json.hh"
#include "../net/URLCache.hh"

namespace dqmcpp {
namespace readers {

namespace JSONReader {
/**
 * @brief Parse string to nlohmann::json
 *
 * @param content HTTP response content
 * @return nlohmann::json
 */
nlohmann::json parseJSON(const std::string& content);
/**
 * @brief Parses DQM json object to vector of channel data
 *
 * @param j Parsed HTTP response content
 * @return std::vector<ECAL::ChannelData>
 */
std::vector<ECAL::ChannelData> parse(nlohmann::json& j);
/**
 * @brief Parse json file to vector of channel data, inline wrapper function
 *
 * @param contentHTTP response content
 * @return std::vector<ECAL::ChannelData>
 */
inline std::vector<ECAL::ChannelData> parse(const std::string& content) {
  auto q = parseJSON(content);
  return parse(q);
}
/**
 * @brief Parse DQM json as vector of 2D points
 *
 * @param j
 * @return std::vector<ECAL::Data2D>
 */
std::vector<ECAL::Data2D> parse2D(nlohmann::json& j, bool skipZeroes = true);
/**
 * @brief Parse DQM HTTP output as vector of 2D points
 *
 * @param j
 * @return std::vector<ECAL::Data2D>
 */
inline std::vector<ECAL::Data2D> parse2D(const std::string& content,
                                         bool skipZeroes = true) {
  auto q = parseJSON(content);
  return parse2D(q, skipZeroes);
}

std::vector<ECAL::Data1D> parse1D(nlohmann::json& j, bool skipZeroes = true);
inline std::vector<ECAL::Data1D> parse1D(const std::string& content,
                                         bool skipZeroes = true) {
  auto q = parseJSON(content);
  return parse1D(q, skipZeroes);
}

inline std::string get(const std::string& url) {
  return dqmcpp::net::URLCache::get(url);
}

};  // namespace JSONReader
}  // namespace readers
}  // namespace dqmcpp
#define JSONREADER_HH
#endif