#ifndef JSONREADER_HH
/**
 * @file JSONReader.hh
 * @brief Reading JSON data from DQM
 *
 */
#include "../ECAL/ECAL.hh"
#include "../common/json.hh"
#include "Reader.hh"

namespace dqmcpp {
namespace readers {

class JSONReader : public Reader {
 public:
  JSONReader(){};
  /**
   * @brief Parse string to nlohmann::json
   *
   * @param content HTTP response content
   * @return nlohmann::json
   */
  static nlohmann::json parseJSON(const std::string& content);
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
  std::vector<ECAL::Data2D> parse2D(nlohmann::json& j);
  inline std::vector<ECAL::Data2D> parse2D(const std::string& content) {
    auto q = parseJSON(content);
    return parse2D(q);
  }

  /**
   * @brief Check that DQM json is valid. Just check that some data returned.
   * No "smart" checks performed.
   * @param j Parsed HTTP response content
   * @return true
   * @return false
   */
  static bool isValid(nlohmann::json& j);
};

}  // namespace readers
}  // namespace dqmcpp
#define JSONREADER_HH
#endif