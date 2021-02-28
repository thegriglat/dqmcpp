#include "JSONReader.hh"

nlohmann::json JSONReader::parseJSON(const std::string &content)
{
    nlohmann::json j(content);
    return j;
}