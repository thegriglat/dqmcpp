#ifndef URLCACHE_HH
/**
 * @brief Class to cache URL requests
 *
 */

#include <string>
#include "URLHandler.hh"
class URLCache
{
private:
    URLHandler *_urlhandler = new URLHandler();
    std::string _cachedir = "/tmp";
    std::string hash(const std::string &text) const;
    std::string getFromCache(const std::string &url) const;
    void add(const std::string &url, const std::string &content);
    std::string cFileName(const std::string &url) const;

public:
    ~URLCache()
    {
        delete _urlhandler;
    }
    /**
    * @brief Set the Cache Dir object
    *
    * @param dir
    */
    void setCacheDir(const std::string &dir);
    /**
    * @brief Check if cache has entry
    *
    * @param url
    * @return true
    * @return false
    */
    bool has(const std::string &url) const;

    /**
     * @brief Clean cache (WIP)
     *
     */
    void clean(void);
    /**
     * @brief get element from cache
     *
     * @param url
     * @return std::string
     */
    std::string get(const std::string &url);
};

#define URLCACHE_HH

#endif