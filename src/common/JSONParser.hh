#ifndef JSONPARSER_HH

#include <string>
#include "../rapidjson/document.h"
namespace dqmcpp {
namespace common {

/**
 * @brief Parses string JSON to rapidjson::Document
 *
 * @param content input string
 * @return rapidjson::Document
 */
rapidjson::Document parseJSON(const std::string& content);

}  // namespace common
}  // namespace dqmcpp
#define JSONPARSER_HH
#endif  // !JSONPARSER_HH