#ifndef READER_HH
/**
 * @brief Pure abstract class for readers
 *
 */

#include <string>
#include "URLCache.hh"
#include "../dataclasses/ECAL.hh"

class Reader
{
    static const std::string DQMURL;
    URLCache *_cache = nullptr;

public:
    Reader(URLCache *cache = new URLCache()) : _cache(cache) {};
    // std::string url(const unsigned int runnumber, const std::string &dataset);
    /**
     * @brief Returns HTTP content by url using cache.
     *
     * @param url URL
     * @return std::string
     */
    inline std::string get(const std::string &url)
    {
        return _cache->get(url);
    }
    /**
     * @brief Virtual function for parsing
     *
     * @param content Some std::string parameter, usually HTTP response content
     * @return std::vector<ECAL::ChannelData>
     */
    virtual std::vector<ECAL::ChannelData> parse(const std::string &content) = 0;
    virtual std::vector<ECAL::Data2D> parse2D(const std::string &content) = 0;

};

#define READER_HH
#endif