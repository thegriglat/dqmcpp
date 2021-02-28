#ifndef JSONREADER_HH
/**
 * @file JSONReader.hh
 * @brief Reading JSON data from DQM
 *
 * @copyright Copyright (c) 2021
 *
 */
#include "Reader.hh"
#include "../dataclasses/ECALHardware.hh"

#include "../json/json.hh"

class JSONReader : public Reader
{
public:
    JSONReader() {};
    /**
     * @brief Parse string to nlohmann::json
     *
     * @param content
     * @return nlohmann::json
     */
    static nlohmann::json parseJSON(const std::string &content);
    static ECALHardware::ChannelData parse(nlohmann::json &j);
    static inline ECALHardware::ChannelData parse(const std::string &content)
    {
        return parse(parseJSON(content));
    }
};

#define JSONREADER_HH
#endif