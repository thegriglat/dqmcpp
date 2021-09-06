#ifndef JSONREADER_HH
/**
 * @file JSONReader.hh
 * @brief Reading JSON data from DQM
 *
 */
#include "../ECAL/ECAL.hh"

namespace dqmcpp {
namespace readers {
namespace JSONReader {

/**
 * @brief Parse DQM JSON to ECAL::ChannelData. For ECAL channels histograms
 *
 * @param content
 * @return std::vector<ECAL::ChannelData>
 */
std::vector<ECAL::ChannelData> parse(const std::string& content);

/**
 * @brief Parse DQM JSON to ECAL::Data2D. For two-dimensional histograms and
 * TT/CCU histograms.
 *
 * @param content
 * @param skipZeroes
 * @return std::vector<ECAL::Data2D>
 */
std::vector<ECAL::Data2D> parse2D(const std::string& content,
                                  bool skipZeroes = true);

/**
 * @brief Parse DQM JSON to ECAL::Data1D. For one-dimensional histograms.
 *
 * @param content
 * @param skipZeroes
 * @return std::vector<ECAL::Data1D>
 */
std::vector<ECAL::Data1D> parse1D(const std::string& content,
                                  bool skipZeroes = true);

/**
 * @brief Get content by URL
 *
 * @param url
 * @return std::string
 */
std::string get(const std::string& url);

};  // namespace JSONReader
}  // namespace readers
}  // namespace dqmcpp
#define JSONREADER_HH
#endif