#ifndef JSONREADER_HH
/**
 * @file JSONReader.hh
 * @brief Reading JSON data from DQM
 *
 */
#include "../ECAL/ECAL.hh"

namespace dqmcpp {
namespace readers {
/**
 * @brief JSON parsing functions
 *
 */
namespace JSONReader {

/**
 * @brief Parse DQM JSON to ECAL::ChannelData. For ECAL channels SM histograms.
 *
 *
 * @param content JSON text
 * @return std::vector<ECAL::ChannelData>
 */
std::vector<ECAL::ChannelData> parse(const std::string& content);

/**
 * @brief Parse DQM JSON to ECAL::Data2D. For two-dimensional histograms,
 * TT/CCU and full detector histograms.
 *
 * @param content JSON text
 * @param skipZeroes exclude zero values
 * @return std::vector<ECAL::Data2D>
 */
std::vector<ECAL::Data2D> parse2D(const std::string& content,
                                  bool skipZeroes = true);

/**
 * @brief Parse DQM JSON to ECAL::Data1D. For one-dimensional histograms.
 *
 * @param content JSON text
 * @param skipZeroes exclude zero values
 * @return std::vector<ECAL::Data1D>
 */
std::vector<ECAL::Data1D> parse1D(const std::string& content,
                                  bool skipZeroes = true);

/**
 * @brief Get content by URL
 *
 * @param url URL
 * @return std::string
 */
std::string get(const std::string& url);

};  // namespace JSONReader
}  // namespace readers
}  // namespace dqmcpp
#define JSONREADER_HH
#endif