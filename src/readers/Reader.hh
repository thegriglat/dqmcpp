#ifndef READER_HH
/**
 * @brief Pure abstract class for readers
 *
 */

#include <string>
#include "URLCache.hh"

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
};

#define READER_HH
#endif