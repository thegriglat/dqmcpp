#ifndef JSONREADER_HH
/**
 * @file JSONReader.hh
 * @brief Reading JSON data from DQM
 * 
 * @copyright Copyright (c) 2021
 * 
 */
#include "Reader.hh"

#include "../json/json.hh"

class JSONReader : public Reader
{
    static nlohmann::json parseJSON(const std::string &content);
};

#define JSONREADER_HH
#endif