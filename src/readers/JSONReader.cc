#include "JSONReader.hh"

nlohmann::json JSONReader::parseJSON(const std::string &content)
{
    return nlohmann::json::parse(content);
}