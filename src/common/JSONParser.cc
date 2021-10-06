/**
 * @file JSONParser.cc
 * @author Grigory Latyshev (thegriglat@gmail.com)
 * @brief RapidJSON wrapper
 *
 *
 */
#include "JSONParser.hh"
#include <iostream>

namespace dqmcpp {
namespace common {

rapidjson::Document parseJSON(const std::string& str) {
  using namespace rapidjson;
  Document document;
  try {
    document.Parse(str.c_str());
  } catch (std::exception e) {
    std::cerr << "Cannot parse JSON string:" << std::endl
              << ">>>" << std::endl
              << str << std::endl
              << "<<<" << std::endl
              << "Exception: " << std::endl
              << e.what() << std::endl;
    exit(-1);
  }
  return document;
}

}  // namespace common
}  // namespace dqmcpp