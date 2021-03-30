#ifndef JSONPARSER_HH

#include <string>
#include "json.hh"
namespace dqmcpp {
namespace common {

nlohmann::json parseJSON(const std::string& str);

}  // namespace common
}  // namespace dqmcpp
#define JSONPARSER_HH
#endif  // !JSONPARSER_HH