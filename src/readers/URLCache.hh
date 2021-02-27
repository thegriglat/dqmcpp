#ifndef URLCACHE_HH
/**
 * @brief Class to cache URL requests
 * 
 */

#include <string>
#include "../json/json.hh"
class URLCache
{
public:
    bool has(const std::string &url) const;
    void clean(void);
    nlohmann::json get(const std::string &url);
};

#define URLCACHE_HH

#endif