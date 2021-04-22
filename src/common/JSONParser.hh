#ifndef JSONPARSER_HH

#include <string>
#include "../rapidjson/document.h"
namespace dqmcpp {
namespace common {

rapidjson::Document parseJSON(const std::string& content);

}  // namespace common
}  // namespace dqmcpp
#define JSONPARSER_HH
#endif  // !JSONPARSER_HH