#include "JSONParser.hh"

#include "json.hh"

namespace dqmcpp {
namespace common {

nlohmann::json parseJSON(const std::string& content) {
  auto j = nlohmann::json::parse(content);
  return j;
}

}  // namespace common
}  // namespace dqmcpp