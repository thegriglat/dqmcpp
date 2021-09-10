/**
 * @file JSONParser.cc
 * @author Grigory Latyshev (thegriglat@gmail.com)
 * @brief RapidJSON wrapper
 *
 *
 */
#include "JSONParser.hh"

namespace dqmcpp {
namespace common {

rapidjson::Document parseJSON(const std::string& str) {
  using namespace rapidjson;
  Document document;
  document.Parse(str.c_str());
  return document;
}

}  // namespace common
}  // namespace dqmcpp