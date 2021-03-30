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

std::vector<ECAL::ChannelData> parse(const std::string& content);

std::vector<ECAL::Data2D> parse2D(const std::string& content,
                                  bool skipZeroes = true);

std::vector<ECAL::Data1D> parse1D(const std::string& content,
                                  bool skipZeroes = true);

std::string get(const std::string& url);

};  // namespace JSONReader
}  // namespace readers
}  // namespace dqmcpp
#define JSONREADER_HH
#endif