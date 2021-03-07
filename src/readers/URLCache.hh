#ifndef URLCACHE_HH
/**
 * @brief Class to cache URL requests
 *
 */

#include <string>
#include "URLHandler.hh"
class URLCache {
 private:
  URLHandler* _urlhandler = new URLHandler();
  std::string _cachedir = "/tmp";
  /**
   * @brief Calculate hash from std::string. At the moment std::hash() is used.
   *
   * @param text
   * @return std::string
   */
  std::string hash(const std::string& text) const;
  /**
   * @brief Get data from the cache. Assumes that cache entry for url exists.
   *
   * @param url URL
   * @return std::string
   */
  std::string getFromCache(const std::string& url) const;
  /**
   * @brief Adds item to the cache. Overwrites cache item if needed.
   *
   * @param url URL
   * @param content HTTP response content
   */
  void add(const std::string& url, const std::string& content);
  /**
   * @brief Returns cache file name.
   *
   * @param url URL
   * @return std::string
   */
  std::string cFileName(const std::string& url) const;

 public:
  ~URLCache() { delete _urlhandler; }
  /**
   * @brief Set the cache directory. If changed there is NO cache migration
   * mechanism.
   *
   * @param dir
   */
  void setCacheDir(const std::string& dir);
  /**
   * @brief Check if cache has entry
   *
   * @param url URL
   * @return true
   * @return false
   */
  bool has(const std::string& url) const;

  /**
   * @brief get element from cache
   *
   * @param url URL
   * @return std::string
   */
  std::string get(const std::string& url);
};

#define URLCACHE_HH

#endif