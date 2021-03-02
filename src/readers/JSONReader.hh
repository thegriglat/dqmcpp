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

#include "../common/json.hh"

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
    /**
     * @brief Parses DQM json object to vector of channel data
     *
     * @param j
     * @return std::vector<ECALHardware::ChannelData>
     */
    static std::vector<ECALHardware::ChannelData> parse(nlohmann::json &j);
    /**
     * @brief Parse json file to vector of channel data
     *
     * @param content
     * @return std::vector<ECALHardware::ChannelData>
     */
    static inline std::vector<ECALHardware::ChannelData> parse(const std::string &content)
    {
        auto q = parseJSON(content);
        return parse(q);
    }
    /**
     * @brief Check that DQM json is valid
     *
     * @param j
     * @return true
     * @return false
     */
    static bool isValid(nlohmann::json &j);
};

#define JSONREADER_HH
#endif